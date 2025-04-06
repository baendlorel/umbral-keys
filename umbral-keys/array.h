#pragma once
#include <stdexcept>

template <typename T>
struct NativeArray {
  T *data;      // ԭ������ָ��
  size_t size;  // ��С
};

template <typename T>
class Array {
 private:
  T *data;          // ԭ������ָ��
  size_t capacity;  // ��������
  size_t size;      // ��ǰ��С

  // ��չ��������
  void expand();

 public:
  // ���캯��
  Array();

  // ��������
  ~Array();

  // ���Ԫ��
  void push(const T &value);

  // ɾ��ָ���±��Ԫ��
  void remove(size_t index);

  // �������Ԫ��
  void clear();

  // ��ȡ��ǰ����ĳ���
  size_t getSize() const;

  NativeArray<T> toNativeArray() const;

  // ��ȡָ��������Ԫ��
  T &operator[](size_t index);

  const T &operator[](size_t index) const;
};
