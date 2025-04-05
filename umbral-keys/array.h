#pragma once
#include <iostream>

template <typename T> struct NativeArray {
  T *data; // 原生数组指针
  size_t size; // 大小
};

template <typename T> class Array {
private:
  T *data;         // 原生数组指针
  size_t capacity; // 数组容量
  size_t size;     // 当前大小

  // 扩展数组容量
  void resize(size_t newCapacity) {
    T *resized = new T[newCapacity];
    for (size_t i = 0; i < size; ++i) {
      resized[i] = data[i];
    }
    delete[] data;
    data = resized;
    capacity = newCapacity;
  }

public:
  // 构造函数
  Array() : data(nullptr), capacity(0), size(0) {}

  // 析构函数
  ~Array() { delete[] data; }

  // 添加元素
  void push(const T &value) {
    if (size == capacity) {
      // 初次添加时设置为 1，之后每次容量翻倍
      resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[size++] = value;
  }

  // 删除指定下标的元素
  void remove(size_t index) {
    if (index >= size) {
      throw out_of_range("[UmbralKeys Array::remove] Index out of range");
    }

    for (size_t i = index; i < size - 1; ++i) {
      data[i] = data[i + 1];
    }
    --size;
  }

  // 清空所有元素
  void clear() { size = 0; }

  // 获取当前数组的长度
  size_t getSize() const { return size; }

  NativeArray<T> toNativeArray() const {
    // 返回一个新的数组，包含当前的元素
    T *arr = new T[size];
    for (size_t i = 0; i < size; ++i) {
      arr[i] = data[i];
    }

    NativeArray<T> nativeArray;
    nativeArray.data = arr;
    nativeArray.size = size;
    return nativeArray;
  }

  // 获取指定索引的元素
  T &operator[](size_t index) {
    if (index >= size || index < 0) {
      throw out_of_range("[UmbralKeys Array::operator[]] Index out of range");
    }
    return data[index];
  }

  const T &operator[](size_t index) const {
    if (index >= size || index < 0) {
      throw out_of_range("[UmbralKeys Array::operator[]] Index out of range");
    }
    return data[index];
  }
};
