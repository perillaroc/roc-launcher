#include "launcher_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    LauncherWidget w;
//    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return app.exec();
}
