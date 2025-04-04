#pragma once
#include <iostream>
#include <stdexcept>

template <typename T> class Array {
private:
  T *data;         // 原生数组指针
  size_t capacity; // 数组容量
  size_t size;     // 当前大小

  // 扩展数组容量
  void resize(size_t new_capacity) {
    T *new_data = new T[new_capacity];
    for (size_t i = 0; i < size; ++i) {
      new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
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
      throw std::out_of_range("Index out of range");
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

  T *to_array() const {
    // 返回一个新的数组，包含当前的元素
    T *new_array = new T[size];
    for (size_t i = 0; i < size; ++i) {
      new_array[i] = data[i];
    }
    return new_array;
  }

  // 获取指定索引的元素
  T &operator[](size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index out of range");
    }
    return data[index];
  }

  const T &operator[](size_t index) const {
    if (index >= size) {
      throw std::out_of_range("Index out of range");
    }
    return data[index];
  }
};
