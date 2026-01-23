#ifndef CUSTOMSTYLESHEETS_H
#define CUSTOMSTYLESHEETS_H

#include <QMainWindow>

#include "custom_colors.h"
#include "custom_metrics.h"

class CustomStyleSheets
{
public:
    CustomStyleSheets ();

    QString vertical_scrollbar (void);

    QString table_scrollbar (int);

    QString vertical_scrollbar_quirk (void);


private:
    void setHeaderHeight (int);

    ScrollBar_Metrics sb_metrics;
};

#endif // CUSTOMSTYLESHEETS_H
