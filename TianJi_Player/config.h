#ifndef CONFIG_H
#define CONFIG_H

//- 定义应用程序的配置参数，例如默认音乐文件夹路径。

#include <QString>

// 音乐库文件夹的默认路径
const QString DefaultMusicLibraryPath = "./music/library";

// 播放列表文件的默认保存路径
const QString DefaultPlaylistPath = "./default/playlist.m3u";

// 歌词文件夹的默认路径
const QString DefaultLyricsPath = "./lyrics";

// 应用程序的其它相关设置
// ...

#endif // CONFIG_H
