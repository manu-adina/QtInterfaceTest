#include "rosgroundcontrol.h"
#include <QApplication>
#include <gst/gst.h>
#include <gst/video/videooverlay.h> // For GST_VIDEO_OVERLAY variable.

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ROSGroundControl w;

    /* Gstreamer goes here */
    gst_init(&argc, &argv);
    GstElement *pipeline = gst_parse_launch("udpsrc port=5801 ! rtpjitterbuffer latency=50 ! rtph264depay ! h264parse ! omxh264dec ! videoconvert ! ximagesink name=mySink", NULL);
    GstElement *sink = gst_bin_get_by_name((GstBin*)pipeline, "mySink");

    WId xwinid = w.winId();
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), (guintptr)xwinid);


    w.show();

    /* Play the video */
    GstStateChangeReturn sret = gst_element_set_state(pipeline, GST_STATE_PLAYING);

    return a.exec();
}
