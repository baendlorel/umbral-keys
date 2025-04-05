#pragma once
#include "logger.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <windows.h>

using namespace std;

class Config {
public:
  static unordered_map<string, NativeArray<string>> load() {
    filesystem::path currentPath =
        filesystem::current_path(); // 获取当前工作目录
    filesystem::path configPath = currentPath / "config.txt"; // 拼接文件名

    cout << "configPath: " << configPath << endl;

    if (!filesystem::exists(configPath)) {
      // 创建并打开文件
      ofstream newFile(configPath);

      // 检查文件是否成功打开
      if (!newFile.is_open()) {
        Logger::Throw(L"Config::load",
                      L"Cannot create 'config.txt' (must be in same folder)");
      } else {
        newFile.close();
        cout << "[UmbralKeys Config::load] 'config.txt' created" << endl;
        Logger::MsgBox(L"config.txt已经新建，请打开它进行配置吧！");
      }
    }

    ifstream file(configPath);
    if (!file.is_open()) {
      Logger::Throw(L"Config::load", L"Cannot open 'config.txt'");
    }

    string rawLine;
    unordered_map<string, NativeArray<string>> umbralMap;
    while (getline(file, rawLine)) {
      string line = regex_replace(rawLine, regex("\\s+"), ""); // 去除注释
      size_t pos = line.find('=');
      if (pos == string::npos) {
        continue;
      }

      string origin = line.substr(0, pos);
      NativeArray<string> umbras = split(line.substr(pos + 1), '+');

      if (!origin.empty() && umbras.size > 0) {
        umbralMap[origin] = umbras;
      }
    }

    if (umbralMap.size() == 0) {
      throw runtime_error(
          "[UmbralKeys ConfigLoader::load] No valid config found!");
    }

    file.close();
    return umbralMap;
  }
};
