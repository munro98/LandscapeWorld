#include "FrameBufObj.hpp"
#include <iostream>

using namespace std;


FrameBufObj *FrameBufObj::_curentBinding = nullptr;

FrameBufObj::FrameBufObj(): _colorBuff(0), _drawBuff(0)
{
}

FrameBufObj::~FrameBufObj() 
{
	destroy();
}

void FrameBufObj::destroy()
{
	// Cleanup
	if (_frameBuffObjId != 0)
	{
		glDeleteFramebuffers(1, &_frameBuffObjId);
		_frameBuffObjId = 0;
	}
}

void FrameBufObj::resetBinding()
{
	// unbind current bindings
	if (_curentBinding != nullptr)
	{
		_curentBinding->unbind();
	}

	_curentBinding = nullptr;

	// set the framebuffer and texture to the default (0)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawBuffer(GL_BACK);
}

bool FrameBufObj::createAndBind()
{
	// unbind current bindings it is not bound to this object
	if (_curentBinding != nullptr && _curentBinding != this)
	{
		_curentBinding->unbind();
	}

	// generate and bind Framebuffer 
	glGenFramebuffers(1, &_frameBuffObjId);

	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffObjId);
	_isBound = true;
	_curentBinding = this;

	return true;
}

bool FrameBufObj::createAndBind(GLuint texId)
{
	createAndBind();
	attachTexture(texId);
	return check();
}

bool FrameBufObj::bind()
{
	// if already bound, there is nothing to do
	if (_isBound)
	{
		return true;
	}

	// unbind current bindings it is not bound to this object
	if (_curentBinding != nullptr && _curentBinding != this)
	{
		_curentBinding->unbind();
	}

	// bind the framebuffer and set the current binding to this object
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffObjId);
	_isBound = true;
	_curentBinding = this;

	return true;
}

void FrameBufObj::bindColourTarget() const
{
	glBindTexture(GL_TEXTURE_2D, _colorBuff);
}

void FrameBufObj::attachTexture(GLuint texId)
{
	_drawBuff = GL_COLOR_ATTACHMENT0;
	_colorBuff = texId;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);

	glDrawBuffers(1, static_cast<const GLenum *>(&_drawBuff));
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
	if (_curentBinding == this)
	{
		_isBound = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}