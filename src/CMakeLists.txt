set(EXE_FILE "pathtracer.exe")

file(GLOB_RECURSE ALL_SRC
    "*.h"
    "*.cpp"
)

include_directories(utils)
include_directories(engine)
include_directories(rendering)

add_executable(${EXE_FILE} ${ALL_SRC})

set(embree_DIR /home/bartosz/Downloads/embree-3.6.1.x86_64.linux)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(embree 3.0 REQUIRED)
find_package(ASSIMP REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${EMBREE_INCLUDE_DIRS})
include_directories(/usr/local/include/OpenEXR)


target_link_libraries(${EXE_FILE} GLEW ${OPENGL_gl_LIBRARY} glfw ${EMBREE_LIBRARY} ${ASSIMP_LIBRARIES} IlmImf Imath Half Iex IexMath IlmThread /home/bartosz/UWR/RCG/lib/libcommon.a)

