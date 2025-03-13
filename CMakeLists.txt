cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SDL2pp")

add_library("SDL2pp"
  "src/argb8888.cpp"
  "src/color.cpp"
  "src/error.cpp"
  "src/event.cpp"
  "src/events_subsystem.cpp"
  "src/renderer_flags.cpp"
  "src/renderer.cpp"
  "src/rgba8888.cpp"
  "src/sdl.cpp"
  "src/texture.cpp"
  "src/video_subsystem.cpp"
  "src/window_flags.cpp"
  "src/window.cpp")
    
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD 20)
endif()

find_package(boost_optional REQUIRED CONFIG)

target_link_libraries("${PROJECT_NAME}" PRIVATE Boost::optional)

find_package(SDL2 REQUIRED CONFIG)

target_link_libraries("${PROJECT_NAME}"
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/samples")
