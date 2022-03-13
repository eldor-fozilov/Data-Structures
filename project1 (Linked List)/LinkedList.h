#ifndef LinkedList_h
#define LinkedList_h

#include <typeinfo>
#include <iostream>

template <typename Type>
class LinkedList;

template <typename Type>
class Node {
private:
	Type data;
	Node<Type>* next;
	friend class LinkedList<Type>;
};

template <typename Type>
class LinkedList
{
private:
	Node<Type>* head; 
	Node<Type>* tail;
public:
	// Constructor
	LinkedList();

	// Destructor
	~LinkedList();

	// Get the value located at index
	Type Get(const int index);

	// Add val at head
	void AddAtHead(const Type& val);

	// Add val at index
	void AddAtIndex(const int index, const Type& val);

	// Delete an element located at index
	void DeleteAtIndex(const int index);

	// Delete val in linked list
	void DeleteValue(const Type& val);

	// Move the first element of val to head
	void MoveToHead(const Type& val);

	// Rotate the linked list right by steps times
	void Rotate(const int steps);

	// Reduce value that repeats multiple times
	void Reduce();

	// Reverse at every k number of nodes at a time
	void K_Reverse(const int k);

	// Sort even and odd numbers separately then append
	void EvenOddSeparateSort();

	// Return the number of elements in the linked list
	int Size();

	// Check whether the linked list is empty or not
	bool Empty();

	// Delete all elements from the linked list
	void CleanUp();

	// Print the list
	void Print();

};


/*
 *	Implementation
 */

template <typename Type>
LinkedList<Type>::LinkedList()
{
	head = 0;
	tail = 0;
}

template <typename Type>
LinkedList<Type>::~LinkedList()
{
	CleanUp();
}

template <typename Type>
Type LinkedList<Type>::Get(const int index)
{
	if (index < 0 || index > (Size() - 1)) {
		return -1;
	}
	else {
		
		if (!Empty()){
		Node<Type>* temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}

		return temp->data;
		}
		
		else{
			return -1;
		}
	}
}

template <typename Type>
void LinkedList<Type>::AddAtHead(const Type& val)
{
	 
	Node<Type>* newnode = new Node<Type>;
	newnode->data = val;
	newnode->next = head;
	head = newnode;
	if (tail == 0){
		tail = newnode;
	}

}

template <typename Type>
void LinkedList<Type>::AddAtIndex(const int index, const Type& val)
{
	if (index == 0) {
		AddAtHead(val);
	}
	else if (index == Size()) {
		Node<Type>* newnode = new Node<Type>;
		newnode->data = val;
		newnode->next = 0;
		tail->next = newnode;
		tail = newnode;
	}
	else if (index > 0 && index < Size()) {
		Node<Type>* newnode = new Node<Type>;
		Node<Type>* temp = head;

		for (int i = 0; i < index - 1; i++) {
			temp = temp->next;
		}

		newnode->data = val;
		newnode->next = temp->next;
		temp->next = newnode;
	}

}

template <typename Type>
void LinkedList<Type>::DeleteAtIndex(const int index)
{
	if (!Empty()){

	Node<Type>* temp1 = head;
	Node<Type>* temp2 = 0;

	if (index == 0) {
		head = head->next;
		delete temp1;
	}
	else if (index == Size() - 1) {
		
		for (int i = 0; i < index - 1;i++) {
			temp1 = temp1->next;
		}
		
		temp2 = tail;
		tail = temp1;
		tail->next = 0;

		delete temp2;
	}
	else if (index > 0 && index < Size() - 1) {
		
		for (int i = 0; i < index - 1;i++) {
			temp1 = temp1->next;
		}

		temp2 = temp1->next;
		temp1->next = temp2->next;
		
		delete temp2;

	}
	}
}

template <typename Type>
void LinkedList<Type>::DeleteValue(const Type& val)
{
	if (!Empty()){

	Node<Type>* temp = head;
	int index = -1;
	bool check = false;
	while (temp != 0) {
		
		index++;

		if (temp->data == val) {
			check = true;
			break;
		}
		
		temp = temp->next;
	}

	if (check) {
		DeleteAtIndex(index);
	}

	}

}

template <typename Type>
void LinkedList<Type>::MoveToHead(const Type& val)
{
	if (!Empty() && Size() != 1){

	Node<Type>* temp = head;

	if (head->data != val){
		
	while (temp != tail){
		if (temp->next->data == val){
				Node<Type>* x = temp->next->next; 
				temp->next->next = head;
				head = temp->next;

				if (temp->next == tail){
					tail = temp;
				}

				temp->next = x;

				break;
		}
	
		temp = temp->next;
	}

	}
	}

}

template <typename Type>
void LinkedList<Type>::Rotate(const int steps)
{
	if (!Empty() && Size() != 1 && steps > 0){

	for (int i = 1; i <= steps; i++)
	{


	Node<Type>* old_head = head;
	Node<Type>* temp = head;
	while (temp->next != tail){
		temp = temp->next;
	}


	head = temp->next;
	temp->next = tail->next;
	head->next = old_head;
	tail = temp;

	}

	}

}

template <typename Type>
void LinkedList<Type>::Reduce()
{	
	if (!Empty() && Size() != 1){
	
	Node<Type>* temp1 = head;

	while (temp1 != tail){

		Type x = temp1->data;
		Node<Type>* temp2 = temp1;
		Node<Type>* trash = 0;
			
		while (temp2 != tail){

			if (x == temp2->next->data){
				
				trash = temp2->next;

				if (temp2->next != tail){
					
				temp2->next = temp2->next->next;
				delete trash;
				}
				
				else{
					temp2->next = 0;
					tail = temp2;
					delete trash;
				}
			}
			else {

			temp2 = temp2->next;
			}
		}

		temp1 = temp1->next;

	}

	}
}

template <typename Type>
void LinkedList<Type>::K_Reverse(const int k)
{
	if (!Empty() && Size() != 1 && k <= Size()){

	Node<Type>* prev = 0;
	Node<Type>* current = head;
	Node<Type>* next = 0;
	Node<Type>* new_tail = head;

	int iter = int(Size() / k);


	for (int i = 0; i < iter; i++){

		for (int j = 0; j < k; j++){
			new_tail = new_tail->next;
		}

		prev = new_tail;

		if (i != iter - 1){	
		for (int z = 0; z < k - 1; z++){
			prev = prev->next;
		}
		}

		while (current != new_tail){
		
			next = current->next;
			current->next = prev;
			prev = current;
			current = next;
		}
		if (i == 0){
			head = prev;
		}

	}

	}
}

template <typename Type>
void LinkedList<Type>::EvenOddSeparateSort()
{
	if (!Empty() && Size() != 1){
	Node<Type>* temp1 = head;
	Node<Type>* temp2 = temp1;
	Type copy; 

	// First, we will put even numbers on the front and odd numbers at the end
	while (temp1 != tail){
		
		temp2 = temp1;

		if (temp1->data % 2 != 0){

			while (temp2 != tail){
				if (temp2->next->data % 2 == 0){
					copy = temp1->data;
					temp1->data = temp2->next->data;
					temp2->next->data = copy;
					break;
				}

				temp2 = temp2->next;
		}
	}

	temp1 = temp1->next;

	}

	// Second, we will sort even numbes in ascending order
	Node<Type>* odd_start = head;

	while (odd_start->data % 2 == 0){
		
		if (odd_start == tail){
			break;
		}

		odd_start = odd_start->next;
	}

	bool check1 = true;
	bool check2 = true;

	if (odd_start == head && odd_start->data % 2 == 0){
		check1 = false;
		odd_start = head->next;
	}
	else if (odd_start == head && odd_start->data % 2 != 0)
	{
		check1 = false;
	}
	else if (odd_start == tail && odd_start->data % 2 == 0){
		check2 = false;
		odd_start = odd_start->next;
	}
	else if (odd_start == tail && odd_start->data % 2 != 0){
		check2 = false;
	}


	if (check1){

	temp1 = head;

	while (temp1->next != odd_start){
		Type min = temp1->data;
		Node<Type>* min_index = 0;
		temp2 = temp1;

		while (temp2->next != odd_start){
			temp2 = temp2->next;

			if (temp2->data < min){
				min = temp2->data;
				min_index = temp2;
			}
		}

		if (min_index){
		copy = temp1->data;
		temp1->data = min;
		min_index->data = copy;
		}
		temp1 = temp1->next;
	}

	}

	// Third, we will sort odd numbers in descending order
	
	if (check2){
	  	temp1 = odd_start;

	while (temp1 != tail){
		Type min = temp1->data;
		Node<Type>* min_index = 0;
		temp2 = temp1;

		while (temp2 != tail){
			temp2 = temp2->next;

			if (temp2->data > min){
				min = temp2->data;
				min_index = temp2;
			}
		}

		if (min_index){
		copy = temp1->data;
		temp1->data = min;
		min_index->data = copy;
		}
		temp1 = temp1->next;
	}

	}

	}

}

template <typename Type>
int LinkedList<Type>::Size()
{
	Node<Type>* temp = head;
	int size = 0;
	while (temp != 0){
		temp = temp->next;
		size++;
	}
	return size;

}

template <typename Type>
bool LinkedList<Type>::Empty(){
	return head == 0;
}

template <typename Type>
void LinkedList<Type>::CleanUp()
{
	Node<Type>* temp1 = head;
	Node<Type>* temp2 = head;
	
	while (temp1 != 0){
		temp1 = temp1->next;
		delete temp2;
		temp2 = temp1;
	}

	head = 0;
	tail = 0;

}

template <typename Type>
void LinkedList<Type>::Print()
{


	Node<Type>* temp = head;
	std::cout << "(";
	while (temp!= 0){
		std::cout << temp->data;
		temp = temp->next;
		if (temp != 0){
			std::cout << ",";
		}
	}
	std::cout << ")" << "\n";
}



#endif

