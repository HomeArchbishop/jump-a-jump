# 跳一跳 <sup>WIP</sup>

- 暂仅支持 Windows 平台。如使用其他平台，需重新实现部分模块。
- GCC-6.3.0-1

## Features
- 命令行界面，像素风格

## Compile

```shell
$ mkdir build
$ cd build
$ cmake .. -G "Unix Makefiles"
$ make
```

Then you will get an executable file `build/jump-a-jump.exe`.

## Run

- 请确保 shell 不要使用 cmd.exe, 推荐使用 Zsh / Git Bash / Powershell 等，以满足打印需要。

- 请确保你的终端窗口足够大，以满足打印需要。

- 运行编译得到的可执行文件：
  ```shell
  $ ./jump-a-jump.exe
  ```

- 游戏操作键位如下 (请确保小写键入)：
  - `s` 主页下开始新游戏 (**s**tart)；
  - `a` 主页下开始新自动游戏；游戏结束后开始新自动游戏 (start **a**utomatic game)；
  - `(空格)` 游戏过程中长按蓄力；
  - `r` 游戏结束后直接开始新的对局 (**r**estart)；
  - `h` 游戏结束后回到主页 (**h**ome)；
  - `q` 主页下退出程序；游戏过程中退出程序；游戏结束时退出程序 (**q**uit)；

  注：一般情况下未进行键盘监听，退出程序请依照实际情况发出终止信号，一般是 `Ctrl + c`。

## TODOS

- 添加界面边框
- 改进配色方案
- 完善界面说明
- 适配 linux 等平台
- 添加彩蛋、细化加分规则
- a lot...
