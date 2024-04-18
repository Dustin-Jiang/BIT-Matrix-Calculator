#include <ftxui/component/component_options.hpp> // for ButtonOption
#include <ftxui/component/mouse.hpp>             // for ftxui
#include <functional>                            // for function
#include <memory>                                // for allocator, shared_ptr

#include "ftxui/component/component.hpp" // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp" // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT

#include "view/AlertView.hpp"
#include "view/MainView.hpp"

using namespace ftxui;

auto button_style = ButtonOption::Animated();

int main(int argc, const char *argv[]) {
  auto screen = ScreenInteractive::Fullscreen();

  // State of the application:
  bool modal_shown = false;

  // Some actions modifying the state:
  auto show_modal = [&] { modal_shown = true; };
  auto hide_modal = [&] { modal_shown = false; };
  auto exit = screen.ExitLoopClosure();
  auto do_nothing = [&] {};

  // Instanciate the main and modal components:
  auto main_component = MainComponent(show_modal, exit);
  auto modal_component = ModalComponent(do_nothing, hide_modal);

  // Use the `Modal` function to use together the main component and its modal
  // window. The |modal_shown| boolean controls whether the modal is shown or
  // not.
  main_component |= Modal(modal_component, &modal_shown);

  screen.Loop(main_component);
  return 0;
}