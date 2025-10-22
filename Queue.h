#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>
#include <memory>
#include <iostream>

// Node class definition
template<typename T>
class Node {
public:
    std::shared_ptr<T> data;
    std::shared_ptr<Node<T>> next;
    std::weak_ptr<Node<T>> prev;

    Node() : data(nullptr), next(nullptr), prev() {}
    Node(const T& val) : data(std::make_shared<T>(val)), next(nullptr), prev() {}
};

// Queue class definition
template<typename T>
class Queue {
private:
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;
    int size;

public:
    // Constructor
    Queue() : head(nullptr), tail(nullptr), size(0) {}

    // Destructor
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Insert element into the queue and return a pointer to the newly added node
    std::shared_ptr<Node<T>> enqueue(const T& element) {
        auto newNode = std::make_shared<Node<T>>(element);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++size;
        return newNode; // Return the newly added node
    }

    // Remove element from the front of the queue
    T dequeue() {
        if (isEmpty()) {
            throw std::out_of_range("Queue<>::dequeue(): empty Queue");
        }
        T value = *(head->data);
        head = head->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --size;
        return value;
    }

    // Peek at the front element of the queue
    T& peak() {
        if (isEmpty()) {
            throw std::out_of_range("Queue<>::peak(): empty Queue");
        }
        return *(head->data);
    }

    // Remove element at a specific position
    void removeValueInPosition(std::shared_ptr<Node<T>> pos) {
        if (!pos) {
            throw std::invalid_argument("Queue<>::removeValueAtPosition(): null position");
        }
        if (pos == head) {
            dequeue(); // Special case for head
        } else {
            auto prevNode = pos->prev.lock();
            if (prevNode) {
                prevNode->next = pos->next;
            }
            if (pos == tail) {
                tail = prevNode;
            }
            if (pos->next) {
                pos->next->prev = pos->prev;
            }
            --size;
        }
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Get the number of elements in the queue
    int getSize() const {
        return size;
    }
    // Output the queue to an ostream
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Queue<U>& queue);
};

// Output operator for Queue
template<typename T>
std::ostream& operator<<(std::ostream& os, const Queue<T>& queue) {
    os << "Queue: [";
    auto current = queue.head;
    while (current) {
        os << *(current->data);
        current = current->next;
        if (current) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

#endif // QUEUE_H
