cmake_minimum_required(VERSION 3.8)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("cv_pc3")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(fmt REQUIRED)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")



# Agregue un origen al ejecutable de este proyecto.
add_executable(cv_pc3 ${SOURCES})

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cv_pc3 PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(cv_pc3 PRIVATE "${CMAKE_SOURCE_DIR}\\out\\build\\x64-debug\\vcpkg_installed\\x64-windows\\include")
target_link_libraries(cv_pc3 PRIVATE OpenGL::GL GLUT::GLUT fmt::fmt ${stb_INCLUDE_DIR})

target_compile_definitions(cv_pc3 PRIVATE SHADERS_DIR="${SHADERS_DIR}")