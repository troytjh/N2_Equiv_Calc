#ifndef CONVERSION_FACTORS_H
#define CONVERSION_FACTORS_H

#include <QMap>
#include <QSettings>

class Conversion_Factors : public QObject
{
public:
    explicit Conversion_Factors(QObject *parent = nullptr);

    void set_map(QString key, double value);

    void update_map();

    void init_map();

    void reset_settings();

    QMap<QString, double> get_cf_map() const;

signals:

private:
    QSettings *m_settings;

    // Set default
    QMap<QString, double> cf_map;
};

#endif // CONVERSION_FACTORS_H
