#include "mainwindow.h"
#include "custom_colors.h"
#include "custom_style.h"

#include <QApplication>
#include <QUrl>
#include <QPixmap>
#include <QLabel>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("UCT");
    QCoreApplication::setOrganizationDomain("uct.com");
    QCoreApplication::setApplicationName("N2 Equivalency Calc");

    QApplication a(argc, argv);

    auto* style = new CustomStyle();
    a.setStyle(style);

    MainWindow w;
    w.show();
    return a.exec();
}
