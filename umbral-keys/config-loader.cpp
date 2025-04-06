#pragma once
#include "config-loader.h"

void openNotepad(const path& filePath) {
  string command = "notepad.exe " + filePath.string();
  system(command.c_str());
}

unordered_map<string, Array<string>> LoadConfig() {

  static const WCHAR* h = L"Config::load";

  path cwd = current_path();  // ��ȡ��ǰ����Ŀ¼
  path configPath = cwd / "config.txt";   // ƴ���ļ���

  Logger::Log(format("configPath: {}", configPath.string()), "Config::load");

  if (!exists(configPath)) {
    // ���������ļ�
    ofstream newFile(configPath);

    // ����ļ��Ƿ�ɹ���
    if (!newFile.is_open()) {
      Logger::Abort(
          I18N{L"�ڳ����ļ������½�config.txtʧ��",
               L"Cannot create 'config.txt' (must be in same folder)"},
          h);
    } else {
      newFile << "# UmbralKeys ���ã�'#'��ͷ��ʾע�ͣ�\n";
      newFile << "# UmbralKeys key mapping configuration \n";
      newFile << "# \n";
      newFile << "# ��ʽ��origin=umbral1+umbral2+...\n";
      newFile << "# Format��origin=umbral1+umbral2+...\n";
      newFile << "# \n";
      newFile << "# ��Сд���ǿ��Ե�\n";
      newFile << "# Upper case or lower case matters not \n";
      newFile << "# \n";
      newFile << "# ��ͬ�����ͼӺ�֮����пո�Ҳ�ǿ��Ե�\n";
      newFile << "# Spaces between key names and plus signs are allowed \n";
      newFile << "# \n";
      newFile << "# �·����������ð����ȼ�\n";
      newFile << "# Two configurations bellow are equivalent\n";
      newFile << "# \n";
      newFile << "# capslock=ctrl+space\n";
      newFile << "# CapsLock = Ctrl + Space\n";
      newFile.close();
      Logger::MsgBox(
          I18N{L"config."
               L"txt�Ѿ��½����������д����ӳ�����á�д�걣�������"
               L"�´�Ӱ��",
               L"config.txt is created. Please open it and write key maps. "
               L"After saving it, run UmbralKeys again."});
      // �򿪼��±�����������ӽ��̣��༭��ɺ�Ҫ����LoadConfig
      openNotepad(configPath);
      return LoadConfig();
    }
  }

  ifstream file(configPath);
  if (!file.is_open()) {
    Logger::Abort(I18N{L"�޷���config.txt", L"Cannot open 'config.txt'"}, h);
  }

  string line;
  unordered_map<string, Array<string>> umbralMap;
  while (getline(file, line)) {
    erase_if(line, ::isspace);

    // �������к�ע����
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
        L"δ���ص����õļ���ӳ�����ã��ֽ�����config."
        L"txt�ļ�����༭�󱣴��˳���Ӱ�����Զ����Լ�����",
        L"No valid config found. Now opening config.txt. Please edit it, save "
        L"it then quit Notepad. UmbralKeys will try to load it again."});
    // �򿪼��±�����������ӽ��̣��༭��ɺ�Ҫ����LoadConfig
    openNotepad(configPath);
    // �ȹ��������¼���
    file.close();
    return LoadConfig();
  }

  file.close();
  return umbralMap;
}
