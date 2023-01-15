cmake_minimum_required(VERSION 3.11.0)

project(AstroEngine VERSION 1.0)

if (DEFINED VULKAN_SDK_PATH)
	set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/include")
	set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/lib")
	set(Vulkan_FOUND "True")
else()
	find_package(Vulkan REQUIRED) # throws error if could not find Vulkan
	message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

find_package(glfw3 3.3 REQUIRED)
set(GLFW_LIB glfw)
message(STATUS "Found GLFW")
if (NOT GLFW_LIB)
	message(FATAL_ERROR "Could not find glfw library!")
else()
	message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()

include_directories(external)

if (NOT TINYOBJ_PATH)
  message(STATUS "TINYOBJ_PATH not specified in .env.cmake, using external/tinyobjloader")
  set(TINYOBJ_PATH external/tinyobjloader)
endif()


if(NOT STB_PATH)
	message(STATUS "STB_PATH not specified in .env.cmake, using external/stb")
	set(STB_PATH external/stb)
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src
		/Users/antonin/VulkanSDK/1.3.236.0/macOS/include
		${TINYOBJ_PATH}
		${STB_PATH})

target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
