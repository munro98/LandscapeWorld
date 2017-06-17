
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
#include "MousePicker.hpp"

#include "Frustum.hpp"
#include "WaterRenderer.hpp"

using namespace std;

GLFWwindow* window;

static const float fovx = 80.0f;
static const float znear = 0.1f;
static const float zfar = 2000.0f;


bool leftMouseDown = false;
glm::vec2 mousePosition = glm::vec2(0.0, 0.0);
glm::vec2 lastMousePosition = glm::vec2(0.0, 0.0);

glm::vec2 currentWaterTexturePoint = glm::vec2(0.0, 0.0);

bool hasWindowFocus = true;
Camera camera;

// Water related fields
WaterRenderer* _waterRendere;
float _dropSize = 0.5;
float _rainIntensity = 0.6;
glm::vec3 _waterPosition;

bool showMenu = true;
bool isFlying = false;

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
	{
		camera.rotate(xoffset, -yoffset);
	}
}



// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	// cout << "Mouse Button Callback :: button=" << button << "action=" << action << "mods=" << mods << endl;
	// send messages to the GUI manually
	SimpleGUI::mouseButtonCallback(win, button, action, mods);

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		leftMouseDown = (action == GLFW_PRESS);
		if (action == 0)
		{
			_waterRendere->addDrop(currentWaterTexturePoint.x, currentWaterTexturePoint.y, _dropSize);
		}
	}
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

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		isFlying = !isFlying;
	}
}


//Character callback
//Called for every character input event on since the last glfwPollEvents
void charCallback(GLFWwindow *win, unsigned int c) {
	// cout << "Char Callback :: c=" << char(c) << endl;

}

float deltaFrame = 0.0;
float lastFrame = 0.0;

void relocateWater(float centerX, float centerZ, World& world)
{
	const int initWidth = 201;
	const int scaleFactor = 2;
	float x = 0;
	float z = 0;
	float height = 0;

	// Expanding search for a good location for the tub
	for (int i = 1; i < 10; i++)
	{
		for(int j = 0; j < i*100; j++)
		{
			x = ((rand() % initWidth) - initWidth / 2) + centerX;
			z = ((rand() % initWidth) - initWidth / 2) + centerZ;
			height = world.heightAt(x, z);

			// Check the height difference between the 4 edges of the tub. if bigger than 0.5, look for another place
			float diff = abs(height - world.heightAt(x - scaleFactor, z - scaleFactor));
			diff = max(diff, abs(height - world.heightAt(x, z - scaleFactor)));
			diff = max(diff, abs(height - world.heightAt(x - scaleFactor, z)));
			if(diff < 0.5)
			{
				i = 11;
				j = 1000000;
			}
		}
	}

	// set the actual position
	_waterPosition.x = x;
	_waterPosition.y = height;
	_waterPosition.z = z;
}

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
	

	bool updateFrustum = true;
	float showBlendMap = 0.0f;
	float snowCoverage = 0.99f;
	bool showColour = true;

	World world;
	//TriangleRenderer triangleRenderer;
	ModelRenderer modelRenderer(projection);
	TerrainRenderer terrainRenderer(projection, world);
	SkydomeRenderer skydomeRenderer(projection);

	MousePicker mousePicker(world, *world.findTerrainAt(0,0));
	
	//WaterRenderer_Old waterRenderer(projection);
	glm::vec3 lightDir = glm::vec3(0, 1, 0);
	_waterRendere = new WaterRenderer(projection, lightDir);
	_waterPosition = glm::vec3(40, 0, 40);

	Mesh *mesh = OBJLoader::loadObjModel("box");
	

	while (!glfwWindowShouldClose(window)) {

		//Poll for and process events
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		deltaFrame = currentFrame - lastFrame;
		lastFrame = currentFrame;



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

		if (isFlying) {
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
		}
		else {
			bool takeInput = false;
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				takeInput |= true;
				camera.leftAccelerate(deltaFrame);
			}

			if (glfwGetKey(window, GLFW_KEY_D))
			{
				takeInput |= true;
				camera.rightAccelerate(deltaFrame);
			}

			if (glfwGetKey(window, GLFW_KEY_W))
			{
				takeInput |= true;
				camera.forwardAccelerate(deltaFrame);
			}

			if (glfwGetKey(window, GLFW_KEY_S))
			{
				takeInput |= true;
				camera.backwardAccelerate(deltaFrame);
			}

			camera.update(deltaFrame, world, takeInput);


		}


		glm::vec3 cameraPos = camera.getPosition();

		world.update(cameraPos.x, cameraPos.z);

		glClearColor(0.564f, 0.682f, 0.831f, 1.0f); // Blueish sky colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projection = glm::perspective(90.0f, (float)width / (float)height, 0.5f, 2000.0f);


		//////////////////////////////////////////////////////
		glm::mat4 view;
		view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());

		mousePicker.update(width, height, mousePosition.x, mousePosition.y, projection, view, cameraPos);
		currentWaterTexturePoint = mousePicker.m_currentWaterTexturePoint;
		//glm::vec3 ray = mousePicker.getCurrentRay();
		//std::cout << ray.x << " " << ray.y << " " << ray.z << "\n";

		Frustum frustum(view, projection);

		if (updateFrustum) {
			world.checkTerrainInFrustum(frustum);
		}

		glm::mat4 model(1);

		glm::mat4 boxModel(1);
		
		//glDisable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		skydomeRenderer.render(view, model);
		glClear(GL_DEPTH_BUFFER_BIT); // Everything goes on top of sky
		//glEnable(GL_DEPTH_TEST);
		terrainRenderer.render(view, model, projection, cameraPos, showBlendMap, snowCoverage);

		//glm::translate(model, camera.getPosition() + glm::vec3(0, -10, 0));
		//float heightAt = world.heightAt(camera.getPosition().x, camera.getPosition().z);
		//model = glm::translate(model, glm::vec3(camera.getPosition().x, heightAt, camera.getPosition().z));

		if(_waterPosition.y == 0)
		{
			_waterPosition.y = world.heightAt(_waterPosition.x, _waterPosition.z);
			mousePicker.SetWaterPosition(_waterPosition);
		}

		model = glm::translate(model, _waterPosition);
		Terrain *t = world.findTerrainAt(0, 0);
		glm::vec3 v;
		if (t != nullptr) {
			v = t->getNormal(camera.getPosition().x, camera.getPosition().y);
		}

		boxModel = glm::translate(boxModel, mousePicker.getCurrentTerrainPoint());
		boxModel = glm::scale(boxModel, glm::vec3(0.2, 0.2, 0.2));
		modelRenderer.render(view, boxModel, projection, mesh);

		//glEnable(GL_BLEND); // Water can be transparent
		glDisable(GL_CULL_FACE);
		_waterRendere->render(view, model, projection, cameraPos, _dropSize, _rainIntensity);
		glEnable(GL_CULL_FACE);
		//glDisable(GL_BLEND);

		// Render GUI on top
		/*
		if (showMenu) {
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&showMenu);
		}
		*/

		//SimpleGUI::render();
		if (showMenu) {
			SimpleGUI::newFrame();

			if (ImGui::IsMouseClicked(1))
				ImGui::OpenPopup("Player");

			if (ImGui::BeginPopup("Player")) {
				if (ImGui::Selectable("1")) {


				}

				if (ImGui::Selectable("2")) {


				}

				if (ImGui::Selectable("3")) {


				}

				ImGui::EndPopup();
			}

			//Uncomment to show test window
			//ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			//ImGui::ShowTestWindow(&showMenu);

			static bool no_titlebar = false;
			static bool no_border = true;
			static bool no_resize = false;
			static bool no_move = false;
			static bool no_scrollbar = false;
			static bool no_collapse = false;
			static bool no_menu = false;

			// Demonstrate the various window flags. Typically you would just use the default.
			ImGuiWindowFlags window_flags = 0;
			if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
			if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
			if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
			if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
			if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
			if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
			if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
			ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiSetCond_FirstUseEver);
			if (ImGui::Begin("LandScape Demo", &showMenu, window_flags))
			{
				ImGui::PushItemWidth(-140);// Right align, keep 140 pixels for labels
				//ImGui::Text("hello.");

				static int seedValue = 0;
				static bool interpolateNoise = true;
				ImGui::InputInt("Terrain seed int", &seedValue);
				ImGui::Checkbox("Cosine Interp Noise", &interpolateNoise);
				static bool a = false;
				if (ImGui::Button("Apply Seed(lots of memory allocation)")) { 
					world.applyNewSeed(seedValue, interpolateNoise);
					cout << "Applying seed\n"; 
					a ^= 1; 
				}
				//static bool showBlendMap = true;
				//ImGui::Checkbox("Show terrain blendMap", &showBlendMap);
				ImGui::SliderFloat("Terrain blendMap", &showBlendMap, 0.0f, 1.0f, "%.3f");
				ImGui::SliderFloat("Snow coverage", &snowCoverage, 0.0f, 1.0f, "%.3f");
				ImGui::Checkbox("Update frustum", &updateFrustum);

				// Water settings
				ImGui::Text("Water Settings");
				ImGui::SliderFloat("Rain intensity", &_rainIntensity, 0.0f, 1.0f, "%.3f");
				ImGui::SliderFloat("Drop size", &_dropSize, 0.4f, 1.0f, "%.3f");
				if (ImGui::Button("Relocate Water"))
				{
					auto camPos = camera.getPosition();
					relocateWater(camPos.x, camPos.z, world);
				}

				if(ImGui::Checkbox("Show Colour", &showColour))
				{
					_waterRendere->showColor(showColour);
				}
			}

			ImGui::End();


			//Flush components and render
			ImGui::Render();
		}

		//Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}
