#include "MyPriorityQueue.h"

template <typename T>
MyPriorityQueue<T>::MyPriorityQueue() {}

template <typename T>
void MyPriorityQueue<T>::heapify_up(unsigned int index) {
    //starts from the last index
    while (index > 0) {
        //while we didn't reach the root
        unsigned int parent = (index - 1) / 2;
        //this condition is comparing priority not numbers
        // the higher priotiy goes to the smaller cost
        if (heap[parent] < heap[index]) { // if this was changed it would be min heap
            T temp = heap[parent];
            heap[parent] = heap[index];
            heap[index] = temp;
            index = parent; // to move up and repeat
        } else {
            break;
        }
    }
}

template <typename T>
void MyPriorityQueue<T>::heapify_down(unsigned int index) {
    unsigned int size = heap.size();
    while (true) {
        unsigned int left = 2 * index + 1;
        unsigned int right = 2 * index + 2;
        //in our contect it's not largest it's lowest
        // we are assuming it's the best node with lowest node
        // and highst priority
        unsigned int largest = index; //

        if (left < size && heap[largest] < heap[left]) {
            largest = left;
        }
        if (right < size && heap[largest] < heap[right]) {
            largest = right;
        }

        if (largest != index) {
            T temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;
            index = largest;
        } else {
            break;
        }
    }
}

template <typename T>
bool MyPriorityQueue<T>::empty() const {
    return heap.empty();
}

template <typename T>
unsigned int MyPriorityQueue<T>::size() const {
    return heap.size();
}

template <typename T>
const T& MyPriorityQueue<T>::top() const {
    return heap[0];
}

template <typename T>
void MyPriorityQueue<T>::push(const T& value) {
    heap.push_back(value);
    heapify_up(heap.size() - 1);
}

template <typename T>
void MyPriorityQueue<T>::pop() {
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        heapify_down(0);
    }
}
