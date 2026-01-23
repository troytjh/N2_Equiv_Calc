#include "custom_colors.h"

CustomColors::CustomColors()
{
}


/* Merge two colors */
QColor CustomColors::merged_colors (const QColor &colorA,
                                    const QColor &colorB,
                                    int factor)
{
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed ((tmp.red() * factor)
                 / maxFactor + (colorB.red() * (maxFactor - factor))
                 / maxFactor);
    tmp.setGreen ((tmp.green() * factor)
                   / maxFactor + (colorB.green() * (maxFactor - factor))
                   / maxFactor);
    tmp.setBlue ((tmp.blue() * factor)
                  / maxFactor + (colorB.blue() * (maxFactor - factor))
                  / maxFactor);
    return tmp;
}


/* The default button and handle gradient */
QLinearGradient CustomColors::qt_fusion_gradient (const QRect rect,
                                                  const QBrush &baseColor,
                                                  Direction direction)
{
    int x = rect.center().x();
    int y = rect.center().y();
    QLinearGradient gradient;
    switch (direction) {
    case FromLeft:
        gradient = QLinearGradient (rect.left(), y, rect.right(), y);
        break;
    case FromRight:
        gradient = QLinearGradient (rect.right(), y, rect.left(), y);
        break;
    case BottomUp:
        gradient = QLinearGradient (x, rect.bottom(), x, rect.top());
        break;
    case TopDown:
    default:
        gradient = QLinearGradient (x, rect.top(), x, rect.bottom());
        break;
    }
    if (baseColor.gradient())
        gradient.setStops(baseColor.gradient()->stops());
    else {
        QColor gradientStartColor = baseColor.color().lighter(124);
        QColor gradientStopColor = baseColor.color().lighter(102);
        gradient.setColorAt(0, gradientStartColor);
        gradient.setColorAt(1, gradientStopColor);
        //          Uncomment for adding shiny shading
        //            QColor midColor1 = merged_colors(gradientStartColor, gradientStopColor, 55);
        //            QColor midColor2 = merged_colors(gradientStartColor, gradientStopColor, 45);
        //            gradient.setColorAt(0.5, midColor1);
        //            gradient.setColorAt(0.501, midColor2);
    }
    return gradient;
}
