#pragma once
#include "utils.h"
#include "logger.h"
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

using namespace std;

class Config {
public:
  static unordered_map<string, NativeArray<string>> load() {
    filesystem::path currentPath =
        filesystem::current_path(); // ��ȡ��ǰ����Ŀ¼
    filesystem::path configPath = currentPath / "config.txt"; // ƴ���ļ���

    cout << "configPath: " << configPath << endl;

    if (!filesystem::exists(configPath)) {
      // ���������ļ�
      ofstream newFile(configPath);

      // ����ļ��Ƿ�ɹ���
      if (!newFile.is_open()) {
        Logger::Throw(L"Config::load",
            L"Cannot create 'config.txt' (which must be in same folder)");
      } else {
        newFile.close();
        cout << "[UmbralKeys Config::load] 'config.txt' created" << endl;
      }
    }

    ifstream file(configPath);
    if (!file.is_open()) {
      throw runtime_error("[UmbralKeys Config::load] Cannot open "
                          "'config.txt' (which must be in same folder)");
    }

    string rawLine;
    unordered_map<string, NativeArray<string>> umbralMap;
    while (getline(file, rawLine)) {
      string line = regex_replace(rawLine, regex("\\s+"), ""); // ȥ��ע��
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

