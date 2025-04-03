cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SDL2pp")

add_library("${PROJECT_NAME}"
  "sources/error.cpp"
  "sources/sdl.cpp"
  "sources/window_flags.cpp"
  "sources/window.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET "${PROJECT_NAME}" PROPERTY CXX_STANDARD 20)
endif()

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries("${PROJECT_NAME}"
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/samples")

add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
