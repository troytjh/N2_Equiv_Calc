#include <QDebug>
#include <QStyleOption>
#include <QStringBuilder>

#include "custom_stylesheets.h"
#include "custom_colors.h"

CustomStyleSheets::CustomStyleSheets()
{
}

QString CustomStyleSheets::vertical_scrollbar (void)
{
    // add space on either side of the scroll handle
    QString stylesheet =
    "QScrollBar::Vertical { \
         border: none; \
         background: transparent; \
         padding-left: %2px; \
         padding-right: %2px; \
         border-radius: 0px; \
    }";
    stylesheet = stylesheet.arg(sb_metrics.padding);


    // set scrollbar handle colors
    stylesheet +=
    "QScrollBar::handle:vertical { \
         border: 0.5px solid rgb(%1,%2,%3); \
         background-color: rgb(%4,%5,%6); \
         min-height: 30px; \
         border-radius: %7px; \
    }";
    stylesheet = stylesheet.arg(CustomColors::scrollbar_handle_outline().red())
                           .arg(CustomColors::scrollbar_handle_outline().green())
                           .arg(CustomColors::scrollbar_handle_outline().blue());

    stylesheet = stylesheet.arg(CustomColors::scrollbar_handle_bgColor().red())
                           .arg(CustomColors::scrollbar_handle_bgColor().green())
                           .arg(CustomColors::scrollbar_handle_bgColor().blue());

    // round both ends of handle
    stylesheet = stylesheet.arg(sb_metrics.handle_radius);


    // remove arrow button at top of scrollbar
    stylesheet +=
    "QScrollBar::sub-line:vertical { \
         border: none; \
         background-color: transparent; \
         height: 0px; \
         subcontrol-position: top; \
         subcontrol-origin: margin; \
    }";


    // remove arrow button at bottom of scrollbar
    stylesheet +=
    "QScrollBar::add-line:vertical { \
         border: none; \
         background-color: transparent; \
         height: 0px; \
         subcontrol-position: bottom; \
         subcontrol-origin: margin; \
    }";

    return stylesheet;
}


QString CustomStyleSheets::table_scrollbar (int header_height)
{
    setHeaderHeight(header_height);

    /* Vertical ScrollBar */
    QString stylesheet =
    "QScrollBar::Vertical { \
         border-left: 1px solid rgba(%1,%2,%3,%4); \
         border-right: 1px solid rgba(%5,%6,%7,%8); \
         background: rgb(%9,%10,%11); \
         padding-top: %12px; \
         padding-bottom: 10px; \
    }";

    // line to visually seperate scrollbar area from treeview contents
    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().red())
                           .arg(CustomColors::inner_contrast_line().green())
                           .arg(CustomColors::inner_contrast_line().blue())
                           .arg(CustomColors::inner_contrast_line().alpha());

    // match outer color of treeview frame
    stylesheet = stylesheet.arg(CustomColors::outer_frame_border_color().red())
                           .arg(CustomColors::outer_frame_border_color().green())
                           .arg(CustomColors::outer_frame_border_color().blue())
                           .arg(CustomColors::outer_frame_border_color().alpha());

    // match background color of scrollbar to frame
    stylesheet = stylesheet.arg(CustomColors::frame_color().red())
                           .arg(CustomColors::frame_color().green())
                           .arg(CustomColors::frame_color().blue());

    // shift scrollbar start below the treeview header
    stylesheet = stylesheet.arg(sb_metrics.treeview_scrollbar_padding);


    // fill top scrollbar area with colors to match the header colors
    stylesheet +=
    "QScrollBar::sub-line:vertical { \
         border-top: 1px solid rgba(%1,%2,%3,%4); \
         border-bottom: 1px solid rgb(%5,%6,%7); \
         background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \
                                           stop:0 rgb(%8,%9,%10) \
                                           stop:0.5 rgb(%11,%12,%13) \
                                           stop:0.501 rgb(%14,%15,%16) \
                                           stop:0.92 rgb(%17,%18,%19) \
                                           stop:1 rgb(%20,%21,%22)); \
         height: %23px; \
    }";
    // header top border
    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().red())
                           .arg(CustomColors::inner_contrast_line().green())
                           .arg(CustomColors::inner_contrast_line().blue())
                           .arg(CustomColors::inner_contrast_line().alpha());

    // header bottom border
    stylesheet = stylesheet.arg(CustomColors::outline().red())
                           .arg(CustomColors::outline().green())
                           .arg(CustomColors::outline().blue());

    // header gradient
    stylesheet = stylesheet.arg(CustomColors::header_gradientStart().red())
                           .arg(CustomColors::header_gradientStart().green())
                           .arg(CustomColors::header_gradientStart().blue());

    stylesheet = stylesheet.arg(CustomColors::header_midColor1().red())
                           .arg(CustomColors::header_midColor1().green())
                           .arg(CustomColors::header_midColor1().blue());

    stylesheet = stylesheet.arg(CustomColors::header_midColor2().red())
                           .arg(CustomColors::header_midColor2().green())
                           .arg(CustomColors::header_midColor2().blue());

    stylesheet = stylesheet.arg(CustomColors::header_gradientStop().red())
                           .arg(CustomColors::header_gradientStop().green())
                           .arg(CustomColors::header_gradientStop().blue());

    stylesheet = stylesheet.arg(CustomColors::header_gradientStop().darker(104).red())
                           .arg(CustomColors::header_gradientStop().darker(104).green())
                           .arg(CustomColors::header_gradientStop().darker(104).blue());

    stylesheet = stylesheet.arg(sb_metrics.header_height);


    stylesheet +=
    "QTableWidget { \
         gridline-color: rgba(%1,%2,%3,%4)\
    }";

    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().red());
    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().green());
    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().blue());
    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().alpha());


    return stylesheet;
}


/* Fix vertical scrollbar not painting bottom border in Tree and List Views
**
** Painting the border produces a unnecessary line when
** the horizontal scrollbar is visible
**
** See MainWindow::eventFilter for implementation
**
**/
QString CustomStyleSheets::vertical_scrollbar_quirk (void)
{
    QString stylesheet =
    "QScrollBar::add-line:vertical { \
         border-bottom: 1px solid rgba(%5,%6,%7,%8); \
    }";

    stylesheet = stylesheet.arg(CustomColors::outer_frame_border_color().red())
                           .arg(CustomColors::outer_frame_border_color().green())
                           .arg(CustomColors::outer_frame_border_color().blue())
                           .arg(CustomColors::outer_frame_border_color().alpha());

    return stylesheet;
}


void CustomStyleSheets::setHeaderHeight(int height)
{
    if (height < 2)
    {
        qWarning() << "Warning: Passing header height of less than 2px"
                   << "to setHeaderHeight";
        return;
    }

    sb_metrics.header_height = height - 2;
    sb_metrics.treeview_scrollbar_padding = height + 6;
}
