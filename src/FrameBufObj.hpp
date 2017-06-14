#pragma once
#include "GL/glew.h"
#include <vector>


// represents the FBO objects, that is used mostly in offscreen rendering
class FrameBufObj 
{
private:
	struct Target 
	{
		Target() { _object = 0; _type = 0; _active = false; }
		GLuint _object;
		GLenum _type;
		bool _active;
	};

	GLuint _fboId = 0;

	std::vector<Target> _targets;
	Target _depthTarget;
	std::vector<GLenum> _drawBuffers;

	bool _isBound = false;

	int _width = 0;
	int _height = 0;

	void unbind();

	static GLuint MaxColourTargets;
	static GLuint MaxRenderbuffSize;
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

	// bind framebuffer
	bool bind();

	// simplifies process of binding texture from color target
	void bindColourTarget(GLuint);

	// attaches texture
	void attachTexture(GLuint, GLuint, int, int);

	// selects buffers to draw by the openGL
	void setDrwaBuff();

	// checks its completness
	bool check();
};
