#pragma once

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QScopedPointer>

#define GL_TRIANGLE_WIREFRAME__ 8764846

class ShaderRenderer
{
public:
	typedef QScopedPointer<ShaderRenderer> Ptr;

	// constructors
	ShaderRenderer(QVector<QVector3D>& vertices);
	ShaderRenderer(QVector<QVector3D>& vertices, QVector<QVector3D>& normals);
	ShaderRenderer(QVector<QVector3D>& vertices, QVector<QVector4D>& colors);
	ShaderRenderer(QVector<QVector3D>& vertices, QVector<QVector3D>& normals, QVector<QVector4D>& colors);
	ShaderRenderer(const ShaderRenderer &) = delete;
        ~ShaderRenderer();

	// parameters
	void setMode(GLenum mode);
	void setPointSize(GLfloat size);
	void setLineWidth(GLfloat width);

	// Two options:
	// (1) create QOpenGLShader object for each shader
	// (2) create QOpenGLShaderProgram object for each shader
	// destroy shader program manually seems more economy 
	// because single QOpenGLShaderProgram can handle all
	// especially good for rendering method without switching shaders back and forth

	// destroy the program
	void destroyShaderProgram(); // call this method before change the rendering method

	// rendering methods
	void renderUniformColor(QColor color, QMatrix4x4 matrix);
	void renderUniformColorSphereFading(QColor color, QMatrix4x4 matrix);
	void renderUniformColorLighting(QColor color, QMatrix4x4 modelMatrix, QMatrix4x4 pvMatrix, QVector3D eyePos);
	void renderPerVertexColor(QMatrix4x4 matrix);
	void renderPerVertexColorLighting(QMatrix4x4 modelMatrix, QMatrix4x4 pvMatrix, QVector3D eyePos);

	// depth peeling
	void setDepthImage(int w, int h, const QVector<float>& image);
	void renderDepthPeeling(QMatrix4x4 matrix);

protected:
	void bakeGpuData(QVector<QVector3D>& vertices, 
        QVector<QVector3D>& normals,	QVector<QVector4D>& colors);
	void drawArray();

	void linkUniformColorProgram();
	void linkUniformColorLightingProgram();
	void linkUniformColorSphereFadingProgram();
	void linkPerVertexColorLightingProgram();
	void linkPerVertexColorProgram();
	void linkDepthPeelingProgram();
	void createDepthTexture();

protected:
        int					_nbVertex;

	QOpenGLBuffer				_vertexBuffer;
	QOpenGLBuffer				_normalBuffer;
	QOpenGLBuffer				_colorBuffer;
        QOpenGLVertexArrayObject                _vao;

        QOpenGLShaderProgram                    _program;

        GLenum					_mode;
        GLfloat					_pointSize;
        GLfloat					_lineWidth;

        int					_width, _height;
	QVector<float>				_depthImage;
	QOpenGLTexture*				_texture;
};
