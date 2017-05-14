#ifndef QMGE_GLWINDOW_H
#define QMGE_GLWINDOW_H

#include "qmge_global.h"

namespace QMGE_Core
{


//QMGE_GLWindow used as a rendering surface
//in charge of event handling

class QMGE_GLWindow : public QWindow
{
    Q_OBJECT

public:

    QMGE_GLWindow(QSurfaceFormat contextSettings);
    ~QMGE_GLWindow();

signals:

    void startRenderThread();
    void stopRenderThread();

public slots:

    void safeClose();

protected:

    //overrided virtual functions from QWindow
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent * e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent * e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent * e) Q_DECL_OVERRIDE;
};
//class QMGE_GLWindow


}
//namespace QMGE_CORE


#endif // QMGE_GLWINDOW_H
