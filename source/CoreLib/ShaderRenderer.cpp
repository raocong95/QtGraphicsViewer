#include "ShaderRenderer.h"
#include "MeshModel.h"


ShaderRenderer::ShaderRenderer(QVector<QVector3D>& vertices, QVector<QVector4D>& colors)
	:ShaderRenderer(vertices, QVector<QVector3D>(), colors){}
ShaderRenderer::ShaderRenderer(QVector<QVector3D>& vertices, QVector<QVector3D>& normals)
	:ShaderRenderer(vertices, normals, QVector<QVector4D>()){}
ShaderRenderer::ShaderRenderer(QVector<QVector3D>& vertices)
	: ShaderRenderer(vertices, QVector<QVector3D>(), QVector<QVector4D>()){}
ShaderRenderer::ShaderRenderer(QVector<QVector3D>& vertices,
	QVector<QVector3D>& normals, QVector<QVector4D>& colors)
{
	bakeGpuData(vertices, normals, colors);

	_mode = GL_POINTS;
	_pointSize = 1.0f;
	_lineWidth = 1.0f;

	_texture = nullptr;
}

void ShaderRenderer::bakeGpuData(QVector<QVector3D>& vertices, QVector<QVector3D>& normals, QVector<QVector4D>& colors)
{
	_nbVertex = vertices.size();

	// VAO
	_vao.create();
	_vao.bind();
	{
		// vertex buffer
		_vertexBuffer.create();
		_vertexBuffer.bind();
		{
			_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
			_vertexBuffer.allocate(vertices.data(), _nbVertex * sizeof(QVector3D));
		}
		_vertexBuffer.release();

		// normal buffer
		if (!normals.isEmpty())
		{
			_normalBuffer.create();
			_normalBuffer.bind();
			{
				_normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
				_normalBuffer.allocate(normals.data(), _nbVertex * sizeof(QVector3D));
			}
			_normalBuffer.release();
		}

		// color buffer
		if (!colors.isEmpty())
		{
			_colorBuffer.create();
			_colorBuffer.bind();
			{
				_colorBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
				_colorBuffer.allocate(colors.data(), _nbVertex * sizeof(QVector4D));
			}
			_colorBuffer.release();
		}
	}
	_vao.release();
}


ShaderRenderer::~ShaderRenderer()
{
	_vertexBuffer.destroy();
	_normalBuffer.destroy();
	_vao.destroy();
}


void ShaderRenderer::setDepthImage(int w, int h, const QVector<float>& image)
{
	// copy the depth image
	_width = w;
	_height = h;
	_depthImage = image;

	// delete current texture
	if (_texture)
	{
		delete _texture;
		_texture = nullptr;
	}
}

void ShaderRenderer::setMode(GLenum mode)
{
	_mode = mode;
}

void ShaderRenderer::setPointSize(GLfloat size)
{
	_pointSize = size;
}

void ShaderRenderer::setLineWidth(GLfloat width)
{
	_lineWidth = width;
}

void ShaderRenderer::destroyShaderProgram()
{
	_program.removeAllShaders();
}

void ShaderRenderer::linkUniformColorProgram()
{
	_program.removeAllShaders();
	_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/uniform_color_vertex.glsl");
	_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uniform_color_fragment.glsl");
	_program.link();

	_vao.bind();
	{
		_vertexBuffer.bind();
		{
			_program.enableAttributeArray("vertex");
			_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
		}
		_vertexBuffer.release();
	}
	_vao.release();
}

void ShaderRenderer::linkUniformColorLightingProgram()
{
	_program.removeAllShaders();
	_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/uniform_color_lighting_vertex.glsl");
	_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uniform_color_lighting_fragment.glsl");
	_program.link();

	_vao.bind();
	{
		// vertex VBO
		_vertexBuffer.bind();
		{
			_program.enableAttributeArray("vertex");
			_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
		}
		_vertexBuffer.release();

		// normal VBO
		_normalBuffer.bind();
		{
			_program.enableAttributeArray("normal");
			_program.setAttributeBuffer("normal", GL_FLOAT, 0, 3);
		}
		_normalBuffer.release();
	}
	_vao.release();
}

void ShaderRenderer::linkPerVertexColorLightingProgram()
{
	_program.removeAllShaders();
	_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/per_vertex_color_lighting_vertex.glsl");
	_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/per_vertex_color_lighting_fragment.glsl");
	_program.link();

	_vao.bind();
	{
		// vertex VBO
		_vertexBuffer.bind();
		{
			_program.enableAttributeArray("vertex");
			_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
		}
		_vertexBuffer.release();

		// normal VBO
		_normalBuffer.bind();
		{
			_program.enableAttributeArray("normal");
			_program.setAttributeBuffer("normal", GL_FLOAT, 0, 3);
		}
		_normalBuffer.release();

		// color VBO
		_colorBuffer.bind();
		{
			_program.enableAttributeArray("color");
			_program.setAttributeBuffer("color", GL_FLOAT, 0, 4);
		}
		_colorBuffer.release();
	}
	_vao.release();
}

void ShaderRenderer::linkPerVertexColorProgram()
{
	_program.removeAllShaders();
	_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/per_vertex_color_vertex.glsl");
	_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/per_vertex_color_fragment.glsl");
	_program.link();

	_vao.bind();
	{
		// vertex VBO
		_vertexBuffer.bind();
		{
			_program.enableAttributeArray("vertex");
			_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
		}
		_vertexBuffer.release();

		// color VBO
		_colorBuffer.bind();
		{
			_program.enableAttributeArray("color");
			_program.setAttributeBuffer("color", GL_FLOAT, 0, 4);
		}
		_colorBuffer.release();
	}
	_vao.release();
}

void ShaderRenderer::linkUniformColorSphereFadingProgram()
{
	_program.removeAllShaders();
	_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/uniform_color_sphere_fading_vertex.glsl");
	_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/uniform_color_sphere_fading_fragment.glsl");
	_program.link();

	_vao.bind();
	{
		_vertexBuffer.bind();
		{
			_program.enableAttributeArray("vertex");
			_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
		}
		_vertexBuffer.release();
	}
	_vao.release();
}

void ShaderRenderer::linkDepthPeelingProgram()
{
	_program.removeAllShaders();
	_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/depth_peeling_vertex.glsl");
	_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/depth_peeling_fragment.glsl");
	_program.link();

	_vao.bind();
	{
		_vertexBuffer.bind();
		{
			_program.enableAttributeArray("vertex");
			_program.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
		}
		_vertexBuffer.release();
	}
	_vao.release();
}

void ShaderRenderer::createDepthTexture()
{
	QImage dImg(_width, _height, QImage::Format_RGB32);
	for (auto x = 0; x < _width; x++) {
		for (auto y = 0; y < _height; y++) {
			float z = _depthImage[_width * y + x];
			int c = int(z * 255);
			dImg.setPixel(x, y, qRgb(c, c, c));
		}
	}

	//dImg.save("before-setup.png");

	_texture = new QOpenGLTexture(dImg, QOpenGLTexture::DontGenerateMipMaps);
}

void ShaderRenderer::renderUniformColor(QColor color, QMatrix4x4 matrix)
{
	// lazy set up 
	if (!_program.isLinked()) {
		linkUniformColorProgram();
	}

	// render
	_program.bind();
	{
		// uniforms
		_program.setUniformValue("pvmMatrix", matrix);
		_program.setUniformValue("Color", color);

		// draw model
		drawArray();
	}
	_program.release();
}

void ShaderRenderer::renderUniformColorLighting(QColor color,
	QMatrix4x4 modelMatrix, QMatrix4x4 pvMatrix, QVector3D eyePos)
{
	// lazy set up 
	if (!_program.isLinked())	{
		linkUniformColorLightingProgram();
	}

	// render
	_program.bind();
	{
		// uniforms
		auto normalMatrix = modelMatrix.inverted().transposed();

		_program.setUniformValue("modelMatrix", modelMatrix);
		_program.setUniformValue("viewMatrix", pvMatrix);
		_program.setUniformValue("normalMatrix", normalMatrix);
		_program.setUniformValue("Color", color);
		_program.setUniformValue("lightPos", eyePos);
		_program.setUniformValue("viewPos", eyePos);
		_program.setUniformValue("lightColor", QVector3D(1, 1, 1));

		// draw model
		drawArray();
	}
	_program.release();
}

void ShaderRenderer::renderPerVertexColorLighting(
	QMatrix4x4 modelMatrix, QMatrix4x4 pvMatrix, QVector3D eyePos)
{
	// lazy set up 
	if (!_program.isLinked()) {
		linkPerVertexColorLightingProgram();
	}

	// render
	_program.bind();
	{
		// uniforms
		auto normalMatrix = modelMatrix.inverted().transposed();

		_program.setUniformValue("modelMatrix", modelMatrix);
		_program.setUniformValue("viewMatrix", pvMatrix);
		_program.setUniformValue("normalMatrix", normalMatrix);
		_program.setUniformValue("lightPos", eyePos);
		_program.setUniformValue("viewPos", eyePos);
		_program.setUniformValue("lightColor", QVector3D(1, 1, 1));

		// draw model
		drawArray();
	}
	_program.release();
}

void ShaderRenderer::renderPerVertexColor(QMatrix4x4 matrix)
{
	// lazy set up 
	if (!_program.isLinked()) {
		linkPerVertexColorProgram();
	}

	// render
	_program.bind();
	{
		// uniforms
		_program.setUniformValue("pvmMatrix", matrix);

		// draw model
		drawArray();
	}
	_program.release();
}

void ShaderRenderer::renderUniformColorSphereFading(QColor color, QMatrix4x4 matrix)
{
	// lazy set up 
	if (!_program.isLinked()) {
		linkUniformColorSphereFadingProgram();
	}

	// render
	_program.bind();
	{
		// uniforms
		_program.setUniformValue("pvmMatrix", matrix);
		_program.setUniformValue("Color", color);
		// draw model
		drawArray();
	}
	_program.release();
}

void ShaderRenderer::renderDepthPeeling(QMatrix4x4 matrix)
{
	// lazy set up 
	if (!_program.isLinked()) {
		linkDepthPeelingProgram();
	}
	if (!_texture) {
		createDepthTexture();
	}

	// render
	_program.bind();
	_texture->bind();
	{
		// uniforms
		_program.setUniformValue("pvmMatrix", matrix);

		QVector2D size(_width, _height);
		_program.setUniformValue("windowSize", size);
		_program.setUniformValue("depthTex", 0);

		// draw model
		drawArray();
	}
	_texture->release();
	_program.release();
}

void ShaderRenderer::drawArray()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(_pointSize);
	glLineWidth(_lineWidth);

	_vao.bind();
	{
		if (_mode == GL_TRIANGLE_WIREFRAME__)
		{
			for (int i = 0; i < _nbVertex / 3; i++) {
				glDrawArrays(GL_LINE_LOOP, 3 * i, 3);
			}
		}
		else
		{
			glDrawArrays(_mode, 0, _nbVertex);
		}
	}
	_vao.release();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_POINT_SMOOTH);
}

