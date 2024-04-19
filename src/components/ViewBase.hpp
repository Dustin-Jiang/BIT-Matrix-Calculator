#ifndef COMPONENTS_VIEW_BASE
#define COMPONENTS_VIEW_BASE

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <functional>
#include <string>

using namespace ftxui;

Component ViewBase(Component &inner, std::string title)
{
  WindowOptions option = {
    inner,
    title,
    0,
    0,
    Dimension::Full().dimx,
    Dimension::Full().dimy
  };
  return Window(option);
}

#endif