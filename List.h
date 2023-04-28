#ifndef LIST_H
#define LIST_H

template <class T>
struct ListNode {
	T data;
	ListNode <T>* mNext;
	ListNode <T>* mPrev;

	ListNode(const T &inital = T());

	ListNode <T>* next(unsigned int number = 1);
	ListNode <T>* prev(unsigned int number = 1);
};

template <class T>
class List {
public:
	List();
	~List();

	T& back();
	T& front();

	ListNode <T>* begin();
	ListNode <T>* end();
	ListNode <T>* rbegin();
	ListNode <T>* rend();

	bool empty() const;
	int size() const;

	void clear();
	void insert(unsigned int index, ListNode <T>* node);
	void insert(unsigned int index, const T &value);
	T erase(unsigned int index);

	void pushBack(ListNode <T>* node);
	void pushBack(const T &value);
	T popBack();
	void pushFront(ListNode <T>* node);
	void pushFront(const T &value);
	T popFront();

	List <T>& operator = (const List <T> &other);

private:	
	ListNode <T>* head;
	ListNode <T>* tail;
	unsigned int mSize;
};

template <class T>
ListNode <T>::ListNode(const T &inital) {
	data = inital;
	mNext = nullptr;
	mPrev = nullptr;
}

template <class T>
ListNode <T>* ListNode <T>::next(unsigned int number) {
	ListNode <T>* node = this;
	while (node != nullptr && number--) {
		node = node->mNext;
	}

	return node;
}

template <class T>
ListNode <T>* ListNode <T>::prev(unsigned int number) {
	ListNode <T>* node = this;
	while (node != nullptr && number--) {
		node = node->mPrev;
	}

	return node;
}

template <class T>
List <T>::List() {
	head = nullptr;
	tail = nullptr;
	mSize = 0;
}

template <class T>
List <T>::~List() {
	clear();
}

template <class T>
T& List <T>::back() {
	return tail->data;
}

template <class T>
T& List <T>::front() {
	return head->data;
}

template <class T>
ListNode <T>* List <T>::begin() {
	return head;
}

template <class T>
ListNode <T>* List <T>::end() {
	return nullptr;
}

template <class T>
ListNode <T>* List <T>::rbegin() {
	return tail;
}

template <class T>
ListNode <T>* List <T>::rend() {
	return nullptr;
}

template <class T>
bool List <T>::empty() const {
	return mSize == 0;
}

template <class T>
int List <T>::size() const {
	return (int)mSize;
}

template <class T>
void List <T>::clear() {
	while (!empty()) {
		popBack();
	}
}

template <class T>
void List <T>::insert(unsigned int index, ListNode <T>* node) {
	if (index > size()) {
		return;
	}

	if (index == 0) {
		pushFront(node);
		return;
	}

	if (index == size()) {
		pushBack(node);
		return;
	}

	ListNode <T>* pos = head->next(index - 1);
	node->mPrev = pos;
	node->mNext = pos->next();
	pos->next()->mPrev = node;
	pos->mNext = node;
	mSize++;
}

template <class T>
void List <T>::insert(unsigned int index, const T &value) {
	if (index > size()) {
		return;
	}

	ListNode <T>* node = new ListNode <T> (value);
	if (index == 0) {
		pushFront(node);
		return;
	}

	if (index == size()) {
		pushBack(node);
		return;
	}

	ListNode <T>* pos = head->next(index - 1);
	node->mPrev = pos;
	node->mNext = pos->next();
	pos->next()->mPrev = node;
	pos->mNext = node;
	mSize++;
}

template <class T>
T List <T>::erase(unsigned int index) {
	if (index >= size()) {
		return T();
	}

	if (index == 0) {
		return popFront();
	}

	if (index == size() - 1) {
		return popBack();
	}

	ListNode <T>* pos = head->next(index - 1);
	T res = pos->next()->data;
	pos->mNext = pos->next(2);
	delete pos->next()->prev();
	pos->next()->mPrev = pos;
	mSize--;

	return res;
}

template <class T>
void List <T>::pushBack(ListNode <T>* node) {
	if (empty()) {
		head = tail = node;
	} else {
		node->mPrev = tail;
		tail->mNext = node;
		tail = tail->next();
	}

	mSize++;
}

template <class T>
void List <T>::pushBack(const T &value) {
	pushBack(new ListNode <T> (value));
}

template <class T>
T List <T>::popBack() {
	if (tail == nullptr) {
		return T();
	}

	T res = tail->data;
	if (tail->prev() == nullptr) {
		delete tail;
		head = tail = nullptr;
	} else {
		tail = tail->prev();
		delete tail->next();
		tail->mNext = nullptr;
	}

	mSize--;

	return res;
}

template <class T>
void List <T>::pushFront(ListNode <T>* node) {
	if (empty()) {
		head = tail = node;
	} else {
		node->mNext = head;
		head->mPrev = node;
		head = head->prev();
	}

	mSize++;
}

template <class T>
void List <T>::pushFront(const T &value) {
	pushFront(new ListNode <T> (value));
}

template <class T>
T List <T>::popFront() {
	if (head == nullptr) {
		return T();
	}

	T res = head->data;
	if (head->next() == nullptr) {
		delete head;
		head = tail = nullptr;
	} else {
		head = head->next();
		delete head->prev();
		head->mPrev = nullptr;
	}

	mSize--;

	return res;
}

template <class T>
List <T>& List <T>::operator = (const List <T> &other) {
	clear();
	for (ListNode <T>* iterator = other.begin(); iterator != other.end(); iterator = iterator->next()) {
		pushBack(iterator->data);
	}

	return *this;
}

#endif 