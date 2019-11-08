#include "rosgroundcontrol.h"
#include <QApplication>
#include <gst/gst.h>
#include <gst/video/videooverlay.h> // For GST_VIDEO_OVERLAY variable.

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ROSGroundControl w;

//    QWidget *window = new QWidget();
//    window->setWindowTitle("udpsrc video stream");
//    window->resize(700, 700);

    /* Gstreamer goes here */
    gst_init(&argc, &argv);
    GstElement *pipeline = gst_parse_launch("udpsrc port=5801 ! application/x-rtp,payload=(int)96,encoding-name=(string)H264,clock-rate=(int)90000 ! rtpjitterbuffer latency=50 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! ximagesink name=mySink", NULL);
    GstElement *sink = gst_bin_get_by_name((GstBin*)pipeline, "mySink");

//    WId xwinid = window->winId();
//    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), (guintptr)xwinid);

    WId xwinid = w.winId();
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), (guintptr)xwinid);

    w.show();
//    window->show();

    /* Play the video */
    GstStateChangeReturn sret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    return a.exec();
}
