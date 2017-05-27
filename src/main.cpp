
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>

#include "TriangleRenderer.hpp"
#include "SimpleGUI.hpp"
#include "Shader.hpp"
#include "OBJLoader.hpp"
#include "ModelShader.hpp"
#include "Camera.hpp"
#include "Loader.hpp"
#include "ModelRenderer.hpp"
#include "SkydomeRenderer.hpp"
#include "TerrainRenderer.hpp"
#include "World.hpp"

using namespace std;


GLFWwindow* window;

static const float fovx = 80.0f;
static const float znear = 0.1f;
static const float zfar = 1000.0f;


bool leftMouseDown = false;
glm::vec2 mousePosition = glm::vec2(0.0, 0.0);
glm::vec2 lastMousePosition = glm::vec2(0.0, 0.0);

bool hasWindowFocus = true;
Camera camera;

void windowFocusCallback(GLFWwindow* win, int focused) {
	if (focused)
	{
		hasWindowFocus = true;// The window gained input focus
	}
	else
	{
		hasWindowFocus = false;// The window lost input focus
	}
}

// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {
	// cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;
	mousePosition = glm::vec2(xpos, ypos);

	float xoffset = mousePosition.x - lastMousePosition.x;
	float yoffset = mousePosition.y - lastMousePosition.y;

	xoffset /= 2.0f;
	yoffset /= 2.0f;

	lastMousePosition = mousePosition;

	if (leftMouseDown)
		camera.rotate(xoffset, -yoffset);
}



// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	// cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
	// send messages to the GUI manually
	SimpleGUI::mouseButtonCallback(win, button, action, mods);

	if (button == GLFW_MOUSE_BUTTON_LEFT)
		leftMouseDown = (action == GLFW_PRESS);
}


//Scroll callback
//Called for scroll event on since the last glfwPollEvents
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
	// cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
}


//Keyboard callback
//Called for every key event on since the last glfwPollEvents
void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	// cout << "Key Callback :: key=" << key << "scancode=" << scancode
	// 	<< "action=" << action << "mods=" << mods << endl;

}


//Character callback
//Called for every character input event on since the last glfwPollEvents
void charCallback(GLFWwindow *win, unsigned int c) {
	// cout << "Char Callback :: c=" << char(c) << endl;

}

float deltaFrame = 0.0;
float lastFrame = 0.0;

int main(int argc, char **argv) {
	//Initialize the GLFW library
	if (!glfwInit()) {
		cerr << "Error: Could not initialize GLFW" << endl;
		abort();
	}

	//glfwWindowHint(GLFW_SAMPLES, 4); // MSAA X 4
	//We want this version of gl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//Get the version for GLFW
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);


	//Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);

	if (!window) {
		cerr << "Error: Could not create GLFW window" << endl;
		abort();
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);


	// Initialize GLEW
	// must be done after making a GL context current (glfwMakeContextCurrent in this case)
	glewExperimental = GL_TRUE; // required for full GLEW functionality for OpenGL 3.0+
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}

	//Print out this OpenGL verisions
	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "Using GLFW " << glfwMajor << "." << glfwMinor << "." << glfwRevision << endl;



	//Attach input callbacks to window
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCharCallback(window, charCallback);
	glfwSetWindowFocusCallback(window, windowFocusCallback);

	int num_texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num_texture_units);
	cout << "Texture Units " << num_texture_units << endl;


	// Initialize IMGUI
	if (!SimpleGUI::init(window, false)) {
		cerr << "Error: Could not initialize IMGUI" << endl;
		abort();
	}

	glm::mat4 projection = glm::perspective(80.0f, (float)640 / (float)480, 0.5f, 2000.0f);
	

	World world;
	//TriangleRenderer triangleRenderer;
	ModelRenderer modelRenderer(projection);
	TerrainRenderer terrainRenderer(projection, world);
	SkydomeRenderer skydomeRenderer(projection);



	Mesh *mesh = OBJLoader::loadObjModel("box");
	

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaFrame = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Poll for and process events
		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		//Set viewport to be the whole window
		glViewport(0, 0, width, height);

		/*
		float xoffset = mousePosition.x - width / 2;
		float yoffset = mousePosition.y - height / 2;

		if (hasWindowFocus) {
			glfwSetCursorPos( window ,width / 2, height / 2);
		}

		float sensitivity = 0.1;
		xoffset = xoffset * sensitivity;
		yoffset = yoffset * sensitivity;
		camera.Rotate(xoffset, -yoffset);
		*/


		if (glfwGetKey(window, GLFW_KEY_A))
		{
			camera.left(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_D))
		{
			camera.right(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_W))
		{
			camera.forward(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_S))
		{
			camera.backward(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_F))
		{
			camera.left(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_H))
		{
			camera.right(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_T))
		{
			camera.forward(deltaFrame);
		}

		if (glfwGetKey(window, GLFW_KEY_G))
		{
			camera.backward(deltaFrame);
		}

		glm::vec3 cameraPos = camera.getPosition();

		world.update(cameraPos.x, cameraPos.z);

		glClearColor(0.564f, 0.682f, 0.831f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(90.0f, (float)width / (float)height, 0.5f, 2000.0f);


		//glDisable(GL_CULL_FACE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);


		//triangleRenderer.render();

		//////////////////////////////////////////////////////
		glm::mat4 view;
		view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());

		glm::mat4 model(1);
		
		//skyboxRenderer.render(view, model);
		//glDisable(GL_DEPTH_TEST);
		
		skydomeRenderer.render(view, model);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);
		terrainRenderer.render(view, model, projection, camera.getPosition());

		//glm::translate(model, camera.getPosition() + glm::vec3(0, -10, 0));
		//float heightAt = world.heightAt(camera.getPosition().x, camera.getPosition().z);
		//model = glm::translate(model, glm::vec3(camera.getPosition().x, heightAt, camera.getPosition().z));

		float heightAt = world.heightAt(50, 50);
		model = glm::translate(model, glm::vec3(50, heightAt, 50));

		modelRenderer.render(view, model, projection, mesh);

		

		// Render GUI on top
		SimpleGUI::render();

		//Swap front and back buffers
		glfwSwapBuffers(window);

		
	}

	glfwTerminate();
}
