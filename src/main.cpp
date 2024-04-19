#include <ftxui/component/component_options.hpp> // for ButtonOption
#include <ftxui/component/mouse.hpp>             // for ftxui
#include <functional>                            // for function
#include <memory>                                // for allocator, shared_ptr

#include "ftxui/component/component.hpp" // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp" // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT

#include "components/ButtonList.hpp"
#include "components/ButtonTiny.hpp"
#include "components/MatrixEditor.hpp"
#include "components/List.hpp"
#include "components/Text.hpp"
#include "components/ViewBase.hpp"
#include "model/Matrix.hpp"
#include "model/MatrixList.hpp"
// #include "view/MatrixEditView.hpp"

using namespace ftxui;

auto button_style = ButtonOption::Animated();

int main(int argc, const char *argv[]) {
  auto screen = ScreenInteractive::TerminalOutput();

  // State of the application:
  bool matrix_define_view_shown = false;
  bool matrix_edit_view_shown = false;
  std::vector<std::string> debug = {};

  // MatrixDefineView
  int height = 0, width = 0;
  std::string height_v = "", width_v = "";
  std::vector<std::vector<std::shared_ptr<double>>> m{};

  // MatrixEditView
  std::vector<std::vector<std::shared_ptr<std::string>>> matrix_editor_ctx{};

  // Some actions modifying the state:
  int tab_matrix_define_selected = 0;
  auto show_matrix_define_view = [&] {
    matrix_editor_ctx = std::vector<std::vector<std::shared_ptr<std::string>>> {};
    height_v = "", width_v = "";
    height = 0, width = 0;
    m = std::vector<std::vector<std::shared_ptr<double>>>{};

    matrix_define_view_shown = true;
  };
  auto hide_matrix_define_view = [&] { matrix_define_view_shown = false; };

  auto show_matrix_edit_view = [&] {
    debug.push_back(std::to_string(m.size()) + " " + std::to_string(m[0].size()));
    matrix_editor_ctx = std::vector<std::vector<std::shared_ptr<std::string>>>{};

    for (auto i : m) {
      std::vector<std::shared_ptr<std::string>> row;
      for (auto j : i) {
        row.push_back(std::make_shared<std::string>(std::to_string(*j)));
      }
      matrix_editor_ctx.push_back(row);
    }

    matrix_edit_view_shown = true;

    debug.push_back("Line 64 " + std::to_string(reinterpret_cast<std::uintptr_t>(&m)));
    debug.push_back("Line 65 " + std::to_string(matrix_editor_ctx.size()) + " " + std::to_string(matrix_editor_ctx[0].size()));
  };
  auto hide_matrix_edit_view = [&] { matrix_edit_view_shown = false; };

  auto exit = screen.ExitLoopClosure();

  // Instanciate the main and modal components:
  std::vector<std::string> tab_values{
      "定义/编辑",
      "tab_2",
      "tab_3",
  };
  int tab_selected = 0;
  auto tab_toggle = Toggle(&tab_values, &tab_selected);

  auto matrix_list = MatrixList{
    {
      Matrix {3,3}
    },
  };
  auto tab_matrix_define_action = std::function<bool(Event event)>([&](Event event)
  {
    debug.push_back("Line 84 " + std::to_string(event.mouse().button));
    debug.push_back("Line 85 " + std::to_string(tab_matrix_define_selected));
    if (
      (event == Event::Return) ||
      (event.is_mouse() && event.mouse().motion == event.mouse().Released && event.mouse().button == Mouse::Left)
    ) {
      debug.push_back("Line 92 " + std::to_string(matrix_list.size()) + " " + std::to_string(tab_matrix_define_selected));
      if (tab_matrix_define_selected == matrix_list.size())
      {
        show_matrix_define_view();
      }
      else
      {
        m = matrix_list.get(tab_matrix_define_selected);
        show_matrix_edit_view();
      }
    }
    return false;
  });
  auto tab_matrix_define_entries = matrix_list.entries();
  auto edit_matrix = [&](std::vector<std::vector<double>> *v) {
    matrix_list.add(v);
  };

  std::vector<std::string> tab_2_entries{
      "Hello",
      "Hi",
      "Hay",
  };
  int tab_2_selected = 0;

  std::vector<std::string> tab_3_entries{
      "Table",
      "Nothing",
      "Is",
      "Empty",
  };
  int tab_3_selected = 0;

  auto tab_container = Container::Tab(
      {
          Menu(&tab_matrix_define_entries, &tab_matrix_define_selected) | CatchEvent(tab_matrix_define_action),
          Radiobox(&tab_2_entries, &tab_2_selected),
          Radiobox(&tab_3_entries, &tab_3_selected),
      },
      &tab_selected);

  auto main_component = Container::Vertical({
      tab_toggle,
      tab_container,
  });
  main_component = Renderer(main_component, [&] {
    return vbox({
        tab_toggle->Render(),
        separator(),
        tab_container->Render(),
        separator(),
        List(debug)->Render() | size(HEIGHT, LESS_THAN, 20)
    });
  });
  main_component = ViewBase(main_component, "MainView");



  // MatrixEditView
  auto push_matrix = [&]() {
    debug.push_back("Line 130 " + std::to_string(m.size()) + " " + std::to_string(m[0].size()));
    matrix_list.add(&m);
    tab_matrix_define_entries = matrix_list.entries();
    debug.push_back("Line 133 " + std::to_string(matrix_list.size()));
    debug.push_back("Line 134 " + tab_matrix_define_entries[0]);

    m = std::vector<std::vector<std::shared_ptr<double>>>{};
    hide_matrix_edit_view();
  };

  auto matrix_editor_comfirm_button = ButtonTiny("添加矩阵", push_matrix);
  auto matrix_edit_view_component = MatrixEditor(&matrix_editor_ctx);
  // auto matrix_edit_view_component = Renderer([&] {
  //   return vbox({
  //     MatrixEditor(&matrix_editor_ctx)->Render(), 
  //     text([&]
  //     {
  //       return "MatrixEditView " + std::to_string(matrix_editor_ctx.size()) + " " + std::to_string(matrix_editor_ctx[0].size());
  //     }())
  //   });
  // });
  matrix_edit_view_component = Container::Vertical({
    matrix_edit_view_component, matrix_editor_comfirm_button
  });
  auto matrix_edit_view = ViewBase(matrix_edit_view_component, "编辑矩阵");



  // MatrixDefineView
  auto set_size = [&] {
    height = std::stoi(height_v);
    width = std::stoi(width_v);
    for (int i = 0; i < height; i++) {
      std::vector<std::shared_ptr<double>> row{};
      for (int j = 0; j < width; j++) {
        row.push_back(std::make_shared<double>(0.0));
      }
      m.push_back(row);
    }
    debug.push_back([&] {
      std::string res;
      if (m.size() == 0) {
        return std::string{"Empty"};
      }
      for (auto i = m[0].begin(); i != m[0].end(); ++i) {
        res += std::to_string(**i) + " ";
      }
      return res;
    }());
    hide_matrix_define_view();
    show_matrix_edit_view();
  };

  auto matrix_define_view_component = Container::Vertical({
      Container::Vertical({
          Text("定义矩阵尺寸"),
          Container::Horizontal({
              Input(&width_v, {.placeholder = "宽度", .multiline = false}) |
                  underlined | flex | border,
              Input(&height_v, {.placeholder = "高度", .multiline = false}) |
                  underlined | flex | border,
              Button("确定", set_size, button_style) | flex,
          }),
      }),
      Text(&width_v),
      ButtonTiny("返回", hide_matrix_define_view),
  });

  auto matrix_define_view = ViewBase(matrix_define_view_component, "定义矩阵");



  main_component |= Modal(matrix_edit_view, &matrix_edit_view_shown);
  main_component |= Modal(matrix_define_view, &matrix_define_view_shown);

  screen.Loop(main_component);
  return 0;
};