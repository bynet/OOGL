cmake_minimum_required(VERSION 2.8.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# include(CreateLaunchers)

if(MSVS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  if(NOT "${MSVC_VERSION}" LESS 1400)
    add_definitions( "/MP" )
  endif()
endif(MSVS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OOGL)
set(OUTPUT OOGL)

include (Utils.cmake)
SET(SRC "")
SET(INC "")

### OpenGL ###
find_package(OpenGL REQUIRED)
list(APPEND LIB ${OPENGL_LIBRARY})

## SDL2 ###
set(SDL2_ROOT "c:/projects/libs/SDL2")
include_directories("${SDL2_ROOT}/include")
#link_directories("${SDL2_ROOT}/lib")
#list(APPEND LIB SDL2d)
#list(APPEND LIB SDL2maind)


### GLEW ###
set(GLEW_ROOT "c:/projects/libs/glew")
include_directories("${GLEW_ROOT}/include")
#set(GLEW_LIB "${GLEW_ROOT}/lib/glew32sd.lib")
#set(GLEW_LIB "${GLEW_ROOT}/lib64/glew32sd.lib")
#link_directories("${GLEW_ROOT}/lib64")
#list(APPEND LIB ${GLEW_LIB})
#list(APPEND LIB glew32sd)

add_definitions(-DGLEW_STATIC)


### ext 
include_directories("${CMAKE_SOURCE_DIR}/ext/z-logger")

list(APPEND SRC src/lib.cpp)
list(APPEND INC src)

list(APPEND SRC src/GL/GL/Error.cpp)
list(APPEND SRC src/GL/GL/VertexBuffer.cpp)
list(APPEND SRC src/GL/GL/IndexBuffer.cpp)
list(APPEND SRC src/GL/GL/VertexArray.cpp)
list(APPEND SRC src/GL/GL/Texture.cpp)
list(APPEND SRC src/GL/GL/Shader.cpp)
list(APPEND SRC src/GL/GL/Program.cpp)
list(APPEND SRC src/GL/GL/Framebuffer.cpp)
list(APPEND SRC src/GL/GL/GC.cpp)

list(APPEND INC include)

list(APPEND SRC src/GL/GL/Context.cpp)
#list(APPEND SRC src/GL/GL/Context_SDL2.cpp)

list(APPEND SRC src/GL/Window/Window.cpp)
list(APPEND SRC src/GL/Window/Window_SDL2.cpp)

FILE(GLOB_RECURSE JPEG_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/GL/util/libjpeg/*.c" )
FILE(GLOB_RECURSE PNG_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/GL/util/libpng/*.c" )
FILE(GLOB_RECURSE ZLIB_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/GL/util/zlib/*.c" )
FILE(GLOB_RECURSE MATH_LIB_SRC RELATIVE ${CMAKE_SOURCE_DIR} "src/GL/Math/*.cpp" )


list(APPEND SRC ${JPEG_SRC} )
list(APPEND SRC ${PNG_SRC} )
list(APPEND SRC ${ZLIB_SRC} )
list(APPEND SRC ${MATH_LIB_SRC} )

include_directories(src/GL/Math)
include_directories(src/GL/Util/libjpeg)
include_directories(src/GL/Util/libpng)
include_directories(src/GL/Util/zlib)

list(APPEND SRC src/GL/Util/Mesh.cpp)
list(APPEND SRC src/GL/Util/Image.cpp)

build_lib(${OUTPUT} SOURCES ${SRC} HEADERS ${INC} LIBS ${LIB})