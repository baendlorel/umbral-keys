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
#define UK_CREATE_DT L"����ʱ�䣨Create Date��: 2025��4��7��"
#define UK_UPDATE_DT L"����ʱ�䣨Update Date��: 2025��4��7��"

// CW UK_ABOUT =
//    L"Ӱ�� UmbralKeys\n"
//    L"�汾: 1.0.0\n"
//    L"����: KasukabeTsumugi / futami16237@gmail.com\n"
//    L"���: MIT\n"
//    L"���� (ZH): Ӱ����UmbralKeys���Ǽ���һ��ӳ�乤�ߣ�������ӳ�䵽���\n"
//    L"���� (EN): UmbralKeys is a key mapping tool. Allows you to map one key
//    to multiple keys.\n" L"���� (ZH):
//    ���������������ѧϰʹ�ã���ֹ���ڷǷ�Ŀ�ġ����а�Ȩ��ԭ�������С�\n"
//    L"���� (EN): For communication and learning purposes only. It is
//    prohibited for any illegal use. All copyrights belong to the original
//    author.";

constexpr const wchar_t* UK_ABOUT = L"�汾��Version����" UK_VER N L"���ߣ�Author����" UK_AUTHOR
        N L"��ɣ�License����" UK_LICENSE N N UK_DESC_ZH N UK_STAT_ZH N
            UK_DESC_EN N UK_STAT_EN N N UK_CREATE_DT N UK_UPDATE_DT;

#undef N
