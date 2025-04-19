cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SDL2pp-samples")

macro("add_sample" TARGET)
  cmake_parse_arguments(arg "" "" "SOURCES" ${ARGN})

  add_executable(${TARGET} ${arg_SOURCES})

  if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 20)
  endif()

  target_link_libraries(${TARGET} PRIVATE SDL2pp)
endmacro()

add_sample("plasma_fractal" SOURCES "plasma_fractal.cpp" "math.cpp" "stopwatch.cpp")
add_sample("tunnel_effect" SOURCES "tunnel_effect.cpp" "math.cpp" "stopwatch.cpp")
