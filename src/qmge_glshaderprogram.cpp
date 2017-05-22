#include "qmge_glshaderprogram.h"

namespace QMGE_Core
{


QMGE_GLShaderProgram::QMGE_GLShaderProgram() : QOpenGLShaderProgram()
{

}

QMGE_GLShaderProgram::~QMGE_GLShaderProgram()
{

}

void QMGE_GLShaderProgram::linkProgram()
{
    if(!link())
    {
        qWarning()<<log();
        qFatal("Shader link failed.");
    }

    //setup uniforms from config file
    setupUniforms();
    //and update those need only once update
    update_once();
}

void QMGE_GLShaderProgram::setShaderConfigFile(QString filename)
{
    m_configFileName = filename;
}

void QMGE_GLShaderProgram::update_frame()
{
    for(auto p : m_frameUpdateUniforms)
    {
        updateUniform(p);
    }
}

void QMGE_GLShaderProgram::update_fixed()
{
    for(auto p : m_fixedTimeUpdateUniforms)
    {
        updateUniform(p);
    }
}

void QMGE_GLShaderProgram::update_once()
{
    for(auto p : m_updateOnceUniforms)
    {
        updateUniform(p);
    }
}

void QMGE_GLShaderProgram::setupUniforms()
{
    if(!isLinked())
    {
        qWarning("Program not linked,can not set up uniforms.");
        return;
    }

    if(!bind())
    {
        qFatal("ShaderProgram bind failed.Check OpenGL context.");
    }

    //check if file has been set
    if(m_configFileName.isEmpty()) return;

    //Retrieve the source code from filePath
    QFile ConfigFile(m_configFileName);

    if(!ConfigFile.open(QFile::ReadOnly))
    {
        qWarning()<<"Can not open shader config file.Will not configure uniforms of this shader.";
    }
    else
    {
        QTextStream stream(&ConfigFile);
        QString line;
        QString name;
        QString type;
        QString updateMethod;
        QString::SectionFlag flag = QString::SectionSkipEmpty;

        while(stream.readLineInto(&line))
        {
            //skip comment lines
            if(line.startsWith('#')) continue;
            name = line.section(QChar(','),0,0,flag).trimmed();
            type = line.section(QChar(','),1,1,flag).trimmed();
            updateMethod = line.section(QChar(','),2,2,flag).trimmed();
            if(name.isEmpty() || type.isEmpty() || updateMethod.isEmpty())
            {
                continue;
            }
            else
            {
                QMGE_ShaderUniform newUniform;
                //name
                newUniform.name = name;
                //type
                switch (type.at(0).toLatin1())
                {
                case '1':
                    newUniform.type = QMGE_UniformType::INT;
                    break;
                case '2':
                    newUniform.type = QMGE_UniformType::FLOAT;
                    break;
                case '3':
                    newUniform.type = QMGE_UniformType::VEC2;
                    break;
                case '4':
                    newUniform.type = QMGE_UniformType::VEC3;
                    break;
                case '5':
                    newUniform.type = QMGE_UniformType::VEC4;
                    break;
                case '6':
                    newUniform.type = QMGE_UniformType::MAT3;
                    break;
                case '7':
                    newUniform.type = QMGE_UniformType::MAT4;
                    break;
                default:
                    //unknown type
                    qWarning()<<"Config read an unrecognized uniform type.";
                    continue;
                }

                //locations
                newUniform.location = uniformLocation(name);
                if(newUniform.location == -1)
                {
                    qWarning("Can not get uniform location in linked program.Skipping line.");
                    continue;
                }

                //bind host uniforms
                if(!QMGE_GLUniformManager::getInstance()->bindShaderUniform(newUniform))
                {
                    qWarning()<<"Shader Uniform bind failed.";
                    continue;
                }

                //update type
                switch (updateMethod.at(0).toLatin1())
                {
                case '0':
                    m_frameUpdateUniforms.push_back(newUniform);
                    break;
                case '1':
                    m_fixedTimeUpdateUniforms.push_back(newUniform);
                    break;
                case '2':
                    m_updateOnceUniforms.push_back(newUniform);
                    break;
                default:
                    //unknown update ype
                    qWarning("Unknown uniform update type in config file.");
                    continue;
                }
            }
        }//while readLine

        ConfigFile.close();
    }
}

void QMGE_GLShaderProgram::updateUniform(QMGE_ShaderUniform &target)
{
    if(target.dataPtr == nullptr) return;

    switch(target.type)
    {
    case QMGE_UniformType::INT:
        setUniformValueArray(target.location,((GLint *)target.dataPtr),1);
        break;
    case QMGE_UniformType::FLOAT:
        setUniformValueArray(target.location,((GLfloat *)target.dataPtr),1,1);
        break;
    case QMGE_UniformType::VEC2:
        setUniformValueArray(target.location,((QVector2D *)target.dataPtr),1);
        break;
    case QMGE_UniformType::VEC3:
        setUniformValueArray(target.location,((QVector3D *)target.dataPtr),1);
        break;
    case QMGE_UniformType::VEC4:
        setUniformValueArray(target.location,((QVector4D *)target.dataPtr),1);
        break;
    case QMGE_UniformType::MAT3:
        setUniformValueArray(target.location,((QMatrix3x3 *)target.dataPtr),1);
        break;
    case QMGE_UniformType::MAT4:
        setUniformValueArray(target.location,((QMatrix4x4 *)target.dataPtr),1);
        break;
    default:
        break;
    }
}

}
//namespace QMGE_Core

