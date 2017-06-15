#pragma once
#include "GL/glew.h"
#include <vector>


// represents the FBO objects, that is used mostly in offscreen rendering
class FrameBufObj 
{
private:
	GLuint _frameBuffObjId = 0;

	std::vector<GLuint> _targets;
	std::vector<GLenum> _drawBuffers;

	bool _isBound = false;

	void unbind();

	static FrameBufObj *CurentBinding;

public:
	FrameBufObj();
	~FrameBufObj();

	// destroys the framebuffer
	void destroy();

	// resets all the bindings 
	static void resetBinding();

	// creates Frame Buffer object
	bool createAndBind();
	bool createAndBind(GLuint, GLuint);

	// bind framebuffer
	bool bind();

	// simplifies process of binding texture from color target
	void bindColourTarget(GLuint);

	// attaches texture
	void attachTexture(GLuint, GLuint);

	// checks its completness
	static bool check();
};
