
#TODO Add header files
SET(headers
	"Shader.hpp"
	"SimpleGUI.hpp"
	"TriangleRenderer.hpp"
	"CoreRenderer.hpp"
	"ModelRenderer.hpp"
	"OBJLoader.hpp"
	"Mesh.hpp"
	"Model.hpp"
	"ModelShader.hpp"
	"Loader.hpp"
	"Camera.hpp"
	"Texture.hpp"
	"Terrain.hpp"
	"TerrainRenderer.hpp"
	"HeightGenerator.hpp"
	"TerrainShader.hpp"
	"SkydomeRenderer.hpp"
	"SkydomeShader.hpp"
	"World.hpp"
	"TerrainPosition.hpp"
	"BlendMapTexture.hpp"
	"_WaterRenderer.hpp"
	"_WaterShader.hpp"
	"Frustum.hpp"
	"BoundingBox.hpp"
	"WaterRenderer.hpp"
	"WaterShader.hpp"
	"WaterHeightShader.hpp"
	"WaterNormalShader.hpp"
	"WaterAddDropShader.hpp"
	"FrameBufObj.hpp"
	"MousePicker.hpp"
	"GrassRenderer.hpp"
	"GrassShader.hpp"
)


#TODO Add sources
SET(sources
	"main.cpp"
	"SimpleGUI.cpp"
	"Shader.cpp"
	"TriangleRenderer.cpp"
	"CoreRenderer.cpp"
	"ModelRenderer.cpp"
	"OBJLoader.cpp"
	"Mesh.cpp"
	"Model.cpp"
	"ModelShader.cpp"
	"Loader.cpp"
	"Camera.cpp"
	"Texture.cpp"
	"Terrain.cpp"
	"TerrainRenderer.cpp"
	"HeightGenerator.cpp"
	"TerrainShader.cpp"
	"SkydomeRenderer.cpp"
	"SkydomeShader.cpp"
	"World.cpp"
	"TerrainPosition.cpp"
	"BlendMapTexture.cpp"
	"_WaterRenderer.cpp"
	"_WaterShader.cpp"
	"Frustum.cpp"
	"BoundingBox.cpp"
	"WaterRenderer.cpp"
	"WaterShader.cpp"
	"WaterHeightShader.cpp"
	"WaterNormalShader.cpp"
	"WaterAddDropShader.cpp"
	"FrameBufObj.cpp"
	"MousePicker.cpp"
	"GrassRenderer.cpp"
	"GrassShader.cpp"
)

#Add executable target
add_executable(${LANDSCAPE_WORLD} ${headers} ${sources})
#Link libraries
target_link_libraries(${LANDSCAPE_WORLD} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${LANDSCAPE_WORLD} PRIVATE imgui)
target_link_libraries(${LANDSCAPE_WORLD} PRIVATE stb_image)
target_link_libraries (${LANDSCAPE_WORLD} PRIVATE ${CMAKE_THREAD_LIBS_INIT})



