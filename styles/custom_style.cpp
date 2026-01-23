/* Custom Application Style
** File: style_custom.cpp
** --------
** Make changes to the default "Fusion" Style
** --------
**
** Features:
** - Add Rounded corners on button elements
**   * corner_radius: controls roundness of corners
**
** - Change button color
**   * background_color: controls button's base color
**   * button_brightness: change button color brightness
*/

#include <QDebug>

/* QT header files */
#include <QtMath>
#include <QAbstractScrollArea>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QPushButton>
#include <QScrollBar>
#include <QString>
#include <QStyleOption>
#include <QStyleFactory>
#include <QTreeView>

#include <qdrawutil.h>

/* local includes */
#include "custom_style.h"
#include "custom_style_p.h"

#include "custom_colors.h"


/* Helpful debugging commands */
// qDebug() << QString("#%1").arg(QColor().rgb(), 8, 16);

//QColor c = d_ptr->outline(option->palette);
//qDebug() << QString("#%1%2%3%4").arg(c.red(),2,16).arg(c.green(),2,16).arg(c.blue(),2,16).arg(c.alpha(),2,16);


/* Constructor */
CustomStyle::CustomStyle () :
    QProxyStyle (QStyleFactory::create("fusion"))
{
    setObjectName ("custom");
}

/* Private Contructor */
CustomStyle::CustomStyle (CustomStylePrivate &dd) : d_ptr(&dd)
{
}

/* Destructor */
CustomStyle::~CustomStyle ()
{
}


/* Define visual representation of UI elements
** Parameters:
**     primitive element - specify which UI element to draw
**     style options - specify how and where to render the element
**     painter - paint handler to draw the element
**     widget (optional) - specify a widget to draw on
** Return: none
** Notes: re-implementation of fusion style
*/
void CustomStyle::drawPrimitive (PrimitiveElement element,
                                  const QStyleOption *option,
                                  QPainter *painter,
                                  const QWidget *widget) const
{
    // Access color options from private data pointer
    QColor outline = CustomColors::outline(option->palette);
    QColor highlighted_outline = CustomColors::highlighted_outline (option->palette);

    // Initialize button shape
    QRect button_shape;
    QRect rect = option->rect;

    int corner_radius = qMin (rect.width(), rect.height()) / 6;

    // Primitive element selector
    switch (element) {
    case PE_PanelButtonCommand:
    {

        // Set button color
        QColor button_color = CustomColors::button_color();

        // Check button state
        bool isDefault = false;
        bool isDown = (option->state & State_Sunken) || (option->state & State_On);

        if (const QStyleOptionButton *button =
            qstyleoption_cast<const QStyleOptionButton*>(option))
        {
            isDefault =
                (button->features & QStyleOptionButton::DefaultButton)
                && (button->state & State_Enabled);
        }

        bool isEnabled = option->state & State_Enabled;
        bool hasFocus =
                (option->state & State_HasFocus &&
                 option->state & State_KeyboardFocusChange);

        // Set highlight color for selected (default) button
        QColor dark_outline = outline;
        if (hasFocus | isDefault)
        {
            dark_outline = highlighted_outline;
        }

        if (isDefault)
            button_color = CustomColors::merged_colors (button_color,
                                                        highlighted_outline.lighter(130),
                                                        90);

        // Set button shape and location
        QPainter *p = painter;
        button_shape = rect.adjusted (0, 1, -1, 0);

        // Use anti-aliasing and corners
        p->setRenderHint (QPainter::Antialiasing, true);
        p->translate (0.5, -0.5);

        // Draw subtle gradient from top to bottum
        QLinearGradient gradient =
                CustomColors::qt_fusion_gradient
                (rect, (isEnabled && option->state & State_MouseOver)
                 ? button_color : button_color.darker (104));

        // Paint button color based on whether button's pressed
        p->setPen (Qt::transparent);
        p->setBrush (isDown ? QBrush(button_color.darker(110)) : gradient);
        p->drawRoundedRect (button_shape, corner_radius, corner_radius);
        p->setBrush (Qt::NoBrush);

        // Paint button shadow
        p->setPen (!isEnabled ? QPen(dark_outline.lighter(115)) : QPen(dark_outline));
        p->drawRoundedRect (button_shape, corner_radius, corner_radius);

        // Paint semi-transparent inner outline
        p->setPen (CustomColors::inner_contrast_line());
        p->drawRoundedRect (button_shape.adjusted(1, 1, -1, -1), corner_radius, corner_radius);

        break;
    }
    case PE_Frame:
    {
        if (widget && widget->inherits("QComboBoxPrivateContainer"))
        {
            QStyleOption copy = *option;
            copy.state |= State_Raised;
            proxy()->drawPrimitive (PE_PanelMenu, &copy, painter, widget);
            break;
        }
        painter->save();
        painter->setRenderHint (QPainter::Antialiasing, true);
        painter->translate (0.5, 0.5);

        // Draw frame background color
        painter->setPen (Qt::transparent);
        painter->setBrush (CustomColors::frame_color());
        painter->drawRoundedRect (option->rect.adjusted(0,0,-1,-1), 1, 1);
        painter->setBrush (Qt::NoBrush);

        // Draw inner border line
        painter->setPen (CustomColors::inner_frame_border());
        painter->drawRoundedRect (option->rect.adjusted(1,1,-2,-2), 1, 1);

        // Draw outer border line
        painter->setPen (CustomColors::outer_frame_border());
        painter->drawRoundedRect (option->rect.adjusted(1,1,-2,-2), -2, -2);

        painter->restore();
        break;
    }
    case PE_FrameLineEdit:
    {
        QRect r = rect;
        bool hasFocus = option->state & State_HasFocus;

        painter->save();

        painter->setRenderHint(QPainter::Antialiasing, true);
        //  ### highdpi painter bug.
        painter->translate(0.5, 0.5);

        // Draw Outline
        painter->setPen( QPen(hasFocus ? highlighted_outline : outline.lighter(300)));
        painter->drawRoundedRect(r.adjusted(0, 0, -1, -1), 2, 2);

        if (hasFocus) {
            QColor softHighlight = highlighted_outline;
            softHighlight.setAlpha(40);
            painter->setPen(softHighlight);
            painter->drawRoundedRect(r.adjusted(1, 1, -2, -2), 1.7, 1.7);
        }
        // Draw inner shadow
        painter->setPen(CustomColors::top_shadow());
        painter->drawLine(QPoint(r.left() + 2, r.top() + 1), QPoint(r.right() - 2, r.top() + 1));

        // Draw frame background color
        painter->setPen (Qt::transparent);
        painter->setBrush (CustomColors::frame_color());
        painter->drawRoundedRect (option->rect.adjusted(0,0,-1,-1), 1, 1);
        painter->setBrush (Qt::NoBrush);

        painter->restore();

    }
    break;
    case PE_PanelTipLabel: {
        const QBrush brush(Qt::NoBrush);
        qDrawPlainRect(painter, option->rect, CustomColors::frame_font_color(), 0, &brush);
        break;
    }
    default:
        QProxyStyle::drawPrimitive (element, option, painter, widget);
    }
}


/*
** Parameters:
**     control element -
**     style options - specify how and where to render the element
**     painter - paint handler to draw the element
**     widget (optional) - specify a widget to draw on
** Return: none
** Notes: re-implementation of fusion style
*/
void CustomStyle::drawControl (ControlElement element,
                               const QStyleOption *option,
                               QPainter *painter,
                               const QWidget *widget) const
{
    QRect rect = option->rect;

    switch (element) {
    case CE_HeaderSection:
        painter->save();

        // Draw header view for frame objects
        if (const QStyleOptionHeader *header =
            qstyleoption_cast<const QStyleOptionHeader *>(option))
        {
            const QStyleOptionHeaderV2 *headerV2 =
                    qstyleoption_cast<const QStyleOptionHeaderV2 *>(option);
            QString pixmapName =
                    d_ptr->unique_name
                    (u"headersection"_qs, option, option->rect.size());
            pixmapName += QString::number(- int(header->position));
            pixmapName += QString::number(- int(header->orientation));
            if (headerV2)
                pixmapName += QString::number(- int(headerV2->isSectionDragTarget));

            QPixmap cache;
            if (!QPixmapCache::find (pixmapName, &cache))
            {
                cache = d_ptr->style_cache_pixmap (rect.size());
                cache.fill (Qt::transparent);

                // Draw header background
                QRect pixmapRect(0, 0, rect.width(), rect.height());
                QPainter cachePainter (&cache);
                cachePainter.fillRect (pixmapRect,
                                       CustomColors::header_gradient (&pixmapRect));

                // Draw top border
                cachePainter.setPen (CustomColors::inner_contrast_line());
                cachePainter.setBrush (Qt::NoBrush);
                cachePainter.drawLine (pixmapRect.topLeft(), pixmapRect.topRight());

                // Draw bottom border
                cachePainter.setPen (CustomColors::outline(option->palette));
                cachePainter.drawLine (pixmapRect.bottomLeft(), pixmapRect.bottomRight());


                // Draw borders for horizontal header
                if (header->orientation == Qt::Horizontal
                    && header->position != QStyleOptionHeader::End
                    && header->position != QStyleOptionHeader::OnlyOneSection)
                {
                    // Draw right border
                    cachePainter.setPen (CustomColors::header_bottom_border());
                    cachePainter.drawLine (pixmapRect.topRight(),
                                           pixmapRect.bottomRight() + QPoint(0, -1));

                    // Draw left border
                    cachePainter.setPen (CustomColors::inner_contrast_line());
                    cachePainter.drawLine (pixmapRect.topRight() + QPoint(-1, 0),
                                           pixmapRect.bottomRight() + QPoint(-1, -1));
                }
                // Draw borders for vertical header
                else if (header->orientation == Qt::Vertical)
                {
                    // Draw right border
                    cachePainter.setPen (CustomColors::outline(option->palette));
                    cachePainter.drawLine (pixmapRect.topRight(), pixmapRect.bottomRight());
                }
                cachePainter.end();

                QPixmapCache::insert (pixmapName, cache);
            }
            painter->drawPixmap (rect.topLeft(), cache);
        }
        painter->restore();
        break;
    default:
        QProxyStyle::drawControl (element,option,painter,widget);
        break;
    }

}


int CustomStyle::pixelMetric (PixelMetric metric,
                              const QStyleOption *option,
                              const QWidget *widget) const
{
    switch (metric) {
    case PM_ScrollBarExtent:
        return sb_metrics.width;
        break;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}
