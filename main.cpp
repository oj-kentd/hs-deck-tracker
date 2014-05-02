#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if QT_VERSION >= 0x050100
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    a.setApplicationVersion(APP_VERSION);
    MainWindow w;
    w.show();

    return a.exec();
}
