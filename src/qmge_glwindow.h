#ifndef QMGE_GLWINDOW_H
#define QMGE_GLWINDOW_H

#include "qmge_includes.h"

namespace QMGE_Core
{


//A QMGE_GLWindow is created with an OpenGL Context.

class QMGE_GLWindow : public QWindow
{
    Q_OBJECT

public:

    QMGE_GLWindow(QSurfaceFormat contextSettings);
    ~QMGE_GLWindow();

signals:

public slots:


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

private:

    //is current window in fullscreen
    bool m_isFullScreen;
};
//class QMGE_GLWindow


}
//namespace QMGE_CORE


#endif // QMGE_GLWINDOW_H
