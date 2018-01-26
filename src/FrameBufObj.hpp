#pragma once
#include "GL/glew.h"
#include <vector>


// represents the FBO objects, that is used mostly in offscreen rendering
class FrameBufObj 
{
private:
	GLuint _frameBuffObjId = 0;

	GLuint _colorBuff;
	GLenum _drawBuff;

	bool _isBound = false;

	void unbind();

	static FrameBufObj *_curentBinding;

public:
	FrameBufObj();
	~FrameBufObj();

	// destroys the framebuffer
	void destroy();

	// resets all the bindings 
	static void resetBinding();

	// creates framebuffer object
	bool createAndBind();

	// creats a framebuffer object, attaches the texture and checks if it is working correctely
	bool createAndBind(GLuint);

	// bind framebuffer
	bool bind();

	// simplifies process of binding texture from color target
	void bindColourTarget() const;

	// attaches texture
	void attachTexture(GLuint);

	// checks its completness
	static bool check();
};
