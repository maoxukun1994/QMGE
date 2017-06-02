#include "qmge_app.h"

namespace QMGE_Core
{

QMGE_App::QMGE_App() : QObject()
{
}


QMGE_App::~QMGE_App()
{
}

QSharedPointer<QMGE_GLWindow> QMGE_App::createWindow(QSize size, bool isFullScreen)
{
    QSurfaceFormat config;
    config.setDepthBufferSize(24);
    config.setStencilBufferSize(8);
    config.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    config.setSwapInterval(1);
    config.setSamples(0);
    if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        config.setRenderableType(QSurfaceFormat::OpenGL);
        config.setProfile(QSurfaceFormat::CoreProfile);
        config.setVersion(4,5);
    }
    else
    {
        config.setRenderableType(QSurfaceFormat::OpenGLES);
        config.setVersion(3,2);
    }
    //set default format
    QSurfaceFormat::setDefaultFormat(config);

    //check
    if(!m_window.isNull())
    {
        //reset to nullptr
        m_window.reset();
    }

    //create window and set size
    m_window.reset(new QMGE_GLWindow(config,isFullScreen));
    m_window->setWidth(size.width());
    m_window->setHeight(size.height());

    //create renderer
    m_renderer.reset(new QMGE_Renderer(config,m_window.data()));
    //create thread
    m_renderThread.reset(new QThread());

    //move m_renderer to thread
    m_renderer->moveToThread(m_renderThread.data());

    //when window exposed,a updaterequest event will be sent to m_renderer by execRender
    connect(m_window.data(),SIGNAL(startRenderThread()),m_renderer.data(),SLOT(execRender()));
    //when window thread emit stop signal,first quit the event loop on render thread
    connect(m_window.data(),SIGNAL(stopRenderThread()),m_renderThread.data(),SLOT(quit()));
    //when render thread quit,do cleaning up on m_renderer
    connect(m_renderThread.data(),SIGNAL(finished()),m_renderer.data(),SLOT(cleanUp()));
    //when renderer done cleanup,it will emit readyToStop signal,and then we can close the window,quit the app
    connect(m_renderer.data(),SIGNAL(readyToStop()),m_window.data(),SLOT(safeClose()));

    //connect other renderer and render window signals/slots
    //resize event;
    connect(m_renderer.data(),SIGNAL(initialized()),m_window.data(),SLOT(postRendererInitialized()));
    connect(m_window.data(),SIGNAL(resized(int,int)),m_renderer.data(),SLOT(resizeGL(int,int)));
    //key and mouse
    connect(m_window.data(),SIGNAL(keyChanged(int,bool)),m_renderer.data(),SLOT(windowKeyChanged(int,bool)));
    connect(m_window.data(),SIGNAL(mouseMoved(int,int)),m_renderer.data(),SLOT(windowMouseMoved(int,int)));

    //return window pointer
    return m_window;
}

void QMGE_App::run()
{
    //check if window created
    if(m_window.isNull())
    {
        createWindow(QSize(0,0),true);
    }

    //show window
    if(m_window->isFullScreen())
    {
        m_window->showFullScreen();
    }
    else
    {
        m_window->show();
    }

    //start render thread
    m_renderThread->start();
}

}
//namespace QMGE_Core
