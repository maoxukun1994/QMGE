#ifndef QMGE_APP_H
#define QMGE_APP_H

#include "qmge_includes.h"
#include "qmge_glwindow.h"

namespace QMGE_Core
{

class QMGE_App
{

public:    

    QMGE_App(bool isMultiThread = false);

    void createWindow(int width,int height,bool isFullScreen = true);

private:

    QScopedPointer<QMGE_GLWindow> m_window;

    bool m_isMultiThread;
};
//class QMGE_App



}
//namespace QMGE_Core

#endif // QMGE_APP_H
