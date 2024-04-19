cmake_minimum_required(VERSION 3.11)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)
FetchContent_Declare(ftxui-grid-container
  GIT_REPOSITORY https://github.com/mingsheng13/grid-container-ftxui
  GIT_TAG master
)
FetchContent_MakeAvailable(ftxui-grid-container)

project(ftxui-starter LANGUAGES CXX VERSION 1.0.0)
add_executable(ftxui-starter src/main.cpp)
target_link_libraries(ftxui-starter
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE ftxui-grid-container
)