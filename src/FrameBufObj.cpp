#include "FrameBufObj.hpp"
#include <iostream>

using namespace std;


FrameBufObj *FrameBufObj::CurentBinding = nullptr;

GLuint FrameBufObj::MaxColourTargets = 0;
GLuint FrameBufObj::MaxRenderbuffSize = 0;

FrameBufObj::FrameBufObj()
{
}

FrameBufObj::~FrameBufObj() 
{
	destroy();

	_targets.clear();
	_drawBuffers.clear();
}

void FrameBufObj::destroy()
{
	if (_targets.size() > 0 && _fboId != 0)
	{
		if (_depthTarget._active == true && _depthTarget._type == GL_RENDERBUFFER)
		{
			glDeleteRenderbuffers(1, &_depthTarget._object);
		}

		glDeleteFramebuffers(1, &_fboId);
		_fboId = 0;
	}
	_targets.clear();
}

void FrameBufObj::resetBinding()
{
	if (CurentBinding != nullptr)
	{
		CurentBinding->unbind();
	}

	CurentBinding = nullptr;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawBuffer(GL_BACK);
}

bool FrameBufObj::createAndBind()
{
	// calculate max values if this is first FBO...
	if (MaxColourTargets == 0 && MaxRenderbuffSize == 0)
	{
		int t = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &t);
		MaxColourTargets = static_cast<GLuint>(t);

		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &t);
		MaxRenderbuffSize = static_cast<GLuint>(t);
	}

	// add mx num of draw buffers to targets array
	_targets.clear();
	for (GLuint i = 0; i < MaxColourTargets; ++i)
	{
		_targets.push_back(Target());
	}

	if (CurentBinding != nullptr && CurentBinding != this)
	{
		CurentBinding->unbind();
	}

	glGenFramebuffers(1, &_fboId);

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	_isBound = true;
	CurentBinding = this;

	return true;
}

bool FrameBufObj::bind()
{
	if (_isBound) return true;

	if (CurentBinding != nullptr && CurentBinding != this)
	{
		CurentBinding->unbind();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);

	_isBound = true;
	CurentBinding = this;

	return true;
}

void FrameBufObj::bindColourTarget(GLuint colourTargetId)
{
	glBindTexture(GL_TEXTURE_2D, _targets[colourTargetId]._object);
}

void FrameBufObj::attachTexture(GLuint destId, GLuint texId, int w, int h)
{
	_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + destId);

	_targets[destId]._active = true;
	_targets[destId]._object = texId;
	_targets[destId]._type = GL_TEXTURE_2D;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+destId, GL_TEXTURE_2D, texId, 0);

	_width = w;
	_height = h;
}

void FrameBufObj::setDrwaBuff() 
{
	glDrawBuffers(static_cast<GLsizei>(_drawBuffers.size()), static_cast<const GLenum *>(&_drawBuffers[0]));
}

bool FrameBufObj::check()
{
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		cerr << "glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE" << endl;

		switch (status)
		{
		case GL_FRAMEBUFFER_UNSUPPORTED:
			cerr << "Unsupported framebuffer format." << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			cerr << "FrameBuffer incomplete attachment." << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			cerr << "FrameBuffer incomplete, missing attachment." << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			cerr << "FrameBuffer incomplete, missing draw buffer." << endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			cerr << "FrameBuffer incomplete, missing read buffer." << endl;
			break;
		default:
			cerr << "Unknown error " << status << endl;
			break;
		}

		return false;
	}

	return true;
}

void FrameBufObj::unbind()
{
	if (CurentBinding == this)
	{
		_isBound = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}