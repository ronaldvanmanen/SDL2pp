cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SDLpp-tests")

add_executable("${PROJECT_NAME}"
  "global_fixture.cpp"
  "test_main.cpp"
  "test_texture.cpp")

find_package(boost_unit_test_framework REQUIRED CONFIG)
target_link_libraries("${PROJECT_NAME}"
  PRIVATE
  SDL2pp
  Boost::unit_test_framework)

add_test("${PROJECT_NAME}" "${PROJECT_NAME}")
