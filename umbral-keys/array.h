#pragma once
#include <stdexcept>

template <typename T>
struct NativeArray {
  T *data;      // 原生数组指针
  size_t size;  // 大小
};

template <typename T>
class Array {
 private:
  T *data;          // 原生数组指针
  size_t capacity;  // 数组容量
  size_t size;      // 当前大小

  // 扩展数组容量
  void expand();

 public:
  // 构造函数
  Array();

  // 析构函数
  ~Array();

  // 添加元素
  void push(const T &value);

  // 删除指定下标的元素
  void remove(size_t index);

  // 清空所有元素
  void clear();

  // 获取当前数组的长度
  size_t getSize() const;

  NativeArray<T> toNativeArray() const;

  // 获取指定索引的元素
  T &operator[](size_t index);

  const T &operator[](size_t index) const;
};
