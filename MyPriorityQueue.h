#ifndef MYPRIORITYQUEUE_H
#define MYPRIORITYQUEUE_H

#include "MyVector.h"

template <typename T>
class MyPriorityQueue {
private:

    MyVector<T> heap;

    void heapify_up(unsigned int index);
    void heapify_down(unsigned int index);

public:
    MyPriorityQueue();
    bool empty() const;
    unsigned int size() const;
    const T& top() const;
    void push(const T& value);
    void pop();
};

#include "MyPriorityQueue.tpp"

#endif // MYPRIORITYQUEUE_H
