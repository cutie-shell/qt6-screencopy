#pragma once

#include <QtWaylandClient/QtWaylandClient>

class ScreencopyManagerV1Private;
class ScreencopyFrameV1Private;
class ScreencopyFrameV1;

class Q_WAYLANDCLIENT_EXPORT ScreencopyManagerV1 : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(ScreencopyManagerV1)
    public:
	ScreencopyManagerV1();

	ScreencopyFrameV1 *capture();

    signals:

    private:
	ScreencopyManagerV1Private *d_ptr;
};

class Q_WAYLANDCLIENT_EXPORT ScreencopyFrameV1 : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(ScreencopyFrameV1)
    public:
	ScreencopyFrameV1(void *object);

    signals:
	void failed();
	void ready(QImage image);

    private:
	ScreencopyFrameV1Private *d_ptr;
};