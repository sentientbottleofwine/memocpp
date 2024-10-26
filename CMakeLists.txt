cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG        1d40687a403b7babdafdd577d10e001840b0f12
)

FetchContent_MakeAvailable(ftxui)

project(memocpp)

add_executable(memocpp
  src/main.cpp
  src/memo.cpp
  src/cards.cpp
  src/utils.cpp
  src/player.cpp
  src/game_state.cpp
)
target_include_directories(memocpp
  PRIVATE src/include
)

target_link_libraries(memocpp
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)