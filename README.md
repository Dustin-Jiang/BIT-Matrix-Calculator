# [Matrix Calculator](https://github.com/Dusti-Jiang/BIT-Matrix-Calculator)

参加 [北理十行代码挑战赛](https://mp.weixin.qq.com/s?__biz=MjM5NjE3NjI1Mw==&mid=2651374572&idx=1&mpshare=1&scene=23&srcid=0411uN3WtCdJyhDJ7fzngW5X) 的简单作品。

实现了基本的界面, 矩阵的定义与使用，包含矩阵的表达式计算.


# Thirdparty Acknowledgement

- [ArthurSonzogni/FTXUI](https://github.com/ArthurSonzogni/FTXUI), C++ Functional Terminal User Interface, MIT License
- [表达式求值 - OI Wiki](https://oi-wiki.org/misc/expression/), 一元运算符与右结合的运算符, CC BY-SA 4.0 Deed & SATA License


# Requirements

- 经过测试的编译环境

  - `GCC 13.1.0`
  - `MSYS 2`
  - `CMake 3.26.4`
  - `ninja 1.1.1`

- 理论可行的编译环境

  - 支持 C++17 特性的编译器
  - `CMake 3.24+`
  - `Make / ninja`


# Build

## Windows MSYS2 / MinGW

在项目根目录下运行 `./scripts/build-target.ps1` 即可, 编译得到的二进制执行文件在 `/build` 文件夹下. 

## Windows MSVC

没装 Visual Studio, 不清楚喵

## Linux

~~话说这个 `.ps1` 文件内容和 `bash` 也基本一样吧~~

```bash
#!/usr/bin/bash
mkdir -p ./build
cd ./build

cmake ..
cmake --build .
```


# Structure

```
Matrix-Calculator
|
│   .gitignore
│   CMakeLists.txt
│   LICENSE
│   README.md
│
├───.vscode
│       c_cpp_properties.json
│       launch.json
│       settings.json
│       tasks.json
│
├───script
│       build-target.ps1
│       debug-target.ps1
│
├───src
│   │   main.cpp
│   │
│   ├───components
│   │       ButtonList.hpp
│   │       ButtonTiny.hpp
│   │       List.hpp
│   │       Text.hpp
│   │       ViewBase.hpp
│   │
│   └───model
│           Expression.hpp
│           Matrix.hpp
│           MatrixBase.hpp
│           MatrixIters.hpp
│           MatrixList.hpp
│           MatrixNode.hpp
│
│
└───test
        test.cpp
```

## src/components/

基本都是在 `FTXUI` 基础上糊的一些可重用组件. 

不得不说 `FTXUI` 这种在 C++ 基础上做的轻量 UI 项目, 它的本意是好的, 但是被 C++ 限制了太多, 想要一点代码拆分就要指针满天飞, 动不动就 `std::bad_malloc` 绝对不是什么好的开发体验; 同时其为了高效的终端输出, 还用了多线程, 调试就更难了, 没人想写个 `Hello World` 挂上 debugger 看见四个线程在那欢快地抛 exception 吧... 

从文件结构也可以看出, 一开始的时候是想极力靠近前端开发的 UI 结构的 (`components`, `model`, `view`, `viewModel`), 当然实际上这是不现实滴, 在Git log里还能看见删除`view`的几次commit. 


## src/main.cpp

主文件 400 行一把梭, 我们 `FTXUI` 真是太优雅辣! 

其实是因为蛋疼的变量生命周期问题, 6k stars 的 `FTXUI` 文档几乎就是阿卡林状态, ~~迫不得已~~ 懒得研究就把 `view` 文件夹删了, 把里面代码全粘贴 `main()` 里面了. 

> 我是 CV 工程师, <kbd>Ctrl</kbd>+<kbd>C</kbd> 的 <kbd>C</kbd>, <kbd>Ctrl</kbd>+<kbd>V</kbd> 的 <kbd>V</kbd>. 


## src/model/

```
Matrix.hpp
MatrixBase.hpp
MatrixIters.hpp
MatrixNode.hpp
```
这四个文件是最早开始写的, 搓出来了一个还算能看的`Matrix`类实现. 但是一开始拍脑袋决定的用二维链表存矩阵的天才想法让两天后的我每时每刻不想拍死自己. 好在封装的还算好看, 没有人会感受到里面的混乱🤣

- 我起了, 链表一把梭了
- 矩阵运算 `operator overload` 秒了
- 遍历不方便正好可以写 `iterator`
- 这链表删个行怎么要写100多行代码
- 哎呀没事虽然写出来的高斯消元 $O(n^3)$ ~~(可能还不止)~~ 但是这代码真好看吧仨 `if` 俩递归结束辣, 哦前提是封装起来的不算


## /test/test.cpp

这个文件也是最早写的, 配合测试 `Matrix` 类, 自己写的代码, 挂 `gdb` 都格外舒服. 


# Acknowledgement


## Github Copilot

虽然不太聪明, 但是写了很多重复的代码, ~~当然也可以说是代码臃肿的直接责任人~~, 养成了写代码时候等一等等着按 <kbd>Tab</kbd> 的坏习惯😤. 

## [Ri-Nai](https://github.com/Ri-Nai)

帮了我很多呀, 也给了我难以想象的支持和温柔呢. 没有你的话, 我怕是早就把这堆代码扔了吧. 

<div style="opacity: 0.1">好きだよ。</div>