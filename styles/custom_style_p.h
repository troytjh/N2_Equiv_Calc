#ifndef STYLE_CUSTOM_P_H
#define STYLE_CUSTOM_P_H

#include <QApplication>
#include <QCommonStyle>
#include <QStringBuilder>
#include <QStyleOptionComplex>

// internal helper. Converts an integer value to a unique string token
template <typename T>
        struct HexString
{
    inline HexString(const T t)
        : val(t)
    {}

    inline void write(QChar *&dest) const
    {
        const char16_t hexChars[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        const char *c = reinterpret_cast<const char *>(&val);
        for (uint i = 0; i < sizeof(T); ++i)
        {
            *dest++ = hexChars[*c & 0xf];
            *dest++ = hexChars[(*c & 0xf0) >> 4];
            ++c;
        }
    }
    const T val;
};


// specialization to enable fast concatenating of our string tokens to a string
template <typename T>
        struct QConcatenable<HexString<T> >
{
    typedef HexString<T> type;
    enum { ExactSize = true };
    static int size (const HexString<T> &) { return sizeof(T) * 2; }
    static inline void appendTo (const HexString<T> &str, QChar *&out) { str.write(out); }
    typedef QString ConvertTo;
};


class CustomStylePrivate
{
public:
    CustomStylePrivate();

    enum {
        menuItemHMargin      =  3, // menu item hor text margin
        menuArrowHMargin     =  6, // menu arrow horizontal margin
        menuRightBorder      = 15, // right border on menus
        menuCheckMarkWidth   = 12  // checkmarks width on menus
    };

    QString unique_name(const QString &key, const QStyleOption *option, const QSize size)
    {
        const QStyleOptionComplex *complexOption = qstyleoption_cast<const QStyleOptionComplex *>(option);
        QString tmp = key % HexString<uint>(option->state)
                          % HexString<uint>(option->direction)
                          % HexString<uint>(complexOption ? uint(complexOption->activeSubControls) : 0u)
                          % HexString<quint64>(option->palette.cacheKey())
                          % HexString<uint>(size.width())
                          % HexString<uint>(size.height());

        return tmp;
    }

    inline QPixmap style_cache_pixmap (const QSize size)
    {
        const qreal pixelRatio = qApp->devicePixelRatio();
        QPixmap cachePixmap = QPixmap (size * pixelRatio);
        cachePixmap.setDevicePixelRatio(pixelRatio);
        return cachePixmap;
    }
};

#endif // STYLE_CUSTOM_P_H
