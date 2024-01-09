# 天机播放器

#### 介绍
天机播放器是一个音乐播放应用，采用 C++ 语言和 Qt 用于用户界面设计。这个应用程序允许用户播放音乐文件、控制播放并查看歌词。

## 功能
- 播放/暂停音乐
- 通过下一曲和上一曲按钮导航播放列表
- 调整音量
- 显示歌词并高亮当前行

## 系统要求
- Linux 操作系统
- Qt 5.14.2 使用 GCC 64位
- 需要安装 mplayer

## 安装
在 Linux 系统上运行天机播放器前，你需要安装 mplayer。通常可以通过你的发行版包管理器来完成，例如：
- Ubuntu: `sudo apt install mplayer`
- Fedora: `sudo dnf install mplayer`
- Arch Linux: `sudo pacman -S mplayer`
然后，将 `tianji-player` 文件夹复制到你的主目录中，并运行 `make` 命令来编译应用程序

安装完 mplayer 后，使用 Qt 创建者内置的编译器按照你的 Qt 创建者配置来编译天机播放器

## 编译
在 Qt 创建者中打开项目，使用 Desktop Qt 5.14.2 GCC 64bit 工具包配置项目，并在 Debug 模式下构建项目。

## 执行
运行编译后的 `tianji_player` 可执行文件启动播放器。

## 许可证
使用MIT开源许可协议

## 联系方式
如需支持或报告问题，请联系微信号：zero_GK397。记得备注哦，不然可能会被忽略