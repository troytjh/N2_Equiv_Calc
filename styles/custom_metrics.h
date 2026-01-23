#ifndef CUSTOM_METRICS_H
#define CUSTOM_METRICS_H

struct ScrollBar_Metrics
{
    // ScrollBar
    const int width = 16;
    const int padding = 3; // distance between handle and border (floating handle)

    // ScrollBar Handle
    const int handle_minimum = 30;
    const int handle_radius = 4;

    // TreeView
    int treeview_scrollbar_padding = 35;
    int header_height = 27;
};

#endif // CUSTOM_METRICS_H
