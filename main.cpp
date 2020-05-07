
#define GLIB_DISABLE_DEPRECATION_WARNINGS
#include <glib-object.h>

#include "gerberstencilgenerator.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    if (glib_check_version (2, 36, 0) != NULL) { g_type_init (); }

    gtk_init_check(nullptr,nullptr);
    gdk_init(nullptr,nullptr);
    gtk_init(nullptr, nullptr);

    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/res/appicon"));

    GerberStencilGenerator w;

    w.show();

    return a.exec();
}
