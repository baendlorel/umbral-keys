#pragma once
#include "config.h"
#define U8(x) reinterpret_cast<const char*>(u8##x)

void Config::OpenConfigFile() {
  path cwd = current_path();             // 获取当前工作目录
  path configPath = cwd / "config.txt";  // 拼接文件名
  string command = "notepad.exe " + configPath.string();
  //system(command.c_str());
  ShellExecute(NULL, L"open", configPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

unordered_map<WORD, Array<WORD>> Config::Load() {
  static const WCHAR* wh = L"Config::load";
  static const char* h = "Config::load";
  static auto oeditAndReload = [](ofstream& file, const path& filePath) {
    file.close();
    OpenConfigFile();
    return Load();
  };
  static auto ieditAndReload = [](ifstream& file, const path& filePath) {
    file.close();
    OpenConfigFile();
    return Load();
  };

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
          wh);
    } else {
      newFile << "\xEF\xBB\xBF";  // 写入 UTF-8 BOM
      newFile << U8("# UmbralKeys 配置（'#'开头表示注释）\n");
      newFile << U8(
          "# UmbralKeys key mapping configuration ('#' means comment) \n");
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
      Logger::MsgBox(I18N{
          L"config."
          L"txt已经新建，现将开启config."
          L"txt文件，请编辑后保存退出，影键会自动重试加载它",
          L"config.txt is created. Please open it and write key maps. "
          L"After saving and quit, UmbralKeys will try to load it again."});
      return oeditAndReload(newFile, configPath);
    }
  }

  ifstream file(configPath);
  if (!file.is_open()) {
    Logger::Abort(I18N{L"无法打开config.txt", L"Cannot open 'config.txt'"}, wh);
  }

  string line;
  unordered_map<WORD, Array<WORD>> umbralMap;
  size_t lineIdx = 0;
  while (getline(file, line)) {
    lineIdx++;
    erase_if(line, [](unsigned char c) { return std::isspace(c); });
    // 检查 UTF-8 BOM
    if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
      line = line.substr(3);  // 去掉 BOM
    }
    // 跳过空行和注释行
    if (line.at(0) == '#' || line.length() == 0) {
      continue;
    }

    size_t pos = line.find('=');
    if (pos == string::npos) {
      wstring zh =
          format(L"行数:{} 没有找到'='符号，解析config.txt失败，请修改后重试",
                 lineIdx);
      wstring en = format(
          L"Line:{} No '=' found, fail to load key map configuration. Please "
          L"edit config.txt and try again.",
          lineIdx);
      Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
      return ieditAndReload(file, configPath);
    }

    WORD origin = getKeyCode(line.substr(0, pos).c_str());
    Array<string> umbras = split(line.substr(pos + 1), '+');

    // 原按键必须存在
    if (origin == 0) {
      wstring u = stow(line.substr(0, pos));
      wstring zh = format(L"行数:{} 原按键无效：[{}]", lineIdx, u);
      wstring en = format(L"Line:{} Invalid origin key: [{}]", lineIdx, u);
      Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
      return ieditAndReload(file, configPath);
    }

    // 右侧不能有左侧的键、右侧按键必须存在
    for (size_t i = 0; i < umbras.getSize(); i++) {
      WORD umbral = getKeyCode(umbras[i].c_str());
      if (umbral == 0) {
        wstring u = stow(umbras[i]);
        wstring zh = format(L"行数:{} 影键无效：[{}]", lineIdx, u);
        wstring en = format(L"Line:{} Invalid umbral key: [{}]", lineIdx, u);
        Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
        return ieditAndReload(file, configPath);
      }
      if (origin == umbral) {
        wstring u = stow(umbras[i]);
        wstring zh =
            format(L"行数:{} 原按键和影键不能相同，两者都是:[{}]", lineIdx, u);
        wstring en = format(
            L"Line:{} Origin key and umbral key cannot be the same. For they "
            L"are both: [{}]",
            lineIdx, u);
        Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
        return ieditAndReload(file, configPath);
      }
    }

    // 右侧不能有重复的键
    for (size_t i = 0; i < umbras.getSize(); i++) {
      for (size_t j = i + 1; j < umbras.getSize(); j++) {
        if (umbras[i] == umbras[j]) {
          wstring u = stow(umbras[i]);
          wstring zh = format(L"行数:{} 影键重复：[{}]", lineIdx, u);
          wstring en =
              format(L"Line:{} Duplicate umbral key: [{}]", lineIdx, u);
          Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
          return ieditAndReload(file, configPath);
        }
      }
    }

    // 到这里认为配置是正常的

    // 整合umbras为WORD数组
    Array<WORD> umbralKeycodes(umbras.getSize());
    for (size_t i = 0; i < umbras.getSize(); i++) {
      WORD u = getKeyCode(umbras[i].c_str());
      umbralKeycodes.push(u);
    }

    if (origin != 0 && umbras.getSize() > 0) {
      umbralMap.emplace(origin, umbralKeycodes);
    }
  }

  if (umbralMap.size() == 0) {
    Logger::MsgBox(I18N{
        L"未加载到可用的键盘映射配置，现将开启config."
        L"txt文件，请编辑后保存退出，影键会自动重试加载它",
        L"No valid config found. Now opening config.txt. Please edit it, save "
        L"it then quit Notepad. UmbralKeys will try to load it again."});
    return ieditAndReload(file, configPath);
  }

  file.close();
  return umbralMap;
}
