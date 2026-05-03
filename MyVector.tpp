#include "MyVector.h"

template <typename T>
MyVector<T>::MyVector() : data(nullptr), capacity(0), length(0) {
    resize(1);
}

template <typename T>
MyVector<T>::MyVector(unsigned int initial_size) : data(nullptr), capacity(0), length(0) {
    resize(initial_size);
    length = initial_size;
}

template <typename T>
MyVector<T>::MyVector(const MyVector& other) : data(nullptr), capacity(0), length(0) {
    resize(other.capacity);
    length = other.length;
    for (unsigned int i = 0; i < length; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}

template <typename T>
void MyVector<T>::resize(unsigned int new_capacity) {
    T* new_data = new T[new_capacity];
    for (unsigned int i = 0; i < length; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

// we use push instead of normal insert to be optimized
template <typename T>
void MyVector<T>::push_back(const T& value) {
    if (length >= capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[length] = value;
    ++length;
}

template <typename T>
void MyVector<T>::pop_back() {
    if (length > 0) {
        --length;
    }
}

template <typename T>
T& MyVector<T>::operator[](unsigned int index) {
    return data[index];
}

template <typename T>
const T& MyVector<T>::operator[](unsigned int index) const {
    return data[index];
}

template <typename T>
unsigned int MyVector<T>::size() const {
    return length;
}

template <typename T>
bool MyVector<T>::empty() const {
    return length == 0;
}

template <typename T>
void MyVector<T>::clear() {
    length = 0;
}

template <typename T>
T& MyVector<T>::back() {
    return data[length - 1];
}

template <typename T>
const T& MyVector<T>::back() const {
    return data[length - 1];
}


//count is the new size and value is the elements
//will be stored along the vecctor
//so v.assign(5,10) v=[5,5,5,5,5]
template <typename T>
void MyVector<T>::assign(unsigned int count, const T& value) {
    if (count > capacity) {
        resize(count);
    }
    length = count;
    for (unsigned int i = 0; i < count; ++i) {
        data[i] = value;
    }
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if (this != &other) {
        delete[] data;
        data = nullptr;
        capacity = 0;
        length = 0;
        resize(other.capacity);
        length = other.length;
        for (unsigned int i = 0; i < length; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
void MyVector<T>::pop() {
    pop_back();
}

template <typename T>
void MyVector<T>::push(const T& value) {
    push_back(value);
}

template <typename T>
T& MyVector<T>::front() {
    return data[0];
}

template <typename T>
const T& MyVector<T>::front() const {
    return data[0];
}

// Explicit template instantiations
template class MyVector<int>;
template class MyVector<bool>;
template class MyVector<MyVector<int>>;
