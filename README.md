# Wallpaper Engine Project
对于一些特别的情况并没有很好适配，但是其实只要添加让用户自己输入显示器参数的步骤即可解决，但是并不会对我的积累有任何帮助了，所以暂时停下来了。留个坑以后无聊时候可以填上
## Description for each folder

### 抛弃的开机自启动文件(the bat give up which was used to add to auto start menu)
本来是靠代码唤起bat文件执行到开机自启文件夹，但是太慢了，已放弃，换成注册表了

### WPEG (all release file place)
最后的release文件

### WallpaperEngine（Old file, useless）
C++ 简陋版一个程序管所有，判断文件格式

### WallpaperEngineStart（old file, useless）
开机自启的程序，比WallpaperEngine落后一个版本里面的漏掉的东西可参照WallpaperEngine修复，但是懒了，因为没必要，将抛弃它和WallpaperEngine，改用最后的UI版本。

### Controller
传递交互给壁纸(Control the input to wallpaper)
WASD,QWER,shift,crtl,enter,esc,left click,right click(+ctrl)

### UI
C#UI界面(The start program for user)

### WallpaperEngine_ForUI
The core of engine.

### WebkitChrome
which I write by the opensource of cefsharp.(webkit core browser)

[WebkitChrome](https://github.com/DuGuYifei/WebkitChrome)

### ffplay
a very good free tool

