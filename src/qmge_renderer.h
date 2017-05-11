#ifndef QMGE_RENDERER_H
#define QMGE_RENDERER_H

#include "qmge_includes.h"

namespace QMGE_Core
{

class QMGE_GLWindow;

class QMGE_Renderer : public QObject,protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:

    QMGE_Renderer(QSurfaceFormat contextSettings,QMGE_GLWindow * parent = nullptr);

    ~QMGE_Renderer();

protected:

    void init();

    void renderOnce();
    void renderLater();

    bool event(QEvent * event) Q_DECL_OVERRIDE;

signals:

    void stopExec();

public slots:

    //opened for called by parent(QMGE_GLWindow)
    void execRender();

private:

    //context setting for creating the OpenGL context
    QSurfaceFormat m_contextSettings;

    //OpenGL context pointer
    QSharedPointer<QOpenGLContext> m_context;

    //indicate whether opengl context is initialized
    volatile bool m_isInitialized;

    //indicate whether an update request is waiting
    volatile bool m_updatePending;

    //the window this renderer render against
    QMGE_GLWindow * m_renderWindow;

    //count fps
    int m_frames;

};
//class QMGE_Renderer



}
//namespace QMGE_Core


#endif // QMGE_RENDERER_H
