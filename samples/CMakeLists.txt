cmake_minimum_required (VERSION 3.10)

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

find_package(boost_poly_collection REQUIRED CONFIG)

add_sample("plasma_fractal"
  SOURCES
  "plasma_fractal.cpp"
  "shared/math.cpp"
  "shared/stopwatch.cpp"
)

add_sample("sw_ray_tracer"
  SOURCES
  "sw_ray_tracer.cpp"
  "shared/math.cpp"
  "shared/rgb96f.cpp"
  "shared/stopwatch.cpp"
)

target_link_libraries("sw_ray_tracer" PRIVATE Boost::poly_collection)

add_sample("tunnel_effect"
  SOURCES
  "tunnel_effect.cpp"
  "shared/math.cpp"
  "shared/stopwatch.cpp"
)
