#ifndef PDF_DOCUMENT_H
#define PDF_DOCUMENT_H

#include <QFile>
#include <QMap>
#include <QObject>
#include <QPdfDocument>
#include <QUrl>

#include "conversion_factors.h"

struct mfc_config
{
    int position;

    QString gas;
    int flow_rate;

    double equiv_flow_rate;
};

class PDF_Document : public QObject
{
    Q_OBJECT
public:
    explicit PDF_Document(QObject *parent = nullptr, QWidget *widget = nullptr);
    ~PDF_Document();

    void extract_text(QChar chamber);

    void calculate_cf();

    // void update_cf(QMap<QString,double> cf_map);

    void export_to_csv();

    mfc_config get_sysConfig(int position);

    void set_chamber(const QChar &new_chamber);

signals:

public slots:
    void open(const QUrl &file_path);

private slots:
    void new_status(QPdfDocument::Status status);

private:
    QPdfDocument *m_document;
    Conversion_Factors *m_conversion_factors;
    QVector<mfc_config> m_sysConfig;
    QChar *m_chamber;

    QWidget *m_widget;
};



#endif // PDF_DOCUMENT_H
