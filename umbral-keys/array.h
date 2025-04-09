#pragma once
#include <stdexcept>

using namespace std;

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
  void expand() {
    if (size < capacity) {
      return;  // ��������
    }

    size_t newCapacity = (capacity == 0) ? 3 : capacity * 2;
    T *resized = new T[newCapacity];  // �����׳� std::bad_alloc

    // ����ԭ data ��Чʱ�ſ�������
    if (data != nullptr) {
      for (size_t i = 0; i < size; ++i) {
        resized[i] = data[i];  // ȷ�� size <= capacity
      }
      delete[] data;
    }

    data = resized;
    capacity = newCapacity;
  }

 public:
  // ���캯��
  Array() : data(nullptr), capacity(0), size(0) {
    // cout << "Array constructor - normal" << endl;
  }

  // ���ƹ��캯�������
  Array(const Array &other)
      : size(other.size), capacity(other.capacity), data(new T[other.size]) {
    // cout << "Array constructor - copy" << endl;
    for (size_t i = 0; i < size; ++i) {
      data[i] = other.data[i];
    }
  }

  // ��ֵ����������
  Array &operator=(const Array &other) {
    // cout << "Array constructor - copy =" << endl;
    if (this == &other) {
      return *this;
    }
    delete[] data;
    size = other.size;
    data = new T[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = other.data[i];
    }
    return *this;
  }

  Array(size_t capacity) : capacity(capacity), size(0) {
    // cout << "Array constructor - capacity" << endl;
    if (capacity <= 0) {
      throw invalid_argument("[UmbralKeys Array] capacity must be > 0");
    }
    data = new T[capacity];
  }

  // ��������
  ~Array() { delete[] data; }

  // ���Ԫ��
  void push(const T &value) {
    expand();
    data[size] = value;
    size++;
  }

  // ɾ��ָ���±��Ԫ��
  void remove(size_t index) {
    if (index >= size || size == 0) {
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
    if (index >= size || index < 0 || size == 0) {
      throw out_of_range("[UmbralKeys Array::operator[]] Index out of range");
    }
    return data[index];
  }

  const T &operator[](size_t index) const {
    if (index >= size || index < 0 || size == 0) {
      throw out_of_range("[UmbralKeys Array::operator[]] Index out of range");
    }
    return data[index];
  }
};
