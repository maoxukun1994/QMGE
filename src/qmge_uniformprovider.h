#ifndef QMGE_UNIFORMPROVIDER_H
#define QMGE_UNIFORMPROVIDER_H

#include "qmge_global.h"
#include "qmge_gluniformmanager.h"

namespace QMGE_Core
{

class QMGE_UniformProvider
{

public:

    QMGE_UniformProvider();

    virtual ~QMGE_UniformProvider();

protected:

    virtual void registerUniforms();
};
//class QMGE_UniformProvider

}
//namespace QMGE_Core

#endif // QMGE_UNIFORMPROVIDER_H
