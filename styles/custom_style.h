#ifndef STYLE_CUSTOM_H
#define STYLE_CUSTOM_H

#include <QAbstractScrollArea>
#include <QProxyStyle>
#include <QStyleOption>

#include "custom_metrics.h"

class CustomStylePrivate;
class CustomStyle : public QProxyStyle
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CustomStyle)

public:
    CustomStyle();
    ~CustomStyle();

    void polishScrollArea( QAbstractScrollArea* );

    void drawPrimitive (PrimitiveElement element,
                        const QStyleOption *option,
                        QPainter *painter,
                        const QWidget *widget) const override;

    void drawControl (ControlElement element,
                      const QStyleOption *option,
                      QPainter *painter,
                      const QWidget *widget) const override;

    int pixelMetric(PixelMetric metric,
                    const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;

private:
    ScrollBar_Metrics sb_metrics;

protected:
    CustomStyle(CustomStylePrivate &dd);
    CustomStylePrivate *d_ptr;
};

#endif // STYLE_CUSTOM_H
