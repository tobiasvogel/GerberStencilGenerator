#include "gerbv.h"
#include "gerberstencilgenerator.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GerberStencilGenerator w;

    w.show();

    return a.exec();
}
