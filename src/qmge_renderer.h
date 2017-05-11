#ifndef QMGE_RENDERER_H
#define QMGE_RENDERER_H

#include "qmge_includes.h"

namespace QMGE_Core
{


class QMGE_Renderer : public QObject
{
    Q_OBJECT

signals:

public:

    QMGE_Renderer(QObject * parent = nullptr,QSurfaceFormat contextSettings);

    ~QMGE_Renderer();

public slots:


};
//class QMGE_Renderer



}
//namespace QMGE_Core


#endif // QMGE_RENDERER_H
