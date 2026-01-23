#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "custom_stylesheets.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browse_sysConfig_path_button_clicked();

    void on_export_csv_button_clicked();

    void on_read_sysConfig_button_clicked();

    void on_settings_button_triggered();

    void on_textEdited(const QString &new_text);

private:
    Ui::MainWindow *ui;

    CustomStyleSheets *stylesheets;
    QString table_stylesheet;

    void setup_theme();

    void setVerticalScrollBarQuirk();
};
#endif // MAINWINDOW_H
