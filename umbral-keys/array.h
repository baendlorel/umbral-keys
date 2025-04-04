#pragma once
#include <iostream>
#include <stdexcept>

template <typename T> class Array {
private:
  T *data;         // ԭ������ָ��
  size_t capacity; // ��������
  size_t size;     // ��ǰ��С

  // ��չ��������
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
  // ���캯��
  Array() : data(nullptr), capacity(0), size(0) {}

  // ��������
  ~Array() { delete[] data; }

  // ���Ԫ��
  void push(const T &value) {
    if (size == capacity) {
      // �������ʱ����Ϊ 1��֮��ÿ����������
      resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[size++] = value;
  }

  // ɾ��ָ���±��Ԫ��
  void remove(size_t index) {
    if (index >= size) {
      throw std::out_of_range("Index out of range");
    }

    for (size_t i = index; i < size - 1; ++i) {
      data[i] = data[i + 1];
    }
    --size;
  }

  // �������Ԫ��
  void clear() { size = 0; }

  // ��ȡ��ǰ����ĳ���
  size_t getSize() const { return size; }

  T *to_array() const {
    // ����һ���µ����飬������ǰ��Ԫ��
    T *new_array = new T[size];
    for (size_t i = 0; i < size; ++i) {
      new_array[i] = data[i];
    }
    return new_array;
  }

  // ��ȡָ��������Ԫ��
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
