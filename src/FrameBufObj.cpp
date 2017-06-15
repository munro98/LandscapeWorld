#include "FrameBufObj.hpp"
#include <iostream>

using namespace std;


FrameBufObj *FrameBufObj::CurentBinding = nullptr;

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
	if (_targets.size() > 0 && _frameBuffObjId != 0)
	{
		glDeleteFramebuffers(1, &_frameBuffObjId);
		_frameBuffObjId = 0;
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
	int maxColAtt = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColAtt);

	// add mx num of draw buffers to targets array
	_targets.clear();
	for (GLuint i = 0; i < maxColAtt; ++i)
	{
		_targets.push_back(0);
	}

	if (CurentBinding != nullptr && CurentBinding != this)
	{
		CurentBinding->unbind();
	}

	glGenFramebuffers(1, &_frameBuffObjId);

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffObjId);
	_isBound = true;
	CurentBinding = this;

	return true;
}

bool FrameBufObj::createAndBind(GLuint destId, GLuint texId)
{
	createAndBind();
	attachTexture(destId, texId);
	return check();
}

bool FrameBufObj::bind()
{
	if (_isBound) return true;

	if (CurentBinding != nullptr && CurentBinding != this)
	{
		CurentBinding->unbind();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffObjId);

	_isBound = true;
	CurentBinding = this;

	return true;
}

void FrameBufObj::bindColourTarget(GLuint colourTargetId)
{
	glBindTexture(GL_TEXTURE_2D, _targets[colourTargetId]);
}

void FrameBufObj::attachTexture(GLuint destId, GLuint texId)
{
	_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + destId);

	_targets[destId] = texId;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+destId, GL_TEXTURE_2D, texId, 0);

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
			cerr << "Unsupported FrameBuffer format." << endl;
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