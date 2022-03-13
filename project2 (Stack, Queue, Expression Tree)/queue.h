#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

using namespace std;

template<typename type>
class Queue;

template<typename type>
class Stack;

template<typename type>
class Node {
private:
  type item;
  Node<type>* next;
  friend class Queue<type>;
  friend class Stack<type>;

};


template <typename type>
class Queue {
private:
    Node<type>* front_node;
    Node<type>* rear_node;
public:
  // Constructor
  explicit Queue();

  // Destructor
  ~Queue();

  // Store new element
  void push(const type& item);

  // Return first element
  type& front() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;

};

// Implementation

template <typename type>
Queue<type>::Queue() {
  front_node = 0;
  rear_node = 0;

}


template <typename type>
Queue<type>::~Queue(){
  if (!empty()){
    Node<type>* temp1 = front_node;
    Node<type>* temp2 = 0;
    while (temp1 != 0){
      temp2 = temp1;
      temp1 = temp1->next;
      delete temp2;
  }
  }
}


template <typename type>
void Queue<type>::push(const type &item) {
  
  Node<type>* newnode = new Node<type>;
  newnode->item = item;
  
  if (front_node == 0){
    front_node = newnode;
  }

  newnode->next = 0;
  if (rear_node != 0){
  rear_node->next = newnode;
  }

  rear_node = newnode;
  
  
}

template <typename type>
type& Queue<type>::front() const {
  return front_node->item;

}

template <typename type>
void Queue<type>::pop() {
  Node<type>* temp = front_node;
  front_node = front_node->next;
  delete temp;

}

template <typename type>
bool Queue<type>::empty() const {
  return front_node == 0;
}

template <typename type>
int Queue<type>::size() const {
  Node<type>* temp = front_node;
  int size = 0;
  while (temp != 0){
    size++;
    temp = temp->next;
  }
  return size;

}

#endif
