#include "conversion_factors.h"

Conversion_Factors::Conversion_Factors(QObject *parent)
    : QObject{parent}
{
    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "UCT", "N2_Equiv_Calc");
    qDebug() << m_settings->fileName();

    init_map();

    update_map();
}


void Conversion_Factors::set_map(QString key, double value)
{
    m_settings->beginGroup("Conversion_Factors");
    m_settings->setValue(key.toUpper(), value);
    m_settings->endGroup();
}


void Conversion_Factors::update_map()
{
    m_settings->beginGroup("Conversion_Factors");
    QStringList keys = m_settings->childKeys();
    foreach(QString key, keys)
    {
        cf_map[key] = m_settings->value(key).toDouble();
    }
    m_settings->endGroup();
}


void Conversion_Factors::init_map()
{
    cf_map.clear();

    cf_map =
        {
            {"AR", 1.412},
            {"BCL3", 0.455},
            {"C4F6-Q", 0.193},
            {"C4F8", 0.172},
            {"CF4", 0.432},
            {"CH2F2", 0.62},
            {"CH3F", 0.727},
            {"CH4", 0.767},
            {"CHF3", 0.517},
            {"CL2", 0.84},
            {"COS", 0.706},
            {"H2", 0.977},
            {"HBR", 1.026},
            {"He", 1.388},
            {"HE", 1.388},
            {"NF3", 0.499},
            {"O2", 0.98},
            {"SF6", 0.275},
            {"SICL4", 0.317},
            {"SO2", 0.685},
            {"CO", 0.996},
            {"N2", 1.0}
        };
}


void Conversion_Factors::reset_settings()
{
    cf_map.clear();

    cf_map =
        {
            {"AR", 1.412},
            {"BCL3", 0.455},
            {"C4F6-Q", 0.193},
            {"C4F8", 0.172},
            {"CF4", 0.432},
            {"CH2F2", 0.62},
            {"CH3F", 0.727},
            {"CH4", 0.767},
            {"CHF3", 0.517},
            {"CL2", 0.84},
            {"COS", 0.706},
            {"H2", 0.977},
            {"HBR", 1.026},
            {"He", 1.388},
            {"HE", 1.388},
            {"NF3", 0.499},
            {"O2", 0.98},
            {"SF6", 0.275},
            {"SICL4", 0.317},
            {"SO2", 0.685},
            {"CO", 0.996},
            {"N2", 1.0}
        };

    m_settings->clear();
    for(auto [key, value] : cf_map.asKeyValueRange())
    {
        set_map(key, value);
    }
}


QMap<QString, double> Conversion_Factors::get_cf_map() const
{
    return cf_map;
}



