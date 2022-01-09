#ifndef STACK_H
#define STACK_H

#include <iostream>

using namespace std;

template<typename type>
class Node;

template <typename type>
class Stack {

private:
    Node<type>* top_node;
    
public:
  // Constructor
  explicit Stack();

  // Destructor
  ~Stack();

  // Store new element
  void push(const type& item);

  // Return first element
  type& top() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;
};

// Implementation

template <typename type>
Stack<type>::Stack() {
    top_node = 0;
}

template <typename type>
Stack<type>::~Stack() {
  
  if (!empty()){
    Node<type>* temp1 = top_node;
    Node<type>* temp2 = 0;
    while (temp1 != 0){
      temp2 = temp1;
      temp1 = temp1->next;
      delete temp2;
    }
  }
}

template <typename type>
void Stack<type>::push(const type &item) {
  
  Node<type>* newnode = new Node<type>;
	newnode->item = item;
	newnode->next = top_node;
	top_node = newnode;

}

template <typename type>
type& Stack<type>::top() const {
  return top_node->item;

}

template <typename type>
void Stack<type>::pop() {
  Node<type>* temp = top_node;
  top_node = top_node->next;
  delete temp;
}

template <typename type>
bool Stack<type>::empty() const {
  return top_node == 0;
}

template <typename type>
int Stack<type>::size() const {
  Node<type>* temp = top_node;
  int size = 0;
  while (temp != 0){
    size++;
    temp = temp->next;
  }
  return size;
}

#endif //STACK_H