#pragma once
#include <stdexcept>

using namespace std;

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

  Array(initializer_list<T> list);

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

// 扩展数组容量
template <typename T>
void Array<T>::expand() {
  if (size < capacity) {
    return;  // 无需扩容
  }

  size_t newCapacity = (capacity == 0) ? 3 : capacity * 2;
  T *resized = new T[newCapacity];  // 可能抛出 std::bad_alloc

  // 仅当原 data 有效时才拷贝数据
  if (data != nullptr) {
    for (size_t i = 0; i < size; ++i) {
      resized[i] = data[i];  // 确保 size <= capacity
    }
    delete[] data;
  }

  data = resized;
  capacity = newCapacity;
}

// 构造函数
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

// 添加元素
template <typename T>
void Array<T>::push(const T &value) {
  expand();
  data[size] = value;
  size++;
}

// 删除指定下标的元素
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

// 清空所有元素
template <typename T>
void Array<T>::clear() {
  size = 0;
}

template <typename T>
size_t Array<T>::getSize() const {
  return size;
}
// 获取当前数组的长度

template <typename T>
NativeArray<T> Array<T>::toNativeArray() const {
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

