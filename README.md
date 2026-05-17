# Galgame Demo

这是一个用于 C++ / Qt 课程设计演示的 galgame 小样。项目结构按提交包整理为：

- 'src/'：源代码
- 'bin/'：编译后的可执行程序和可选素材目录
- 'Start.pro'：Qt Creator / qmake 工程文件

## 运行

bash
/Users/mikemao/Qt/6.11.1/macos/bin/qmake Start.pro -o qt-build/Makefile
make -C qt-build
open bin/GalgameDemo.app

当前版本包含：

- 主菜单
- 剧情阅读界面
- Start / Continue
- Skip 文本快进
- Backlog 回想
- ED 页面：黑屏渐变白屏，并播放 'assets/music/ed.mp3'
- 选择支界面
- 存档 / 读档界面
- 设置界面
- CG 鉴赏
- 音乐回廊

## 素材

演示素材放在 'assets/'：

- 背景：'background.png' / 'background.jpg' / 'bg.png' / 'bg.jpg'
- 立绘：'hero.png' / 'character.png' / 'stand.png'
- ED 黑屏图：'picture/ed_black.png'
- ED 白屏图：'picture/ed_white.png'
- ED 音乐：'music/ed.mp3'

如果没有素材，程序会显示临时占位画面。

代码使用 Qt Widgets 编写，主要用到 'QMainWindow'、'QWidget'、'QPushButton'、'QLabel'、'QPainter'、信号槽等基础内容。说明为“基于 C++ / Qt Widgets 的视觉小说 UI 演示程序”。
