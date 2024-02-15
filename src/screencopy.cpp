#include "screencopy_p.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

ScreencopyManagerV1::ScreencopyManagerV1()
	: d_ptr(new ScreencopyManagerV1Private(this))
{
}

ScreencopyFrameV1 *ScreencopyManagerV1::capture()
{
	Q_D(ScreencopyManagerV1);
	return new ScreencopyFrameV1((void *)d->capture_output(
		0, static_cast<QtWaylandClient::QWaylandScreen *>(
			   qApp->screens().first()->handle())
			   ->output()));
}

ScreencopyManagerV1Private::ScreencopyManagerV1Private(ScreencopyManagerV1 *q)
	: QWaylandClientExtensionTemplate(3)
	, q_ptr(q)
{
}

ScreencopyFrameV1::ScreencopyFrameV1(void *object)
	: d_ptr(new ScreencopyFrameV1Private(
		  this, (struct ::zwlr_screencopy_frame_v1 *)object))
{
}

ScreencopyFrameV1Private::ScreencopyFrameV1Private(
	ScreencopyFrameV1 *q, struct ::zwlr_screencopy_frame_v1 *object)
	: QWaylandClientExtensionTemplate(3)
	, q_ptr(q)
	, QtWayland::zwlr_screencopy_frame_v1(object)
{
}

ScreencopyFrameV1Private::~ScreencopyFrameV1Private()
{
}

void ScreencopyFrameV1Private::zwlr_screencopy_frame_v1_buffer(uint32_t format,
							       uint32_t width,
							       uint32_t height,
							       uint32_t stride)
{
	if (m_formatGiven)
		return;
	m_bufferDimensions = QSize(width, height);
	m_bufferFormat = format;
	m_bufferStride = stride;
	m_formatGiven = true;
}

void ScreencopyFrameV1Private::zwlr_screencopy_frame_v1_buffer_done()
{
	Q_Q(ScreencopyFrameV1);
	if (m_formatGiven) {
		create_wl_buffer(m_bufferFormat, m_bufferStride);
		copy(m_buffer);
	} else {
		emit q->failed();
		destroy();
		delete q;
		delete this;
	}
}

void ScreencopyFrameV1Private::zwlr_screencopy_frame_v1_ready(
	uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec)
{
	Q_Q(ScreencopyFrameV1);
	emit q->ready(QImage(m_bufferData, m_bufferDimensions.width(),
			     m_bufferDimensions.height(),
			     m_bufferDimensions.width() * 4,
			     QImage::Format_ARGB32_Premultiplied)
			      .rgbSwapped());
	destroy();
	munmap(m_bufferData, m_bufferSize);
	wl_buffer_destroy(m_buffer);
	delete q;
	delete this;
}

void ScreencopyFrameV1Private::create_wl_buffer(uint32_t format,
						uint32_t stride)
{
	m_bufferSize = m_bufferDimensions.height() * stride;
	int m_fd = memfd_create("wl_shm", MFD_CLOEXEC | MFD_ALLOW_SEALING);

	if (m_fd == -1) {
		qErrnoWarning("Cannot create buffer file");
		return;
	}

	ftruncate(m_fd, m_bufferSize);
	m_bufferData = (uchar *)mmap(nullptr, m_bufferSize,
				     PROT_READ | PROT_WRITE, MAP_SHARED, m_fd,
				     0);

	if (m_bufferData == MAP_FAILED) {
		qErrnoWarning("mmap failed");
		return;
	}

	struct wl_shm_pool *pool = wl_shm_create_pool(
		integration()->display()->shm()->object(), m_fd, m_bufferSize);
	m_buffer = wl_shm_pool_create_buffer(pool, 0,
					     m_bufferDimensions.width(),
					     m_bufferDimensions.height(),
					     stride, format);
	close(m_fd);
	wl_shm_pool_destroy(pool);
}
