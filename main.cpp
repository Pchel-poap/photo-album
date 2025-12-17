#include "ui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем информацию о приложении
    QCoreApplication::setApplicationName("PhotoAlbum");
    QCoreApplication::setOrganizationName("PhotoAlbumApp");
    QCoreApplication::setApplicationVersion("1.0");

    MainWindow w;
    w.show();

    return a.exec();
}
