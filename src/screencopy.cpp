#include "screencopy_p.h"

ScreencopyManagerV1::ScreencopyManagerV1()
	: d_ptr(new ScreencopyManagerV1Private(this)) {}

ScreencopyFrameV1 *ScreencopyManagerV1::capture() {
	Q_D(ScreencopyManagerV1);
	return new ScreencopyFrameV1(
		(void*)d->capture_output(
		0, static_cast<QtWaylandClient::QWaylandScreen*>(
		qApp->screens().first()->handle())->output()));
}

ScreencopyManagerV1Private::ScreencopyManagerV1Private(
	ScreencopyManagerV1 *q
	) : QWaylandClientExtensionTemplate(3),
		q_ptr(q) { }

ScreencopyFrameV1::ScreencopyFrameV1(void *object)
	: d_ptr(new ScreencopyFrameV1Private(this, (struct ::zwlr_screencopy_frame_v1 *)object)) {}

ScreencopyFrameV1Private::ScreencopyFrameV1Private(
	ScreencopyFrameV1 *q,
	struct ::zwlr_screencopy_frame_v1 *object
) : QWaylandClientExtensionTemplate(3),
	q_ptr(q),
	QtWayland::zwlr_screencopy_frame_v1(object) {}

ScreencopyFrameV1Private::~ScreencopyFrameV1Private() {
	delete m_shmBuffer;
}

void ScreencopyFrameV1Private::zwlr_screencopy_frame_v1_buffer(uint32_t format, uint32_t width, uint32_t height, uint32_t stride) {
	m_shmBuffer = new QtWaylandClient::QWaylandShmBuffer(
		integration()->display(), QSize(width, height), QImage::Format_ARGB32_Premultiplied);
	copy(m_shmBuffer->buffer());
}

void ScreencopyFrameV1Private::zwlr_screencopy_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) {
	Q_Q(ScreencopyFrameV1);
	m_image = QImage(*m_shmBuffer->image());
	emit q->ready(&m_image);
}