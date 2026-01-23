#include "pdf_document.h"

#include <QMessageBox>
#include <QMetaEnum>

PDF_Document::PDF_Document(QObject *parent)
    : QObject{parent}
    , m_document(new QPdfDocument(this))
    , m_conversion_factors(new Conversion_Factors(this))
    , m_sysConfig(QVector<mfc_config>(20))
    , m_chamber(new QChar())
{
    /* Initialize Empty System Configuration */
    for(int i=0; i<20; ++i)
    {
        m_sysConfig[i].position = i+1;
    }

    connect(m_document, &QPdfDocument::statusChanged, this, &PDF_Document::new_status);
}


PDF_Document::~PDF_Document()
{
    m_document->close();
}


QDebug operator<<(QDebug debug, const mfc_config config)
{
    QDebugStateSaver saver(debug);
    debug.nospace().noquote()
        << "MFC: " << config.position << " Flow Rate: "
        << config.flow_rate << "sccm" << "  Gas Type: "
        << config.gas << Qt::endl;

    return debug;
}


/* Find MFC Configuration
** Return: none
*/
void PDF_Document::extract_text(QChar chamber)
{
    if(m_document->status() != QPdfDocument::Status::Ready)
    {
        QString message;
        QTextStream out(&message);
        out << "Can not extract text. PDF file is not open.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);
        return;
    }

    /* Extract all text from PDF */
    QString pdf_text;
    QStringList sysConfig_text;
    for(int i=0; i < m_document->pageCount(); ++i)
    {
        pdf_text.append(m_document->getAllText(i).text());
    }

    /* Reduce text to specific chamber configuration */
    QString valid_chambers = "123ABC";
    if(chamber.isNull()
        || (valid_chambers.indexOf(chamber) == -1))
    {
        QString message;
        QTextStream out(&message);
        out << "Chamber not specified or not supported. Defaulting to Chamber 1.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);

        chamber = QChar('1');
    }


    qsizetype start = pdf_text.lastIndexOf("CHAMBER "+QString(chamber)+"/Gas Pallet");
    qsizetype end = pdf_text.lastIndexOf("\r\nCHAMBER "+QString(chamber)+"/GP Information");
    if((start != -1)
        && (end != -1)
        && (end > start))
    {
        qsizetype n = end - start;

        pdf_text.slice(start, n);
        sysConfig_text = pdf_text.split("\r\n");
    }
    else
    {
        QString message;
        QTextStream out(&message);
        out << "Failed to extract MFC config from System Configuration Report." << Qt::endl
            << "System Configuration Report may not be formatted correctly.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::critical(nullptr, tr("Error"), message, QMessageBox::Ok);

        return;
    }

    //qDebug() << sysConfig_text;

    /* Find MFC configuration */
    bool flag;
    int pos = 1;
    int len = sysConfig_text.size();
    for(int i=0; i<len; ++i)
    {
        // Skip text with no configuration info
        if(!sysConfig_text[i].contains("Line"))
        {
            continue;
        }

        sysConfig_text[i] = sysConfig_text[i].trimmed();
        sysConfig_text[i] = sysConfig_text[i].remove(".");
        sysConfig_text[i] = sysConfig_text[i].remove(" (IGI)");
        sysConfig_text[i] = sysConfig_text[i].remove(" (Heated)");

        // Identify MFC Position
        if(sysConfig_text[i].split(" ").size() > 2)
        {
            if(sysConfig_text[i].split(" ").at(0) != "Line")
            {
                continue;
            }

            pos = sysConfig_text[i].split(" ").at(1).toInt(&flag);

            if(!flag)
            {
                QString message;
                QTextStream out(&message);
                out << "Failed to convert MFC position string to number."
                    << Qt::endl << sysConfig_text[i];
                //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
                //qCDebug(lcExample).noquote() << message;
                QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);

                continue;
            }
            else if((pos < 1) || (pos > 20))
            {
                QString message;
                QTextStream out(&message);
                out << "MFC Position is out of range."
                    << Qt::endl << sysConfig_text[i];
                //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
                //qCDebug(lcExample).noquote() << message;
                QMessageBox::warning(nullptr, tr("Warning"), message, QMessageBox::Ok);

                continue;
            }
        }
        else
        {
            QString message;
            QTextStream out(&message);
            out << "Failed to extract MFC config from System Configuration Report. "
                << "Unable to identify MFC position."
                << Qt::endl << sysConfig_text[i];
            //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
            //qCDebug(lcExample).noquote() << message;
            QMessageBox::warning(nullptr, tr("Warning"), message, QMessageBox::Ok);
        }

        // Identify MFC Gas Type and Flow Rate
        if(sysConfig_text[i].contains("Gas Type"))
        {
            QString gas = sysConfig_text[i].split(':').back().toUpper();
            if(gas.split(" ").back().contains("NSR"))
            {
                gas = "N/A";
            }

            m_sysConfig[pos-1].gas = gas;
        }
        else if(sysConfig_text[i].contains("MFC Size"))
        {
            m_sysConfig[pos-1].flow_rate = sysConfig_text[i].split(':').back().toInt(&flag);
            if(!flag)
            {
                QString message;
                QTextStream out(&message);
                out << "Failed to convert flow rate string to number." << Qt::endl
                    << sysConfig_text[i];
                //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
                //qCDebug(lcExample).noquote() << message;
                QMessageBox::warning(nullptr, tr("Warning"), message, QMessageBox::Ok);
            }
        }
    }

    /* Identify N/A Sticks */
    for(int i=0; i<20; ++i)
    {
        if(m_sysConfig[i].gas.isEmpty())
        {
            m_sysConfig[i].gas = "N/A";
        }
    }

    //qDebug() << m_sysConfig;
}


/* Calculate N2 Equivalency Offsets for Configuration
** Return: none
*/
void PDF_Document::calculate_cf()
{
    if(m_document->status() != QPdfDocument::Status::Ready)
    {
        QString message;
        QTextStream out(&message);
        out << "Failed to calculate N2 Equivalant Flow. PDF is not open.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);

        return;
    }

    QMap<QString, double> cf_map = m_conversion_factors->get_cf_map();

    double cf = 1;
    int flow = 0;
    QString gas = "";

    bool isZeroFlow = false;
    for(int i=0; i<20; ++i)
    {
        if(m_sysConfig[i].gas == "N/A")
        {
            continue;
        }

        flow = m_sysConfig[i].flow_rate;
        if(!flow)
        {
            if(!isZeroFlow)
            {
                QString message;
                QTextStream out(&message);
                out << "Failed to calculate N2 Equivalant Flow." << Qt::endl
                    << "Flow Rate can not be zero. ("
                    << flow << " sccm)";
                //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
                //qCDebug(lcExample).noquote() << message;
                QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);

                isZeroFlow = true;
            }
            continue;
        }

        gas = m_sysConfig[i].gas;
        if(!cf_map.contains(gas))
        {
            QString message;
            QTextStream out(&message);
            out << "Failed to find conversion factor for this gas. "
                << gas;
            //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
            //qCDebug(lcExample).noquote() << message;
            QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);
            continue;
        }

        cf = cf_map[gas];
        m_sysConfig[i].equiv_flow_rate = flow / cf;
        // qDebug()<< m_sysConfig[i].position << ": " << m_sysConfig[i].equiv_flow_rate;
    }
}


// void PDF_Document::update_cf(QMap<QString, double> cf_map)
// {
//     for (auto [key, value] : cf_map.asKeyValueRange())
//     {
//         m_conversion_factors->set_map(key, value);
//     }

// }


/* Export N2 Equivalency Offsets to CSV
** Return: none
** Notes:
*/
void PDF_Document::export_to_csv()
{
    QFile file("C:/Users/Troy.Hoover/Documents/Projects/Schematic_Conversion_Factor/out.csv");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString message;
        QTextStream out(&message);
        out << "Failed to open csv for writing. Aborting export." << Qt::endl
            << file.errorString();
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::critical(nullptr, tr("Error"), message, QMessageBox::Ok);

        return;
    }

    int pos[] = {1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 2, 20, 3, 4, 5, 6, 7, 8, 9};

    QTextStream out(&file);
    out << "MFC:,Offset Value:,Gas Names:,Flow:\n";
    for(int i : pos)
    {
        if(m_sysConfig[i-1].gas == "N/A")
        {
            out << "MFC " << QString::number(m_sysConfig[i-1].position)
                << ",-,-,-\n";

            continue;
        }

        out << "MFC " << QString::number(m_sysConfig[i-1].position) << ","
            << QString::number(m_sysConfig[i-1].equiv_flow_rate) << ","
            << m_sysConfig[i-1].gas << ","
            << QString::number(m_sysConfig[i-1].flow_rate) << " sccm\n";
    }

    file.close();
}


/* Getter function to access extracted System Configuration
** Parameters:
**     position - the MFC location in the config
** Return: Struct containing information on 1 MFC location
*/
mfc_config PDF_Document::get_sysConfig(int position)
{
    if(!(m_sysConfig.size() > position))
    {
        return mfc_config{};
    }

    mfc_config conf = m_sysConfig[position];
    return conf;
}


/* Open a PDF File
** Return: none
** Notes: The PDF should be a System Configuration Report
*/
void PDF_Document::open(const QUrl &file_path)
{
    if(!file_path.isLocalFile())
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot open PDF. Not a file path.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(nullptr, tr("Warning"), message, QMessageBox::Ok);
        return;
    }

    if(!file_path.isValid())
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot open PDF. Invalid file path";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(nullptr, tr("Warning"), message, QMessageBox::Ok);
        return;
    }

    if(!file_path.fileName().endsWith(".pdf"))
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot open PDF. Not a PDF File";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(nullptr, tr("Warning"), message, QMessageBox::Ok);
        return;
    }

    m_document->load(file_path.toLocalFile());
}


/* Check the current state after attempting to load a PDF
** Parameters:
**     status - the current state of the PDF document
*/
void PDF_Document::new_status(QPdfDocument::Status status)
{
    QPdfDocument *doc = qobject_cast<QPdfDocument*>(sender());

    switch(status)
    {
        case QPdfDocument::Status::Ready:
        {
            extract_text(*m_chamber);
            break;
        }
        case QPdfDocument::Status::Error:
        {
            QString error = QMetaEnum::fromType<QPdfDocument::Error>()
                                .valueToKey(int(doc->error()));
            QString message;
            QTextStream out(&message);
            out << "Something went wrong while opening PDF file"
                << Qt::endl << error;
            //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
            //qCDebug(lcExample).noquote() << message;
            QMessageBox::critical(nullptr, tr("Error"), message, QMessageBox::Ok);
            break;
        }
        default:
        {
            break;
        }
    }

}


void PDF_Document::set_chamber(const QChar &new_chamber)
{
    m_chamber = new QChar(new_chamber);
}
