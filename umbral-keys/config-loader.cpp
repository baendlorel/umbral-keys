#pragma once
#include "config-loader.h"
#define U8(x) reinterpret_cast<const char*>(u8##x)

void openNotepad(const path& filePath) {
  string command = "notepad.exe " + filePath.string();
  system(command.c_str());
}

unordered_map<string, Array<string>> LoadConfig() {
  static const WCHAR* h = L"Config::load";

  path cwd = current_path();             // 获取当前工作目录
  path configPath = cwd / "config.txt";  // 拼接文件名

  Logger::Log(format("configPath: {}", configPath.string()), "Config::load");

  if (!exists(configPath)) {
    // 创建并打开文件
    ofstream newFile(configPath);

    // 检查文件是否成功打开
    if (!newFile.is_open()) {
      Logger::Abort(
          I18N{L"在程序文件夹内新建config.txt失败",
               L"Cannot create 'config.txt' (must be in same folder)"},
          h);
    } else {
      newFile << "\xEF\xBB\xBF";  // 写入 UTF-8 BOM
      newFile << U8("# UmbralKeys 配置（'#'开头表示注释）\n");
      newFile << U8("# UmbralKeys key mapping configuration \n");
      newFile << U8("# \n");
      newFile << U8("# 格式：origin=umbral1+umbral2+...\n");
      newFile << U8("# Format：origin=umbral1+umbral2+...\n");
      newFile << U8("# \n");
      newFile << U8("# 大小写都是可以的\n");
      newFile << U8("# Upper case or lower case matters not \n");
      newFile << U8("# \n");
      newFile << U8("# 不同按键和加号之间带有空格也是可以的\n");
      newFile << U8("# Spaces between key names and plus signs are allowed \n");
      newFile << U8("# \n");
      newFile << U8("# 下方的两个配置案例等价\n");
      newFile << U8("# Two configurations bellow are equivalent\n");
      newFile << U8("# \n");
      newFile << U8("# capslock=ctrl+space\n");
      newFile << U8("# CapsLock = Ctrl + Space\n");
      newFile.close();
      Logger::MsgBox(
          I18N{L"config."
               L"txt已经新建，现将开启config."
               L"txt文件，请编辑后保存退出，影键会自动重试加载它",
               L"config.txt is created. Please open it and write key maps. "
               L"After saving and quit, UmbralKeys will try to load it again."});
      // 打开记事本，但这个是子进程，编辑完成后还要重新LoadConfig
      openNotepad(configPath);
      // TODO 为何加了递归就报错中断？
      return LoadConfig();
    }
  }

  ifstream file(configPath);
  if (!file.is_open()) {
    Logger::Abort(I18N{L"无法打开config.txt", L"Cannot open 'config.txt'"}, h);
  }

  string line;
  unordered_map<string, Array<string>> umbralMap;
  while (getline(file, line)) {
    erase_if(line, ::isspace);

    // 跳过空行和注释行
    if (line.at(0) == '#' || line.length() == 0) {
      continue;
    }

    size_t pos = line.find('=');
    if (pos == string::npos) {
      continue;
    }

    string origin = line.substr(0, pos);
    Array<string> umbras = split(line.substr(pos + 1), '+');

    if (!origin.empty() && umbras.getSize() > 0) {
      umbralMap[origin] = umbras;
    }
  }

  if (umbralMap.size() == 0) {
    Logger::MsgBox(I18N{
        L"未加载到可用的键盘映射配置，现将开启config."
        L"txt文件，请编辑后保存退出，影键会自动重试加载它",
        L"No valid config found. Now opening config.txt. Please edit it, save "
        L"it then quit Notepad. UmbralKeys will try to load it again."});
    // 打开记事本，但这个是子进程，编辑完成后还要重新LoadConfig
    openNotepad(configPath);
    // 先关了再重新加载
    file.close();
    return LoadConfig();
  }

  file.close();
  return umbralMap;
}
