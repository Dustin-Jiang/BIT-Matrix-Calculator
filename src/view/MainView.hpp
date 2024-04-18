#ifndef VIEW_MAIN_VIEW_HPP
#define VIEW_MAIN_VIEW_HPP

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <functional>
#include <string>

#include "../components/Dialog.hpp"
#include "../components/ViewBase.hpp"

using namespace ftxui;

Component MainComponent(
  std::function<void()> show_modal,
  std::function<void()> exit
) 
{
  auto component = Container::Vertical({
      Button("Show modal", show_modal) | color(Color::Default),
      Button("Quit", exit) | color(Color::Default),
  });

  return ViewBase(component, "MainView");
}

#endif