#pragma once
#define N L"\n"

// 软件信息
#define UK_VER L"1.0.0"
#define UK_AUTHOR L"KasukabeTsumugi / futami16237@gmail.com"
#define UK_LICENSE L"MIT"
#define UK_DESC_ZH L"影键（UmbralKeys）是键盘一款映射工具，将单键映射到多键"
#define UK_DESC_EN                                                            \
  L"UmbralKeys is a key mapping tool. Allows you to map one key to multiple " \
  L"keys."
#define UK_STAT_ZH \
  L"本软件仅供交流与学习使用，禁止用于非法目的。所有版权归原作者所有。"
#define UK_STAT_EN                                                           \
  L"For communication and learning purposes only. It is prohibited for any " \
  L"illegal use. All copyrights belong to the original author."
#define UK_CREATE_DT L"诞生时间（Create Date）: 2025-04-07"
#define UK_UPDATE_DT L"更新时间（Update Date）: 2025-04-07"

constexpr const wchar_t* UK_ABOUT_ZH =
    L"版本：" UK_VER N L"作者：" UK_AUTHOR N L"许可：" UK_LICENSE N N UK_DESC_ZH
        N UK_STAT_ZH N
            N L"诞生时间：" UK_CREATE_DT N L"更新时间：" UK_UPDATE_DT;

constexpr const wchar_t* UK_ABOUT_EN =
    L"Version: " UK_VER N L"Author: " UK_AUTHOR N L"License: " UK_LICENSE N N
        UK_DESC_EN N UK_STAT_EN N
            N L"Create Date: " UK_CREATE_DT N L"Update Date: " UK_UPDATE_DT;

// config.txt默认内容
 // "\xEF\xBB\xBF" \ 
#define SUPPORTED "0-9, A-Z, f1-12, enter, esc, space, tab, shift, "\
    "ctrl, alt, capslock, backspace, delete, insert, home, end, pageup, pagedown, "\
    "left, right, up, down\n"
constexpr const char* UK_CONFIG_ZH =
    "# UmbralKeys 配置（'#'开头表示注释）\n"
    "# 格式：origin=umbral1+umbral2+...\n"
    "# 大小写都是可以的\n"
    "# 不同按键和加号之间带有空格也是可以的\n"
    "# 等号左边只能有一个键\n"
    "# 等号右边不能有重复的键\n"
    "# 支持的按键（直接按照右侧填写）：" SUPPORTED
    "# \n "
    "# 下方的两个配置案例等价\n"
    "# capslock=ctrl+space\n"
    "# CapsLock = Ctrl + Space";

constexpr const char* UK_CONFIG_EN =
    "# UmbralKeys key mapping configuration ('#' means comment) \n"
    "# Format：origin=umbral1+umbral2+...\n"
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
