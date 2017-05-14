#ifndef QMGE_GLSHADERPROGRAM_H
#define QMGE_GLSHADERPROGRAM_H

#include "qmge_global.h"
#include "qmge_gluniformmanager.h"

namespace QMGE_Core
{

class QMGE_GLShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT

public:

    QMGE_GLShaderProgram();
    ~QMGE_GLShaderProgram();

    void linkProgram();
    void setShaderConfigFile(QString filename);
    void update_frame();
    void update_fixed();
    void update_once();

protected:

    void setupUniforms();
    void updateUniform(QMGE_ShaderUniform &target);

private:

    //shader config file name
    QString m_configFileName;

    //uniforms
    QList<QMGE_ShaderUniform> m_frameUpdateUniforms;
    QList<QMGE_ShaderUniform> m_fixedTimeUpdateUniforms;
    QList<QMGE_ShaderUniform> m_updateOnceUniforms;
};
//class QMGE_GLShaderProgram

}
//namespace QMGE_Core

#endif // QMGE_GLSHADERPROGRAM_H
