#ifndef QMGE_RENDERER_H
#define QMGE_RENDERER_H

#include "qmge_global.h"
#include "qmge_glwindow.h"
#include "qmge_gluniformmanager.h"

//tempory
#include "qmge_fpscameracontroller.h"
#include "qmge_glcamera.h"
#include "massiveground/chunkmanager.h"
#include "massiveground/chunkmanagerts.h"
#include "massiveground/chunkmanagerpn.h"

namespace QMGE_Core
{

class QMGE_Renderer : public QObject,protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:

    QMGE_Renderer(QSurfaceFormat contextSettings,QMGE_GLWindow * parent);

    ~QMGE_Renderer();

    //for other resources initialization
    virtual void postInit();
    //for user implemented render
    virtual void render();

protected:

    void init();

    void renderOnce();
    void renderLater();

    bool event(QEvent * event) Q_DECL_OVERRIDE;

signals:

    void initialized();
    void readyToStop();

public slots:

    //opened for called by parent(QMGE_GLWindow)
    void execRender();

    //used for changing glviewport when renderwindow changed size.
    void resizeGL(int w,int h);

    //key and mouse
    void windowKeyChanged(int key,bool pressed);
    void windowMouseMoved(int deltax,int deltay);

    //do cleaning up jobs
    void cleanUp();

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

    //temporary
    //count fps
    int m_frames;
    QSharedPointer<QMGE_GLCamera> camera;
    QSharedPointer<QMGE_FPSCameraController> camcontrol;

    //QScopedPointer<ChunkManager> m_chunkManager;
    QScopedPointer<ChunkManagerTS> m_chunkManager;
    //QScopedPointer<ChunkManagerPN> m_chunkManager;

    bool lineMode;

    //for chunkmanagerts uniform
    int * split;
    bool is1Pressed;
    bool is2Pressed;
};
//class QMGE_Renderer



}
//namespace QMGE_Core


#endif // QMGE_RENDERER_H
