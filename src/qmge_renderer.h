#ifndef QMGE_RENDERER_H
#define QMGE_RENDERER_H

#include "qmge_global.h"
#include "qmge_glwindow.h"
#include "qmge_gluniformmanager.h"

//tempory
#include "qmge_glbatch.h"
#include "qmge_glshaderprogram.h"
#include "qmge_glcamera.h"

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

    void readyToStop();

public slots:

    //opened for called by parent(QMGE_GLWindow)
    void execRender();

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
    QMatrix4x4 mMatrix;
    QScopedPointer<QMGE_GLBatch> batch;
    GLuint texId;
    QScopedPointer<QMGE_GLShaderProgram> program;
    QScopedPointer<QOpenGLTexture> texture;
    QScopedPointer<QMGE_GLCamera> camera;


};
//class QMGE_Renderer



}
//namespace QMGE_Core


#endif // QMGE_RENDERER_H
