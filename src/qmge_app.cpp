#include "qmge_app.h"

namespace QMGE_Core
{

QMGE_App::QMGE_App() : QObject()
{
}


QMGE_App::~QMGE_App()
{
}

void QMGE_App::createWindow(int width,int height,bool isFullScreen)
{
    QSurfaceFormat config;
    config.setDepthBufferSize(24);
    config.setStencilBufferSize(8);
    config.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    config.setSwapInterval(0);
    config.setSamples(4);
    if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        config.setRenderableType(QSurfaceFormat::OpenGL);
        config.setProfile(QSurfaceFormat::CoreProfile);
        config.setVersion(4,3);
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

    m_window.reset(new QMGE_GLWindow(config));

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
    //when renderer done cleanup,it will emit readyToStop signal,and then we can close the window
    connect(m_renderer.data(),SIGNAL(readyToStop()),m_window.data(),SLOT(safeClose()));

    if(isFullScreen)
    {
        m_window->showFullScreen();
    }
    else
    {
        m_window->setWidth(width);
        m_window->setHeight(height);
        m_window->show();
    }
}


void QMGE_App::run()
{
    if(m_window.isNull())
    {
        createWindow(0,0,true);
    }

    m_renderThread->start();
}

}
//namespace QMGE_Core
