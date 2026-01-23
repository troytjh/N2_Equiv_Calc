#ifndef CUSTOMCOLORS_H
#define CUSTOMCOLORS_H

#include <QColor>
#include <QCommonStyle>
#include <QDebug>
#include <QLinearGradient>
#include <QPen>


class CustomColors
{

private:
    enum Direction {
        TopDown,
        FromLeft,
        BottomUp,
        FromRight
    };

public:
    CustomColors();

    static QColor merged_colors (const QColor &colorA,
                                 const QColor &colorB,
                                 int factor = 50);

    static QLinearGradient qt_fusion_gradient (const QRect rect,
                                               const QBrush &baseColor,
                                               Direction direction = TopDown);

    /*
     *  Color Palette
    */
    static QColor light_shade() {
        return QColor (255, 255, 255, 90);
    }
    static QColor dark_shade() {
        return QColor (0, 0, 0, 60);
    }

    static QColor top_shadow() {
        return QColor (0, 0, 0, 18);
    }

    static QColor inner_contrast_line() {
        return QColor (255, 255, 255, 30);
    }

    static QColor highlight(const QPalette &pal) {
        return pal.color(QPalette::Highlight);
    }

    static QColor highlight(void) {
        return QColor (61,174,233);
    }

    static QColor highlighted_text(const QPalette &pal) {
        return pal.color(QPalette::HighlightedText);
    }

    static QColor outline(const QPalette &pal) {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor (0, 0, 0, 160);
        return pal.window().color().darker(140);
    }

    static QColor outline(void) {
        return QColor (30, 34, 39).darker(140);
    }

    static QColor highlighted_outline(const QPalette &pal) {
        QColor highlighted_outline = highlight(pal).darker(125);
        if (highlighted_outline.value() > 160)
            highlighted_outline.setHsl(highlighted_outline.hue(),
                                       highlighted_outline.saturation(),
                                       160);
        return highlighted_outline;
    }

    static QColor highlighted_outline(void) {
        QColor highlighted_outline = highlight().darker(125);
        if (highlighted_outline.value() > 160)
            highlighted_outline.setHsl(highlighted_outline.hue(),
                                       highlighted_outline.saturation(),
                                       160);
        return highlighted_outline;
    }

    static QColor tab_frame_color(const QPalette &pal) {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor (255, 255, 255, 8);
        return background_color().lighter(104);
    }

    static QColor background_color() {
        return QColor (40,44,52);
    }


    /*
     *  Primitive Colors
    */
    static int button_brightness() {
        return 125;
    }

    static QColor button_color() {
        return background_color().lighter(CustomColors::button_brightness());
    }

    static QColor frame_color() {
        return background_color().darker(145);
    }

    static QColor frame_font_color() {
        // Color set based on frame_color
        return QColor (233,235,244);
    }

    static QPen inner_frame_border() {
        QPen inner_border(QColor (255,255,255,2));

        inner_border.setWidth(2);
        inner_border.setCosmetic(false);
        return inner_border;
    }

    static QColor outer_frame_border_color() {
        return CustomColors::merged_colors
                (CustomColors::highlight(),
                 CustomColors::frame_color(),
                 90);
    }

    static QPen outer_frame_border() {
        QPen outer_border (CustomColors::outer_frame_border_color());

        outer_border.setWidth(1);
        outer_border.setCosmetic(false);
        return outer_border;
    }

    static QColor header_gradientStart() {
        return CustomColors::background_color().lighter(130);
    }

    static QColor header_gradientStop() {
        return CustomColors::background_color().lighter(123);
    }

    static QColor header_midColor1() {
        return CustomColors::merged_colors
                (CustomColors::header_gradientStart(),
                 CustomColors::header_gradientStop(),
                 60);
    }

    static QColor header_midColor2() {
        return CustomColors::merged_colors
                (CustomColors::header_gradientStart(),
                 CustomColors::header_gradientStop(),
                 40);
    }

    static QLinearGradient header_gradient(QRect *rect)
    {
        QLinearGradient gradient(rect->topLeft(), rect->bottomLeft());

        gradient.setColorAt(0, header_gradientStart());
        gradient.setColorAt(0.5, header_midColor1());
        gradient.setColorAt(0.501, header_midColor2());
        gradient.setColorAt(0.92, header_gradientStop());
        gradient.setColorAt(1, header_gradientStop().darker(104));

        return gradient;
    }

    static QColor header_bottom_border() {
        return QColor (0, 0, 0, 40);
    }


    /*
     * ScrollBar Colors
    */
    static QColor scrollbar_handle_bgColor() {
        return CustomColors::merged_colors (CustomColors::highlight(),
                                            CustomColors::background_color(),
                                            40);
    }

    static QColor scrollbar_handle_outline() {
        return CustomColors::merged_colors (CustomColors::highlight(),
                                            CustomColors::background_color(),
                                            80);
    }

};

#endif // CUSTOMCOLORS_H
