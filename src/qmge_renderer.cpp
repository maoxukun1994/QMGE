#include "qmge_renderer.h"

namespace QMGE_Core
{

QMGE_Renderer::QMGE_Renderer(QSurfaceFormat contextSettings,QMGE_GLWindow * parent):QObject(),QOpenGLExtraFunctions()
{
    m_isInitialized = false;
    m_updatePending = false;

    m_contextSettings = contextSettings;

    if(parent == nullptr)
    {
        qFatal("Can not create a render without a valid QMGE_GLWindow.");
    }
    else
    {
        m_renderWindow = parent;
    }

    m_frames = 0;
}


QMGE_Renderer::~QMGE_Renderer()
{
}

void QMGE_Renderer::postInit()
{
    camera.reset(new QMGE_GLCamera());
    camera->setPosition(0,0,64);
    camera->setPitch(-45.0f);
    camera->setYaw(45.0f);
    camera->setPerspective(75.0f,1.788f,0.1f,1000.0f);
    camcontrol.reset(new QMGE_FPSCameraController(camera));

    QMGE_GLUniformManager::getInstance()->registerUniform("split",int(640),split);
    m_chunkManager.reset(new ChunkManagerTS());
    m_chunkManager->loadMap(":/massiveground/textures/hm1.jpg",":/massiveground/textures/hm1_norm.jpg");

    //m_chunkManager.reset(new ChunkManagerPN());
    //m_chunkManager->load();

    //m_chunkManager.reset(new ChunkManager());
    //m_chunkManager->loadMap(":/massiveground/textures/hm1.jpg");

    lineMode = false;
    is1Pressed = false;
    is2Pressed = false;

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(0.1f,0.2f,0.1f,1.0f);
}


//for user implemented render
void QMGE_Renderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if(is1Pressed)
    {
        (*split) += 10;
        if(*split > m_renderWindow->width()) *split = m_renderWindow->width();
    }
    if(is2Pressed)
    {
        (*split) -= 10;
        if(*split < 0) *split = 0;
    }
    camcontrol->updateCam(0.016f);
    m_chunkManager->move(camera->getTransform().position);
}

void QMGE_Renderer::init()
{
    if(m_isInitialized)
    {
        return;
    }

    //create an QOpenGLContext Object
    if(m_context.isNull())
    {
        m_context.reset(new QOpenGLContext());
    }
    //set requested format
    m_context->setFormat(m_contextSettings);
    //and create & check
    if(m_context->create() && m_context->isValid())
    {
        //initialize functions
        qDebug()<<"Requested context:";
        qDebug()<<m_contextSettings.renderableType();
        qDebug()<<"OpenGL"<<m_contextSettings.version()<<m_contextSettings.profile();
        qDebug()<<"Got context:";
        qDebug()<<m_context->format().renderableType();
        qDebug()<<"OpenGL"<<m_context->format().version()<<m_context->format().profile();
    }
    else
    {
        qFatal("Failed to create OpenGL(ES) context");
    }

    if(!m_context->makeCurrent(m_renderWindow))
    {
        qFatal("Couldn't make context current against render window.");
    }

    //initialize OpenGL functions for the context
    initializeOpenGLFunctions();

    m_isInitialized = true;

    //prepare the render scene .etc
    //...
    postInit();

    //inform the window that renderer is ready
    emit initialized();
}

void QMGE_Renderer::renderOnce()
{
    m_frames++;
    if(m_renderWindow->isExposed())
    {
        //first make current
        m_context->makeCurrent(m_renderWindow);

        //do actual rendering
        render();

        //swap buffer
        m_context->swapBuffers(m_renderWindow);
        //post next request
        renderLater();
    }
    else
    {
        //when window not visible or available,not continue post updateRequest
        m_context->doneCurrent();
    }
}

void QMGE_Renderer::renderLater()
{
    if (!m_updatePending)
    {
        m_updatePending = true;
        QCoreApplication::postEvent(this,new QEvent(QEvent::UpdateRequest),Qt::EventPriority::HighEventPriority);
    }
}

bool QMGE_Renderer::event(QEvent * event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        m_updatePending = false;
        renderOnce();
        return true;
    default:
        return QObject::event(event);
    }
}

void QMGE_Renderer::execRender()
{
    if(!m_isInitialized)
    {
        init();
    }
    renderLater();
}

//used for changing glviewport when renderwindow changed size.
void QMGE_Renderer::resizeGL(int w, int h)
{
    if(m_isInitialized)
    {
        glViewport(0,0,w,h);
    }
}

void QMGE_Renderer::windowKeyChanged(int key,bool pressed)
{
    //register key change event
    switch(key)
    {
    case Qt::Key_W:
        camcontrol->move(FORWARD,pressed);
        break;
    case Qt::Key_S:
        camcontrol->move(BACK,pressed);
        break;
    case Qt::Key_A:
        camcontrol->move(LEFT,pressed);
        break;
    case Qt::Key_D:
        camcontrol->move(RIGHT,pressed);
        break;
    case Qt::Key_Q:
        if(pressed)
        {
            camcontrol->rotateTo(camera->getYaw(),camera->getPitch(),-60.0f);
        }
        else
        {
            camcontrol->rotateTo(camera->getYaw(),camera->getPitch(),0.0f);
        }
        break;
    case Qt::Key_E:
        if(pressed)
        {
            camcontrol->rotateTo(camera->getYaw(),camera->getPitch(),60.0f);
        }
        else
        {
            camcontrol->rotateTo(camera->getYaw(),camera->getPitch(),0.0f);
        }
        break;
    case Qt::Key_Space:
        camcontrol->move(UP,pressed);
        break;
    case Qt::Key_Control:
        camcontrol->move(DOWN,pressed);
        break;
    case Qt::Key_Shift:
        camcontrol->move(BOOST,pressed);
        break;
    case Qt::Key_1:
        is1Pressed = pressed;
        break;
    case Qt::Key_2:
        is2Pressed = pressed;
        break;
#ifdef OPENGL_DESKTOP_4_4
    case Qt::Key_F:
        if(pressed)
        {
            //temp toggle wire frame
            if(key == Qt::Key_F && pressed)
            {
                auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
                if(lineMode)
                {
                    f->glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                }
                else
                {
                    f->glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                }
                lineMode = !lineMode;
            }
        }
#endif
    default:
        break;
    }
}

void QMGE_Renderer::windowMouseMoved(int deltax,int deltay)
{
    //register mouse move event
    camcontrol->rotate(-(float)deltay * 0.2,-(float)deltax * 0.2,0);
}

void QMGE_Renderer::cleanUp()
{
    //do clean up jobs
    //...
    //...
    camcontrol.reset();
    camera.reset();
    m_chunkManager.reset();

    //delete openGL context
    //need to put behind all opengl-related resource cleaning-up
    if(!m_context.isNull())
    {
        m_context->doneCurrent();
        m_context.reset();
    }
    //last emit readyToStop signal
    emit readyToStop();
}

}
//namespace QMGE_Core
