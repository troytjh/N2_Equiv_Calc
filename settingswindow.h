#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMap>
#include <QWidget>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    QMap<QString, double> read_excel_cf();

private slots:
    void on_browse_cf_path_button_clicked();

    void on_update_cf_button_clicked();

    void on_reset_cf_button_clicked();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
