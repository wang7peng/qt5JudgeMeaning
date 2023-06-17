#include <QApplication>
#include <QFile>
#include <QtWidgets/QDesktopWidget>

#include "home.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName(HOMENAME);
    a.setApplicationVersion("0.1");

    QFile file("/usr/local/etc/cssfiles/widgets.qss");
    file.open(QIODevice::ReadOnly);
    a.setStyleSheet(QString::fromLatin1(file.readAll()));

    // ------ ------ start ------ ------
    Home w;
    w.setWindowTitle(QCoreApplication::applicationName());

    int spaceLeft = (a.desktop()->width() - w.width()) / 2;
    int spaceTop = (a.desktop()->height() - w.height()) / 2;
    w.move(spaceLeft, spaceTop);

    w.show();

    return a.exec();
}
