#ifndef DS_STACK_QUEUE_H
#define DS_STACK_QUEUE_H

#include <vector>
using namespace std;

/* ------------- [ds_stack_queue.h] Stack & Queue (linked list) ------------- */
template <class T>
class Stack
{
    struct Node
    {
        T data;
        Node *next;
    };
    Node *top_ = nullptr;
    int sz = 0;

public:
    ~Stack()
    {
        while (top_)
        {
            Node *t = top_;
            top_ = top_->next;
            delete t;
        }
    }
    void push(T val)
    {
        top_ = new Node{val, top_};
        sz++;
    }
    T pop()
    {
        T v = top_->data;
        Node *t = top_;
        top_ = top_->next;
        delete t;
        sz--;
        return v;
    }
    bool empty() { return top_ == nullptr; }
    int size() { return sz; }
};

template <class T>
class Queue
{
    struct Node
    {
        T data;
        Node *next;
    };
    Node *front_ = nullptr;
    Node *rear_ = nullptr;
    int sz = 0;

public:
    ~Queue()
    {
        while (front_)
        {
            Node *t = front_;
            front_ = front_->next;
            delete t;
        }
    }
    void push(T val)
    {
        Node *n = new Node{val, nullptr};
        if (!rear_)
            front_ = rear_ = n;
        else
        {
            rear_->next = n;
            rear_ = n;
        }
        sz++;
    }
    T pop()
    {
        T v = front_->data;
        Node *t = front_;
        front_ = front_->next;
        if (!front_)
            rear_ = nullptr;
        delete t;
        sz--;
        return v;
    }
    bool empty() { return front_ == nullptr; }
    int size() { return sz; }
    vector<T> snapshot()
    { // read-only peek at all elements, FIFO order
        vector<T> out;
        for (Node *n = front_; n; n = n->next)
            out.push_back(n->data);
        return out;
    }
};

#endif
