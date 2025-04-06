#include "array.h"

using namespace std;

// ��չ��������
template <typename T>
void Array<T>::expand() {
  if (size < capacity) {
    return;
  }

  capacity = capacity * 2;
  if (capacity == 0) {
    capacity = 3;
    data = new T[capacity];
    return;
  }

  T *resized = new T[capacity];
  for (size_t i = 0; i < size; ++i) {
    resized[i] = data[i];
  }
  delete[] data;
  data = resized;
}

// ���캯��
template <typename T>
Array<T>::Array() {
  data = nullptr;
  capacity = 0;
  size = 0;
}

// ���캯��
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
