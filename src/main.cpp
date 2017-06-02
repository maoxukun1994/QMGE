#include "src/qmge_app.h"

int main(int argc,char ** argv)
{
    QApplication a(argc,argv);

    QScopedPointer<QMGE_Core::QMGE_App> app(new QMGE_Core::QMGE_App());
    app->createWindow(QSize(1280,729),false);
    //app->createWindow(QSize(1280,729),true);
    app->run();

    return a.exec();
}
