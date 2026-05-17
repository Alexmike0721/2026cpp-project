#include "startmenu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StartMenu menu;
    menu.show();

    return app.exec();
}
