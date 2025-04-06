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
  void expand();

 public:
  // ���캯��
  Array();

  Array(initializer_list<T> list);

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

// ��չ��������
template <typename T>
void Array<T>::expand() {
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

// ���캯��
template <typename T>
Array<T>::Array() {
  data = nullptr;
  capacity = 0;
  size = 0;
}


template <typename T>
Array<T>::Array(std::initializer_list<T> list)
    : size(list.size()), capacity(list.size()) {
  data = new T[capacity];
  std::copy(list.begin(), list.end(), data);
}

template <typename T>
Array<T>::~Array() {
  delete[] data;
}

// ���Ԫ��
template <typename T>
void Array<T>::push(const T &value) {
  expand();
  data[size] = value;
  size++;
}

// ɾ��ָ���±��Ԫ��
template <typename T>
void Array<T>::remove(size_t index) {
  if (index >= size || size == 0) {
    throw out_of_range("[UmbralKeys Array::remove] Index out of range");
  }

  for (size_t i = index; i < size - 1; ++i) {
    data[i] = data[i + 1];
  }
  --size;
}

// �������Ԫ��
template <typename T>
void Array<T>::clear() {
  size = 0;
}

template <typename T>
size_t Array<T>::getSize() const {
  return size;
}
// ��ȡ��ǰ����ĳ���

template <typename T>
NativeArray<T> Array<T>::toNativeArray() const {
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
template <typename T>
T &Array<T>::operator[](size_t index) {
  if (index >= size || index < 0 || size == 0) {
    throw out_of_range("[UmbralKeys Array::operator[]] Index out of range");
  }
  return data[index];
}

template <typename T>
const T &Array<T>::operator[](size_t index) const {
  if (index >= size || index < 0 || size == 0) {
    throw out_of_range("[UmbralKeys Array::operator[]] Index out of range");
  }
  return data[index];
}

