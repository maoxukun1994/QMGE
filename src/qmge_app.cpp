#include "qmge_app.h"

namespace QMGE_Core
{

QMGE_App::QMGE_App() : QObject()
{

}


QMGE_App::~QMGE_App()
{
    m_renderThread.wait();
}

void QMGE_App::createWindow(int width,int height,bool isFullScreen)
{
    QSurfaceFormat config;
    config.setDepthBufferSize(24);
    config.setStencilBufferSize(8);
    config.setSwapBehavior(QSurfaceFormat::TripleBuffer);
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

    //when window exposed,first a updaterequest event will be sent to m_renderer by execRender
    connect(m_window.data(),SIGNAL(startRenderThread()),m_renderer.data(),SLOT(execRender()));
    //then,thread containing m_renderer will start(or restart) event loop to handle events.
    connect(m_window.data(),SIGNAL(startRenderThread()),&m_renderThread,SLOT(start()));
    //m_renderer will stop the event loop in its thread when render window not available
    connect(m_renderer.data(),SIGNAL(stopExec()),&m_renderThread,SLOT(quit()));
    //move m_renderer to thread
    m_renderer->moveToThread(&m_renderThread);

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
}

}
//namespace QMGE_Core
