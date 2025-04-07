#pragma once
#include "config.h"
#define U8(x) reinterpret_cast<const char*>(u8##x)

void Config::OpenConfigFile() {
  path cwd = current_path();             // ��ȡ��ǰ����Ŀ¼
  path configPath = cwd / "config.txt";  // ƴ���ļ���
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

  path cwd = current_path();             // ��ȡ��ǰ����Ŀ¼
  path configPath = cwd / "config.txt";  // ƴ���ļ���

  Logger::Log(format("configPath: {}", configPath.string()), "Config::load");

  if (!exists(configPath)) {
    // ���������ļ�
    ofstream newFile(configPath);

    // ����ļ��Ƿ�ɹ���
    if (!newFile.is_open()) {
      Logger::Abort(
          I18N{L"�ڳ����ļ������½�config.txtʧ��",
               L"Cannot create 'config.txt' (must be in same folder)"},
          wh);
    } else {
      newFile << "\xEF\xBB\xBF";  // д�� UTF-8 BOM
      newFile << U8("# UmbralKeys ���ã�'#'��ͷ��ʾע�ͣ�\n");
      newFile << U8(
          "# UmbralKeys key mapping configuration ('#' means comment) \n");
      newFile << U8("# \n");
      newFile << U8("# ��ʽ��origin=umbral1+umbral2+...\n");
      newFile << U8("# Format��origin=umbral1+umbral2+...\n");
      newFile << U8("# \n");
      newFile << U8("# ��Сд���ǿ��Ե�\n");
      newFile << U8("# Upper case or lower case matters not \n");
      newFile << U8("# \n");
      newFile << U8("# ��ͬ�����ͼӺ�֮����пո�Ҳ�ǿ��Ե�\n");
      newFile << U8("# Spaces between key names and plus signs are allowed \n");
      newFile << U8("# \n");
      newFile << U8("# �·����������ð����ȼ�\n");
      newFile << U8("# Two configurations bellow are equivalent\n");
      newFile << U8("# \n");
      newFile << U8("# capslock=ctrl+space\n");
      newFile << U8("# CapsLock = Ctrl + Space\n");
      Logger::MsgBox(I18N{
          L"config."
          L"txt�Ѿ��½����ֽ�����config."
          L"txt�ļ�����༭�󱣴��˳���Ӱ�����Զ����Լ�����",
          L"config.txt is created. Please open it and write key maps. "
          L"After saving and quit, UmbralKeys will try to load it again."});
      return oeditAndReload(newFile, configPath);
    }
  }

  ifstream file(configPath);
  if (!file.is_open()) {
    Logger::Abort(I18N{L"�޷���config.txt", L"Cannot open 'config.txt'"}, wh);
  }

  string line;
  unordered_map<WORD, Array<WORD>> umbralMap;
  size_t lineIdx = 0;
  while (getline(file, line)) {
    lineIdx++;
    erase_if(line, [](unsigned char c) { return std::isspace(c); });
    // ��� UTF-8 BOM
    if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
      line = line.substr(3);  // ȥ�� BOM
    }
    // �������к�ע����
    if (line.at(0) == '#' || line.length() == 0) {
      continue;
    }

    size_t pos = line.find('=');
    if (pos == string::npos) {
      wstring zh =
          format(L"����:{} û���ҵ�'='���ţ�����config.txtʧ�ܣ����޸ĺ�����",
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

    // ԭ�����������
    if (origin == 0) {
      wstring u = stow(line.substr(0, pos));
      wstring zh = format(L"����:{} ԭ������Ч��[{}]", lineIdx, u);
      wstring en = format(L"Line:{} Invalid origin key: [{}]", lineIdx, u);
      Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
      return ieditAndReload(file, configPath);
    }

    // �Ҳ಻�������ļ����Ҳఴ���������
    for (size_t i = 0; i < umbras.getSize(); i++) {
      WORD umbral = getKeyCode(umbras[i].c_str());
      if (umbral == 0) {
        wstring u = stow(umbras[i]);
        wstring zh = format(L"����:{} Ӱ����Ч��[{}]", lineIdx, u);
        wstring en = format(L"Line:{} Invalid umbral key: [{}]", lineIdx, u);
        Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
        return ieditAndReload(file, configPath);
      }
      if (origin == umbral) {
        wstring u = stow(umbras[i]);
        wstring zh =
            format(L"����:{} ԭ������Ӱ��������ͬ�����߶���:[{}]", lineIdx, u);
        wstring en = format(
            L"Line:{} Origin key and umbral key cannot be the same. For they "
            L"are both: [{}]",
            lineIdx, u);
        Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
        return ieditAndReload(file, configPath);
      }
    }

    // �Ҳ಻�����ظ��ļ�
    for (size_t i = 0; i < umbras.getSize(); i++) {
      for (size_t j = i + 1; j < umbras.getSize(); j++) {
        if (umbras[i] == umbras[j]) {
          wstring u = stow(umbras[i]);
          wstring zh = format(L"����:{} Ӱ���ظ���[{}]", lineIdx, u);
          wstring en =
              format(L"Line:{} Duplicate umbral key: [{}]", lineIdx, u);
          Logger::MsgBox(I18N{zh.c_str(), en.c_str()});
          return ieditAndReload(file, configPath);
        }
      }
    }

    // ��������Ϊ������������

    // ����umbrasΪWORD����
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
        L"δ���ص����õļ���ӳ�����ã��ֽ�����config."
        L"txt�ļ�����༭�󱣴��˳���Ӱ�����Զ����Լ�����",
        L"No valid config found. Now opening config.txt. Please edit it, save "
        L"it then quit Notepad. UmbralKeys will try to load it again."});
    return ieditAndReload(file, configPath);
  }

  file.close();
  return umbralMap;
}
