cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SDL2pp-samples")

add_executable("plasma_fractal" "plasma_fractal.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET "plasma_fractal" PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries("plasma_fractal" PRIVATE SDL2pp)
