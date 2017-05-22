#include "qmge_app.h"


int main(int argc,char ** argv)
{
    QGuiApplication a(argc,argv);

    QScopedPointer<QMGE_Core::QMGE_App> app(new QMGE_Core::QMGE_App());
    app->createWindow(1280,720,false);
    //app->createWindow(0,0,true);
    app->run();

    return a.exec();
}
