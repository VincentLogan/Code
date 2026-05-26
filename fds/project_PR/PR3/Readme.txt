 程序运行说明
========================================

一、环境要求
----------------------------------------
操作系统：Windows / Linux / macOS
编译器：GCC (MinGW-w64 / TDM-GCC / MSYS2) 或 Clang

二、编译方法
----------------------------------------

1. 打开终端（命令行窗口）：
   - Windows：PowerShell 或 cmd
   - Linux/macOS：Terminal

2. 进入代码所在目录：
   cd C:\你的代码路径

3. 编译程序：
   gcc fds3.c -o fds3.exe

   说明：
   - fds3.c 为源代码文件名
   - -o fds3.exe 指定输出可执行文件名

三、运行方法
----------------------------------------

方法1：直接运行（手动输入数据）
   ./fds3.exe
   然后按照提示手动输入测试数据，输入完成后按 Ctrl+Z (Windows) 
   或 Ctrl+D (Linux/macOS) 结束输入。

方法2：从文件读取输入（推荐）
   将测试数据保存到 input.txt 文件中，然后执行：
   ./fds3.exe < input.txt

对于样例6，执行
cat test_max_1000.txt | ./fds3.exe
输出为Yes，成功