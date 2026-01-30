#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QScopeGuard>

#include "conversion_factors.h"
#include "settingswindow.h"
#include "ui_settingswindow.h"


SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}


/* Use SQL to read N2 Conversion Factors from excel
** Return:
*/
QMap<QString, double> SettingsWindow::read_excel_cf()
{
    QMap<QString, double> cf_map;

    /* Find Excel File */
    QUrl file_path = QUrl::fromLocalFile(ui->cf_path->text());
    if(!file_path.isLocalFile())
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot open Excel File. Not a file path.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        return QMap<QString, double>{};
    }

    if(!file_path.isValid())
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot open Excel File. Invalid file path";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        return QMap<QString, double>{};
    }

    if(!file_path.fileName().endsWith(".xlsx"))
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot open Excel File. Not a Excel File. (file must end with .xlsx)";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        return QMap<QString, double>{};
    }

    // Close connection after db leaves scope
    auto cleanup = qScopeGuard([] { QSqlDatabase::removeDatabase("xlsx_connection"); });

    /* Open Excel File */
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ="
                       + file_path.toLocalFile());

    if(!db.open())
    {
        QString message;
        QTextStream out(&message);
        out << "Failed to open excel sheet."
            << Qt::endl << db.lastError().text();

        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);

        db.close();

        return QMap<QString,double>{};
    }

    /* Read Excel File */
    QSqlQuery query("select * from [" + QString("Sheet1") + "$]", db);
    while (query.next())
    {
        QString gas = query.value(0).toString();
        //qDebug() << gas;

        double cf = query.value(1).toDouble();
        //qDebug() << cf;

        if(gas.isEmpty())
        {
            break;
        }

        cf_map[gas] = cf;
    }

    query.clear();
    db.close();

    return cf_map;
}


/* Open file browser to select a excel sheet
** Return: none
*/
void SettingsWindow::on_browse_cf_path_button_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,
                                        "Open a file",
                                        "",
                                        "Excel files (*.xlsx)");

    ui->cf_path->clear();
    ui->cf_path->insert(path);
}


/* Update N2 conversion factors
** Return: none
*/
void SettingsWindow::on_update_cf_button_clicked()
{
    QMap<QString,double> cf_map;
    cf_map = read_excel_cf();

    if(cf_map.isEmpty())
    {
        QString message;
        QTextStream out(&message);
        out << "Failed to update conversion factors.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        return;
    }

    // Update Settings
    Conversion_Factors cf;
    for (auto [key, value] : cf_map.asKeyValueRange())
    {
        cf.set_map(key, value);
    }

    QString message;
    QTextStream out(&message);
    out << "Conversion Factors have been updated.";
    //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
    //qCDebug(lcExample).noquote() << message;
    QMessageBox::information(this, tr("Info"), message, QMessageBox::Ok);
}


/* Set N2 conversion factors back to default values
** Return: none
*/
void SettingsWindow::on_reset_cf_button_clicked()
{
    // Reset Settings
    Conversion_Factors cf;
    cf.reset_settings();

    QString message;
    QTextStream out(&message);
    out << "Conversion Factors have been reset.";
    //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
    //qCDebug(lcExample).noquote() << message;
    QMessageBox::information(this, tr("Info"), message, QMessageBox::Ok);
}

