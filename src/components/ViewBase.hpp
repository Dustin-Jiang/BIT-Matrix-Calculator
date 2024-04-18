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
    .inner = inner,
    .title = title
  };
  return Window(option);
}

#endif