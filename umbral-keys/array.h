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
  void expand() {
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

 public:
  // 构造函数
  Array() : data(nullptr), capacity(0), size(0) {
    // cout << "Array constructor - normal" << endl;
  }

  // 复制构造函数：深拷贝
  Array(const Array &other)
      : size(other.size), capacity(other.capacity), data(new T[other.size]) {
    // cout << "Array constructor - copy" << endl;
    for (size_t i = 0; i < size; ++i) {
      data[i] = other.data[i];
    }
  }

  // 赋值运算符：深拷贝
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

  // 析构函数
  ~Array() { delete[] data; }

  // 添加元素
  void push(const T &value) {
    expand();
    data[size] = value;
    size++;
  }

  // 删除指定下标的元素
  void remove(size_t index) {
    if (index >= size || size == 0) {
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
