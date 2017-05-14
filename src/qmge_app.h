#ifndef QMGE_APP_H
#define QMGE_APP_H

#include "qmge_global.h"
#include "qmge_glwindow.h"
#include "qmge_renderer.h"

namespace QMGE_Core
{

class QMGE_App : public QObject
{
    Q_OBJECT

public:    

    QMGE_App();
    ~QMGE_App();

    //create window,along with renderer.
    void createWindow(int width,int height,bool isFullScreen = true);

    void run();

private:

    QScopedPointer<QThread> m_renderThread;
    QScopedPointer<QMGE_GLWindow> m_window;
    QScopedPointer<QMGE_Renderer> m_renderer;

};
//class QMGE_App



}
//namespace QMGE_Core

#endif // QMGE_APP_H
