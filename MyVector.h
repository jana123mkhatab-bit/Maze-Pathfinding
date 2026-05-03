#ifndef MYVECTOR_H
#define MYVECTOR_H

template <typename T>
class MyVector {
private:
    T* data; // to allocate array in heap
    unsigned int capacity; // so it won't accept negtive numbers
    unsigned int length;

    void resize(unsigned int new_capacity);

public:
    MyVector();
    MyVector(unsigned int initial_size);
    MyVector(const MyVector& other); // copy const
    ~MyVector(); //destructor

    void push_back(const T& value);
    void pop_back();

    T& operator[](unsigned int index); //overloadd //random access
    const T& operator[](unsigned int index) const;

    unsigned int size() const;
    bool empty() const;
    void clear();

    T& back();
    const T& back() const;

    void assign(unsigned int count, const T& value);
    MyVector& operator=(const MyVector& other); //deep copy

    // Additional methods used in the code
    void pop();
    void push(const T& value);

    T& front();
    const T& front() const;
};
#include "MyVector.tpp"
#endif // MYVECTOR_H
