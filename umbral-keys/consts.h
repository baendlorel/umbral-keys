#pragma once
#define N L"\n"

// �����Ϣ
#define UK_VER L"1.0.0"
#define UK_AUTHOR L"KasukabeTsumugi / futami16237@gmail.com"
#define UK_LICENSE L"MIT"
#define UK_DESC_ZH L"Ӱ����UmbralKeys���Ǽ���һ��ӳ�乤�ߣ�������ӳ�䵽���"
#define UK_DESC_EN                                                            \
  L"UmbralKeys is a key mapping tool. Allows you to map one key to multiple " \
  L"keys."
#define UK_STAT_ZH \
  L"���������������ѧϰʹ�ã���ֹ���ڷǷ�Ŀ�ġ����а�Ȩ��ԭ�������С�"
#define UK_STAT_EN                                                           \
  L"For communication and learning purposes only. It is prohibited for any " \
  L"illegal use. All copyrights belong to the original author."
#define UK_CREATE_DT L"����ʱ�䣨Create Date��: 2025-04-07"
#define UK_UPDATE_DT L"����ʱ�䣨Update Date��: 2025-04-07"

constexpr const wchar_t* UK_ABOUT_ZH =
    L"�汾��" UK_VER N L"���ߣ�" UK_AUTHOR N L"��ɣ�" UK_LICENSE N N UK_DESC_ZH
        N UK_STAT_ZH N
            N L"����ʱ�䣺" UK_CREATE_DT N L"����ʱ�䣺" UK_UPDATE_DT;

constexpr const wchar_t* UK_ABOUT_EN =
    L"Version: " UK_VER N L"Author: " UK_AUTHOR N L"License: " UK_LICENSE N N
        UK_DESC_EN N UK_STAT_EN N
            N L"Create Date: " UK_CREATE_DT N L"Update Date: " UK_UPDATE_DT;

// config.txtĬ������
 // "\xEF\xBB\xBF" \ 
#define SUPPORTED "0-9, A-Z, f1-12, enter, esc, space, tab, shift, "\
    "ctrl, alt, capslock, backspace, delete, insert, home, end, pageup, pagedown, "\
    "left, right, up, down\n"
constexpr const char* UK_CONFIG_ZH =
    "# UmbralKeys ���ã�'#'��ͷ��ʾע�ͣ�\n"
    "# ��ʽ��origin=umbral1+umbral2+...\n"
    "# ��Сд���ǿ��Ե�\n"
    "# ��ͬ�����ͼӺ�֮����пո�Ҳ�ǿ��Ե�\n"
    "# �Ⱥ����ֻ����һ����\n"
    "# �Ⱥ��ұ߲������ظ��ļ�\n"
    "# ֧�ֵİ�����ֱ�Ӱ����Ҳ���д����" SUPPORTED
    "# \n "
    "# �·����������ð����ȼ�\n"
    "# capslock=ctrl+space\n"
    "# CapsLock = Ctrl + Space";

constexpr const char* UK_CONFIG_EN =
    "# UmbralKeys key mapping configuration ('#' means comment) \n"
    "# Format��origin=umbral1+umbral2+...\n"
    "# Upper case or lower case matters not \n"
    "# Spaces between key names and plus signs are allowed \n"
    "# Only one key can be set on the left hand \n"
    "# Same keys on the right hand are not allowed \n"
    "# Supported keys (must spell as blow): " SUPPORTED
    "# \n "
    "# Two configurations bellow are equivalent\n"
    "# capslock=ctrl+space\n"
    "# CapsLock = Ctrl + Space";
#undef N
