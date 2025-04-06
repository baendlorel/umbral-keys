#pragma once
#include "logger.h"
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <format>
#include <unordered_map>
#include <windows.h>

using namespace std;

class Config {
public:
  static unordered_map<string, NativeArray<string>> load() {
    static const WCHAR *h = L"Config::load";

    filesystem::path currentPath =
        filesystem::current_path(); // ��ȡ��ǰ����Ŀ¼
    filesystem::path configPath = currentPath / "config.txt"; // ƴ���ļ���

    Logger::Log(format("configPath: {}", configPath.string()), "Config::load");

    if (!filesystem::exists(configPath)) {
      // ���������ļ�
      ofstream newFile(configPath);

      // ����ļ��Ƿ�ɹ���
      if (!newFile.is_open()) {
        Logger::Abort(
            I18N{L"�ڳ����ļ������½�config.txtʧ��",
                 L"Cannot create 'config.txt' (must be in same folder)"},
            h);
      } else {
        newFile.close();
        Logger::MsgBox(
            I18N{L"config."
                 L"txt�Ѿ��½����������д����ӳ�����á�д�걣�������"
                 L"�´�Ӱ��",
                 L"config.txt is created. Please open it and write key maps. "
                 L"After saving it, run UmbralKeys again."});
      }
    }

    ifstream file(configPath);
    if (!file.is_open()) {
      Logger::Abort(I18N{L"�޷���config.txt", L"Cannot open 'config.txt'"},
                    h);
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
      NativeArray<string> umbras = split(line.substr(pos + 1), '+');

      if (!origin.empty() && umbras.size > 0) {
        umbralMap[origin] = umbras;
      }
    }

    if (umbralMap.size() == 0) {
      Logger::Abort(
          I18N{L"δ���ص����õļ���ӳ������", L"No valid config found"}, h);
    }

    file.close();
    return umbralMap;
  }
};
