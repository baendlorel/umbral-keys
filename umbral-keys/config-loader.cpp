#pragma once
#include "config-loader.h"

unordered_map<string, NativeArray<string>> Config::Load() {
  static const WCHAR *h = L"Config::load";

  filesystem::path cwd = filesystem::current_path();  // 获取当前工作目录
  filesystem::path configPath = cwd / "config.txt";   // 拼接文件名

  Logger::Log(format("configPath: {}", configPath.string()), "Config::load");

  if (!filesystem::exists(configPath)) {
    // 创建并打开文件
    ofstream newFile(configPath);

    // 检查文件是否成功打开
    if (!newFile.is_open()) {
      Logger::Abort(
          I18N{L"在程序文件夹内新建config.txt失败",
               L"Cannot create 'config.txt' (must be in same folder)"},
          h);
    } else {
      newFile.close();
      Logger::MsgBox(
          I18N{L"config."
               L"txt已经新建，请打开它编写键盘映射配置。写完保存后再重"
               L"新打开影键",
               L"config.txt is created. Please open it and write key maps. "
               L"After saving it, run UmbralKeys again."});
    }
  }

  ifstream file(configPath);
  if (!file.is_open()) {
    Logger::Abort(I18N{L"无法打开config.txt", L"Cannot open 'config.txt'"}, h);
  }

  string line;
  unordered_map<string, NativeArray<string>> umbralMap;
  while (getline(file, line)) {
    erase_if(line, ::isspace);
    size_t pos = line.find('=');
    if (pos == string::npos) {
      continue;
    }

    string origin = line.substr(0, pos);
    NativeArray<string> umbras = Util::Split(line.substr(pos + 1), '+');

    if (!origin.empty() && umbras.size > 0) {
      umbralMap[origin] = umbras;
    }
  }

  if (umbralMap.size() == 0) {
    Logger::Abort(I18N{L"未加载到可用的键盘映射配置", L"No valid config found"},
                  h);
  }

  file.close();
  return umbralMap;
}
