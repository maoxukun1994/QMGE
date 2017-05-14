#include "qmge_app.h"


int main(int argc,char ** argv)
{
    QGuiApplication a(argc,argv);

    QScopedPointer<QMGE_Core::QMGE_App> app(new QMGE_Core::QMGE_App());
    app->createWindow(640,480,false);
    app->run();

    return a.exec();
}
