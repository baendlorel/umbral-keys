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

// CW UK_ABOUT =
//    L"影键 UmbralKeys\n"
//    L"版本: 1.0.0\n"
//    L"作者: KasukabeTsumugi / futami16237@gmail.com\n"
//    L"许可: MIT\n"
//    L"描述 (ZH): 影键（UmbralKeys）是键盘一款映射工具，将单键映射到多键\n"
//    L"描述 (EN): UmbralKeys is a key mapping tool. Allows you to map one key
//    to multiple keys.\n" L"声明 (ZH):
//    本软件仅供交流与学习使用，禁止用于非法目的。所有版权归原作者所有。\n"
//    L"声明 (EN): For communication and learning purposes only. It is
//    prohibited for any illegal use. All copyrights belong to the original
//    author.";

constexpr const wchar_t* UK_ABOUT_ZH =
    L"版本：" UK_VER N L"作者：" UK_AUTHOR N L"许可：" UK_LICENSE N N UK_DESC_ZH
        N UK_STAT_ZH N
            N L"诞生时间：" UK_CREATE_DT N L"更新时间：" UK_UPDATE_DT;

constexpr const wchar_t* UK_ABOUT_EN =
    L"Version: " UK_VER N L"Author: " UK_AUTHOR N L"License: " UK_LICENSE N N
        UK_DESC_EN N UK_STAT_EN N
            N L"Create Date: " UK_CREATE_DT N L"Update Date: " UK_UPDATE_DT;

#undef N
