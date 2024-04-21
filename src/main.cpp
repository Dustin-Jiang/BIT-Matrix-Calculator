#include <ftxui/component/component_options.hpp> // for ButtonOption
#include <ftxui/component/mouse.hpp>             // for ftxui
#include <functional>                            // for function
#include <memory>                                // for allocator, shared_ptr

#include "ftxui/component/component.hpp" // for Button, operator|=, Renderer, Vertical, Modal
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive, Component
#include "ftxui/dom/elements.hpp" // for operator|, separator, text, size, Element, vbox, border, GREATER_THAN, WIDTH, center, HEIGHT
#include "ftxui-grid-container/grid-container.hpp"

#include "components/ButtonList.hpp"
#include "components/ButtonTiny.hpp"
#include "components/List.hpp"
#include "components/Text.hpp"
#include "components/ViewBase.hpp"
#include "model/Expression.hpp"
#include "model/Matrix.hpp"
#include "model/MatrixList.hpp"

using namespace ftxui;

auto button_style = ButtonOption::Animated();

int main(int argc, const char *argv[]) {
  auto screen = ScreenInteractive::TerminalOutput();

  // State of the application:
  bool matrix_define_view_shown = false;
  bool matrix_edit_view_shown = false;
  std::vector<std::string> debug = {};
  std::vector<std::string> warning = {};

  // MatrixDefineView
  int height = 0, width = 0;
  std::string height_v = "", width_v = "";
  std::vector<std::vector<std::shared_ptr<double>>> m{};

  // MatrixEditView
  Component matrix_edit_view, matrix_editor_comfirm_button, matrix_editor_input_row;
  std::vector<std::vector<std::shared_ptr<std::string>>>  matrix_editor_ctx{};
  std::string matrix_editor_col_v = "";
  std::string matrix_editor_row_v = "";
  std::string matrix_editor_value_v = "";


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


  auto reset_matrix_edit_view_focus = [&]()
  {
    matrix_editor_input_row->TakeFocus();
  };
  auto show_matrix_edit_view = [&] {
    // debug.push_back(std::to_string(m.size()) + " " + std::to_string(m[0].size()));
    matrix_editor_ctx = std::vector<std::vector<std::shared_ptr<std::string>>>{};

    matrix_editor_col_v = "";
    matrix_editor_row_v = "";
    matrix_editor_value_v = "";

    for (auto i : m) {
      std::vector<std::shared_ptr<std::string>> row;
      for (auto j : i) {
        row.push_back(std::make_shared<std::string>(std::to_string(*j)));
      }
      matrix_editor_ctx.push_back(row);
    }

    matrix_edit_view_shown = true;
    reset_matrix_edit_view_focus();

    // debug.push_back("Line 65 " + std::to_string(matrix_editor_ctx.size()) + " " + std::to_string(matrix_editor_ctx[0].size()));
  };
  auto hide_matrix_edit_view = [&] { matrix_edit_view_shown = false; };

  auto exit = screen.ExitLoopClosure();

  // Instanciate the main and modal components:
  std::vector<std::string> tab_values{
      "定义/编辑",
      "计算",
      "关于",
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
    // debug.push_back("Line 84 " + std::to_string(event.mouse().button));
    // debug.push_back("Line 85 " + std::to_string(tab_matrix_define_selected));
    if (
      (event == Event::Return) ||
      (event.is_mouse() && event.mouse().motion == event.mouse().Released && event.mouse().button == Mouse::Left)
    ) {
      // debug.push_back("Line 92 " + std::to_string(matrix_list.size()) + " " + std::to_string(tab_matrix_define_selected));
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

  std::vector<std::string> tab_3_entries{
      "Table",
      "Nothing",
      "Is",
      "Empty",
  };
  int tab_3_selected = 0;



  // MatrixEditView
  auto set_matrix = [&]() {
    auto matrix = std::vector<std::vector<double>> {};
    for (auto i : m) {
      std::vector<double> row;
      for (auto j : i) {
        row.push_back(*j);
      }
      matrix.push_back(row);
    }

    matrix_list.set(tab_matrix_define_selected, Matrix{&matrix});

    tab_matrix_define_entries = matrix_list.entries();

    hide_matrix_edit_view();
  };

  auto edit_matrix = std::function<void ()>([&]() {
    auto reset_vals = [&]()
    {
      matrix_editor_row_v = "";
      matrix_editor_col_v = "";
      matrix_editor_value_v = "";
      reset_matrix_edit_view_focus();
    };

    if (matrix_editor_row_v == "" || matrix_editor_col_v == "" || matrix_editor_value_v == "")
    {
      reset_vals();
      return;
    }
    int matrix_edit_row = std::stoi(matrix_editor_row_v) - 1;
    int matrix_edit_col = std::stoi(matrix_editor_col_v) - 1;
    if (matrix_edit_row >= m.size() || matrix_edit_col >= m[0].size())
    {
      reset_vals();
      return;
    }
    double value = std::stod(matrix_editor_value_v);

    *m[matrix_edit_row][matrix_edit_col] = value;
    *matrix_editor_ctx[matrix_edit_row][matrix_edit_col] = std::to_string(value);

    reset_vals();
  });

  matrix_editor_comfirm_button = ButtonTiny("确定", set_matrix);
  auto matrix_editor_cancel_button = ButtonTiny("取消", hide_matrix_edit_view);
  matrix_editor_input_row = Input(&matrix_editor_row_v, { .placeholder = "第X行", .multiline = false, });
  auto matrix_editor_input_col = Input(&matrix_editor_col_v, { .placeholder = "第Y列", .multiline = false, });
  auto matrix_editor_input_value = Input(&matrix_editor_value_v, { .placeholder = "更改为", .multiline = false });

  auto matrix_editor_control = Container::Horizontal({
    matrix_editor_input_row | underlined | border,
    matrix_editor_input_col | underlined | border,
    matrix_editor_input_value | underlined | border,
    Button("修改", edit_matrix, button_style),
  });

  auto matrix_edit_view_component = Container::Vertical({
    matrix_editor_control,
    matrix_editor_comfirm_button,
    matrix_editor_cancel_button
  });

  matrix_edit_view_component = Renderer(matrix_edit_view_component, [&] {
    return vbox({
      [&]
      {
        std::vector<Components> matrix_edit_view_components;

        for (auto i : matrix_editor_ctx) {
          Components row;
          for (auto j : i) {
            row.push_back(Text(j.get()) | flex | border);
          }
          matrix_edit_view_components.push_back(row);
        }
        auto matrix_edit_view_component = GridContainer(matrix_edit_view_components);
        return matrix_edit_view_component;
      }()->Render(),
      filler(),
      separator(),
      text("编辑矩阵") | bold,
      matrix_editor_control->Render(),
      separator(),
      vbox({
        matrix_editor_comfirm_button->Render(),
        matrix_editor_cancel_button->Render(),
      })
    });
  });
  matrix_edit_view = ViewBase(matrix_edit_view_component, "查看矩阵");



  // MatrixDefineView
  auto set_size = [&] {
    height = std::stoi(height_v);
    width = std::stoi(width_v);

    if (height == 0 || width == 0)
    {
      height_v = "";
      width_v = "";
      return ;
    }

    auto matrix = std::vector<std::vector<double>> {};
    for (int i = 0; i < height; i++) {
      std::vector<std::shared_ptr<double>> row_p{};
      std::vector<double> row{};
      for (int j = 0; j < width; j++) {
        row_p.push_back(std::make_shared<double>(0.0));
        row.push_back(0.0);
      }
      m.push_back(row_p);
      matrix.push_back(row);
    }

    matrix_list.add(&matrix);
    tab_matrix_define_entries = matrix_list.entries();

    hide_matrix_define_view();
    show_matrix_edit_view();
  };

  auto matrix_define_view_component = Container::Vertical(
      {Container::Vertical({
           Text("定义矩阵尺寸"),
           Container::Horizontal({
               Input(&width_v, {.placeholder = "宽度", .multiline = false}) |
                   underlined | flex | border,
               Input(&height_v, {.placeholder = "高度", .multiline = false}) |
                   underlined | flex | border,
               Button("确定", set_size, button_style) | flex,
           }),
       }),
       ButtonTiny("返回", hide_matrix_define_view)});

  auto matrix_define_view = ViewBase(matrix_define_view_component, "定义矩阵");



  // Calc Panel
  auto tab_matrix_calc_v = std::string{""};
  auto tab_matrix_calc_r = Text("");
  auto matrix_calc = [&] { 
    try {
      auto result_c = Expr::calculate(tab_matrix_calc_v, matrix_list);
      warning.clear();
      if (result_c.isMatrix)
      {
        auto result = std::get<Matrix>(result_c.evaluate());
        debug.push_back(result.to_string());
        tab_matrix_calc_r = [&]
        {
          std::vector<Components> matrix_calc_result_components{};
          for (int i = 0; i < result.height; i++) {
            Components row;
            for (auto j = result.line_begin(i); j != result.line_end(); ++j) {
              row.push_back(Text(std::to_string(*j)) | flex | border);
            }
            matrix_calc_result_components.push_back(row);
          }
          auto matrix_edit_view_component = GridContainer(matrix_calc_result_components);
          return matrix_edit_view_component;
        }();
      }
      else
      {
        auto result = std::get<double>(result_c.evaluate());
        debug.push_back(std::to_string(result));
        tab_matrix_calc_r = Text(std::to_string(result));
      }
    }
    catch(const std::exception & ex)
    {
      warning.push_back(ex.what());
    }
  };
  auto tab_matrix_calc_comfirm = Button("计算", matrix_calc, button_style);
  auto tab_matrix_calc_input = Input(&tab_matrix_calc_v, { .on_enter = [&]
  {
    tab_matrix_calc_comfirm->TakeFocus();
  }});
  auto tab_matrix_calc_panel = Container::Horizontal({
    tab_matrix_calc_input,
    tab_matrix_calc_comfirm,
    tab_matrix_calc_r
  });
  tab_matrix_calc_panel = Renderer(tab_matrix_calc_panel, [&]()
  {
    return vbox({
      hbox(
        tab_matrix_calc_input->Render() | border | size(HEIGHT, GREATER_THAN, 5),
        separator(),
        tab_matrix_calc_comfirm->Render() | size(WIDTH, GREATER_THAN, 8) | size(HEIGHT, GREATER_THAN, 5) | yflex | center
      ),
      separator(),
      text("计算结果") | bold,
      tab_matrix_calc_r->Render()
    });
  });


  // MainView
  auto tab_container = Container::Tab(
      {
          Menu(&tab_matrix_define_entries, &tab_matrix_define_selected) | CatchEvent(tab_matrix_define_action),
          tab_matrix_calc_panel,
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
        filler(),
        List(warning)->Render() | size(HEIGHT, LESS_THAN, 5) | color(Color::Red)
    });
  });
  main_component = ViewBase(main_component, "矩阵计算器");

  main_component |= Modal(matrix_edit_view, &matrix_edit_view_shown);
  main_component |= Modal(matrix_define_view, &matrix_define_view_shown);

  screen.Loop(main_component);
  return 0;
};