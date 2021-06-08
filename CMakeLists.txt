cmake_minimum_required(VERSION 3.4.1)

project(clahe)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
# Message(STATUS ${GLEW_LIBRARIES})
# Message(STATUS ${OPENGL_LIBRARY})
set(OpenGL_LIBS
	glfw
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARY}
)

# set(DCMTK_HOME "C:/Users/kroth/Desktop/dcmtk-master")
# set(GLFW_HOME "${CMAKE_SOURCE_DIR}/glfw")
# set(GLEW_HOME "${CMAKE_SOURCE_DIR}/glew-2.1.0")
set(GLM_INCLUDE "/usr/local/include/glm")
# set(PROJ_DIR "C:/Users/kroth/Documents/UCSD/Grad/Thesis/clahe_2")

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE "Debug")

# add_compile_definitions(clahe SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders")
set(SRC_FILES
	Shader.cpp
	# ImageLoader.cpp
	dicomLoader.cpp
	Camera.cpp
	Cube.cpp
	ComputeCLAHE.cpp
	SceneManager.cpp
	main.cpp
)
# add_executable(clahe "core.h" "main.cpp" "SceneManager.cpp" "Shader.cpp"
# 	"ImageLoader.cpp" "Cube.cpp" "Camera.cpp" "ComputeCLAHE.cpp")

add_executable(clahe ${SRC_FILES})

target_include_directories(clahe PUBLIC 
	# "${GLFW_HOME}/include" 
	# "${GLEW_HOME}/include" 
	# "${DCMTK_HOME}/include"
	"${GLM_INCLUDE}"
	"${CMAKE_SOURCE_DIR}"
	)

# if(WIN32)
# 	target_compile_definitions(clahe PUBLIC -DGLEW_STATIC)

# 	target_link_directories(clahe PUBLIC "${DCMTK_HOME}/lib")
# 	target_link_libraries(clahe 
# 		"${GLFW_HOME}/lib/glfw3.lib"
# 		"${GLEW_HOME}/lib/Release/x64/glew32s.lib"
# 		"OpenGL32.lib"
# 		"ofstd.lib" "oflog.lib" "dcmdata.lib" "dcmimgle.lib"
# 		"ws2_32.lib" "wsock32.lib" "shlwapi.lib" "iphlpapi.lib" "netapi32.lib" "propsys.lib")
# else()
	target_link_libraries(clahe
		${OpenGL_LIBS}
		# "libglfw.so" 
		# "libGLEW.so" 
		# "libGL.so"
	)
# endif()