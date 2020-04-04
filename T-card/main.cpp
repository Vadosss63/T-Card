#include "mainwindow.h"
#include <QTextCodec>

#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef WIN32
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication::setOrganizationName("T-Card");
    QApplication::setOrganizationDomain("t-box.fm");
    QApplication::setApplicationName("T-Card");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
