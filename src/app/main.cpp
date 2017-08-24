#include "launcher_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LauncherWidget w;
//    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
