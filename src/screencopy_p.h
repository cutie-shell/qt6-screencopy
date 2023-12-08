#pragma once

#include <qt6-screencopy.h>

#include <wayland-util.h>
#include <QtWaylandClient/QWaylandClientExtensionTemplate>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandinputdevice_p.h>
#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandintegration_p.h>
#include <QtWaylandClient/private/qwaylandshmbackingstore_p.h>
#include "qwayland-wlr-screencopy-unstable-v1.h"

class ScreencopyManagerV1Private
	: public QWaylandClientExtensionTemplate<ScreencopyManagerV1Private>
	, public QtWayland::zwlr_screencopy_manager_v1 {
	Q_OBJECT
	Q_DECLARE_PUBLIC(ScreencopyManagerV1)
public:
	ScreencopyManagerV1Private(ScreencopyManagerV1 *q);

protected:

private slots:

private:
	ScreencopyManagerV1 *q_ptr;
};

class ScreencopyFrameV1Private
	: public QWaylandClientExtensionTemplate<ScreencopyFrameV1Private>
	, public QtWayland::zwlr_screencopy_frame_v1 {
	Q_OBJECT
	Q_DECLARE_PUBLIC(ScreencopyFrameV1)
public:
	ScreencopyFrameV1Private(
		ScreencopyFrameV1 *q,
		struct ::zwlr_screencopy_frame_v1 *object);
	~ScreencopyFrameV1Private();

protected:
	void zwlr_screencopy_frame_v1_buffer(uint32_t format, uint32_t width, uint32_t height, uint32_t stride) override;
    void zwlr_screencopy_frame_v1_ready(uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) override;

	struct wl_buffer *m_buffer;
	QSize m_bufferDimensions;
	size_t m_bufferSize;
	uchar *m_bufferData;

private:
	ScreencopyFrameV1 *q_ptr;
	int m_fd;
	void create_wl_buffer(uint32_t format, uint32_t stride);
};
