#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "pdf_document.h"
#include "settingswindow.h"

#include <QFileDialog>
#include <QFontDatabase>
#include <QHeaderView>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QTableWidget>
#include <QUrl>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , stylesheets (new CustomStyleSheets)
{
    ui->setupUi(this);

    /* Setup System Configuration Table */
    ui->sysConfig_table->setColumnCount(4);
    ui->sysConfig_table->setRowCount(20);

    QStringList headers = {"MFC:", "Offset Value:", "Gas Name:", "Flow:"};
    ui->sysConfig_table->setHorizontalHeaderLabels(headers);
    ui->sysConfig_table->horizontalHeader()
                       ->setSectionResizeMode(QHeaderView::Stretch);

    /* Only display uppercase letters for chamber (A, B, C) */
    connect(ui->chamber_select, &QLineEdit::textEdited, this, &on_textEdited);

    /* Setup fonts and Style Sheets */
    setup_theme();

}


MainWindow::~MainWindow()
{
    delete ui;
}


/* Open file browser to select pdf
** Return: none
** Notes: The PDF should be a System Configuration Report
*/
void MainWindow::on_browse_sysConfig_path_button_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,
                                        "Open a file",
                                        "",
                                        "PDF files (*.pdf)");

    ui->sysConfig_path->clear();
    ui->sysConfig_path->insert(path);

    // qDebug() << path;
}


/* Export the N2 equivalency info to a csv
** Return: none
** Notes: A PDF should be selected in the Line Edit box above
*/
void MainWindow::on_export_csv_button_clicked()
{
    /* Set location to save csv file (same location as selected PDF) */
    QUrl file_path = QUrl::fromLocalFile(ui->sysConfig_path->text());
    // qDebug() << file_path.path();

    if(!file_path.isValid())
    {
        QString message;
        QTextStream out(&message);
        out << "Cannot save csv. Invalid file path to System Configuration Report";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        return;
    }

    if(!file_path.fileName().endsWith(".pdf"))
    {
        QString message;
        QTextStream out(&message);
        out << "The N2 offsets will be exported to the same location as the System Configuration Report."
            << Qt::endl << "Please select a valid Configuration Report.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::information(this, tr("Notice"), message, QMessageBox::Ok);
        return;
    }

    QString path = file_path.toLocalFile();
    path.remove(".pdf");
    path.append(".csv");

    /* Open csv file for writing */
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString message;
        QTextStream out(&message);
        out << "Failed to open csv for writing. Aborting export." << Qt::endl
            << file.errorString();
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::warning(this, tr("Warning"), message, QMessageBox::Ok);
        return;
    }

    /* Write table contents to csv */
    QTextStream file_out(&file);
    file_out << "MFC:,Offset Value:,Gas Names:,Flow:\n";
    for(int i=0; i<ui->sysConfig_table->rowCount(); ++i)
    {
        for(int j=0; j<ui->sysConfig_table->columnCount(); ++j)
        {
            file_out << ui->sysConfig_table->item(i, j)->data(Qt::DisplayRole).toString();

            if(j != ui->sysConfig_table->columnCount()-1)
            {
                file_out << ",";
            }
        }

        file_out << "\n";
    }

    file.close();

    QString message;
    QTextStream out(&message);
    out << "Successfully exported csv to " << path;
    //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
    //qCDebug(lcExample).noquote() << message;
    QMessageBox::information(this, tr("Info"), message, QMessageBox::Ok);
}


/* Extract the System Configuration from a PDF
** Return: none
*/
void MainWindow::on_read_sysConfig_button_clicked()
{
    if(ui->sysConfig_path->text().isEmpty())
    {
        QString message;
        QTextStream out(&message);
        out << "Select a system configuration file by clicking browse.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);
        return;
    }

    if(ui->chamber_select->text().isEmpty())
    {
        QString message;
        QTextStream out(&message);
        out << "Chamber not specified. Defaulting to Chamber 1.";
        //const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        //qCDebug(lcExample).noquote() << message;
        QMessageBox::information(nullptr, tr("Notice"), message, QMessageBox::Ok);
    }


    /* Select chamber number to extract from System Configuration */
    QString chamber_select = ui->chamber_select->text();
    QChar chamber = QChar('1'); // default
    if(!chamber_select.isEmpty())
    {
        chamber = QChar(chamber_select.at(0));
    }

    /* Open the System Configuration Report and extract information */
    PDF_Document doc;
    QUrl url = QUrl::fromLocalFile(ui->sysConfig_path->text());
    doc.set_chamber(chamber);
    doc.open(url);
    doc.calculate_cf();

    /* Store Configuration Report in a Table Widget */
    int pos[] = {1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 2, 20, 3, 4, 5, 6, 7, 8, 9}; // Same order as GBC MFC configuration page
    for(int i=0; i<20; ++i)
    {
        QTableWidgetItem *gas;
        QTableWidgetItem *offset;
        QTableWidgetItem *flow;

        mfc_config config = doc.get_sysConfig(pos[i]-1);
        if(config.gas == "N/A")
        {
            gas = new QTableWidgetItem("-");
            offset = new QTableWidgetItem("-");
            flow = new QTableWidgetItem("-");
        }
        else
        {
            gas = new QTableWidgetItem(config.gas);
            offset = new QTableWidgetItem(QString::number(config.equiv_flow_rate));
            flow = new QTableWidgetItem(QString::number(config.flow_rate)+" sccm");
        }

        QTableWidgetItem *pos = new QTableWidgetItem("MFC "+QString::number(config.position));

        ui->sysConfig_table->setItem(i, 0, pos);
        ui->sysConfig_table->setItem(i, 1, offset);
        ui->sysConfig_table->setItem(i, 2, gas);
        ui->sysConfig_table->setItem(i, 3, flow);
    }

    doc.deleteLater();
}


void MainWindow::on_settings_button_triggered()
{
    QWidget *w = new SettingsWindow();
    w->show();
    w->raise();
}


void MainWindow::on_textEdited(const QString &new_text)
{
    // Only display uppercase letters for chamber
    ui->chamber_select->setText(new_text.toUpper());
}


void MainWindow::setup_theme()
{
    /* Setup Fonts */
    QFontDatabase::addApplicationFont(":/fonts/AtkinsonHyperlegibleNext-Light.otf");
    QFontDatabase::addApplicationFont(":/fonts/AtkinsonHyperlegibleNext-Regular.otf");
    QFontDatabase::addApplicationFont(":/fonts/AtkinsonHyperlegibleNext-Medium.otf");
    QFontDatabase::addApplicationFont(":/fonts/AtkinsonHyperlegibleNext-SemiBold.otf");
    QFontDatabase::addApplicationFont(":/fonts/AtkinsonHyperlegibleNext-Bold.otf");
    QFontDatabase::addApplicationFont(":/fonts/AtkinsonHyperlegibleNext-ExtraBold.otf");

    ui->sysConfig_path->setFont(QFont("AtkinsonHyperlegible", 10));
    ui->chamber_select->setFont(QFont("AtkinsonHyperlegible", 10));
    ui->browse_sysConfig_path_button->setFont(QFont("AtkinsonHyperlegible", 10));
    ui->read_sysConfig_button->setFont(QFont("AtkinsonHyperlegible", 10));
    ui->export_csv_button->setFont(QFont("AtkinsonHyperlegible", 10));

    ui->sysConfig_table->setFont(QFont("AtkinsonHyperlegible", 10));
    ui->sysConfig_table->horizontalHeader()->setFont(QFont("AtkinsonHyperlegible", 11, QFont::Medium));


    /* Setup Style Sheets */
    ui->centralwidget->setStyleSheet (stylesheets->vertical_scrollbar());

    // Set treeview header size for scrollbar gutter
    table_stylesheet = stylesheets->table_scrollbar(
        ui->sysConfig_table->horizontalHeader()->sizeHint().height());

    ui->sysConfig_table->setStyleSheet(table_stylesheet
                                       % stylesheets->vertical_scrollbar_quirk());
}


void MainWindow::setVerticalScrollBarQuirk()
{
    QString update_scrollbar =
        table_stylesheet
        % stylesheets->vertical_scrollbar_quirk();

    ui->sysConfig_table->setStyleSheet (update_scrollbar);
}
