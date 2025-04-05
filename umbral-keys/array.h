#pragma once
#include <iostream>

template <typename T> struct NativeArray {
  T *data; // ԭ������ָ��
  size_t size; // ��С
};

template <typename T> class Array {
private:
  T *data;         // ԭ������ָ��
  size_t capacity; // ��������
  size_t size;     // ��ǰ��С

  // ��չ��������
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
      throw out_of_range("[UmbralKeys Array::remove] Index out of range");
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

  NativeArray<T> toNativeArray() const {
    // ����һ���µ����飬������ǰ��Ԫ��
    T *arr = new T[size];
    for (size_t i = 0; i < size; ++i) {
      arr[i] = data[i];
    }

    NativeArray<T> nativeArray;
    nativeArray.data = arr;
    nativeArray.size = size;
    return nativeArray;
  }

  // ��ȡָ��������Ԫ��
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
