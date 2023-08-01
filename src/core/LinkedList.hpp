#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

template<class T>
class LinkedList {
public:
    struct Node {
        T mData;
        Node* mNext;
        Node* mPrev;

        Node(const T& inital = T());
        Node* next(unsigned int number = 1);
        Node* prev(unsigned int number = 1);
    };

	LinkedList();
	~LinkedList();

	T& back();
	T& front();

	T& operator[](unsigned int index);

	Node* begin();
	Node* end();
	Node* rbegin();
	Node* rend();

	bool empty() const;
	unsigned int size() const;

	void clear();
	void insert(unsigned int index, const T& value);
	void pushBack(const T& value);
	void pushFront(const T& value);
	T erase(unsigned int index);
	T popBack();
	T popFront();

private:	
	Node* mHead;
	Node* mTail;
	unsigned int mSize;

	void insert(unsigned int index, Node* node);
	void pushBack(Node* node);
	void pushFront(Node* node);
};

template<class T>
LinkedList<T>::Node::Node(const T& inital) {
    mData = inital;
    mNext = nullptr;
    mPrev = nullptr;
}

template<class T>
typename LinkedList<T>::Node* LinkedList<T>::Node::next(unsigned int number) {
	Node* node = this;
	while (node != nullptr && number--) {
		node = node->mNext;
	}

	return node;
}

template<class T>
typename LinkedList<T>::Node* LinkedList<T>::Node::prev(unsigned int number) {
	Node* node = this;
	while (node != nullptr && number--) {
		node = node->mPrev;
	}

	return node;
}

template<class T>
LinkedList<T>::LinkedList() {
	mHead = nullptr;
	mTail = nullptr;
	mSize = 0;
}

template<class T>
LinkedList<T>::~LinkedList() {
	clear();
}

template<class T>
T& LinkedList<T>::back() {
	return mTail->data;
}

template<class T>
T& LinkedList<T>::front() {
	return mHead->data;
}

template<class T>
T& LinkedList<T>::operator[](unsigned int index) {
	return mHead->next(index)->data;
}

template<class T>
typename LinkedList<T>::Node* LinkedList<T>::begin() {
	return mHead;
}

template<class T>
typename LinkedList<T>::Node* LinkedList<T>::end() {
	return nullptr;
}

template<class T>
typename LinkedList<T>::Node* LinkedList<T>::rbegin() {
	return mTail;
}

template<class T>
typename LinkedList<T>::Node* LinkedList<T>::rend() {
	return nullptr;
}

template<class T>
bool LinkedList<T>::empty() const {
	return mSize == 0;
}

template<class T>
unsigned LinkedList<T>::size() const {
	return mSize;
}

template<class T>
void LinkedList<T>::clear() {
	while (!empty()) {
		popBack();
	}
}

template<class T>
void LinkedList<T>::insert(unsigned int index, Node* node) {
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

	Node* pos = mHead->next(index - 1);
	node->mPrev = pos;
	node->mNext = pos->next();
	pos->next()->mPrev = node;
	pos->mNext = node;
	mSize++;
}

template<class T>
void LinkedList<T>::insert(unsigned int index, const T &value) {
	if (index > size()) {
		return;
	}

	Node* node = new Node(value);
	if (index == 0) {
		pushFront(node);
		return;
	}

	if (index == size()) {
		pushBack(node);
		return;
	}

	Node* pos = mHead->next(index - 1);
	node->mPrev = pos;
	node->mNext = pos->next();
	pos->next()->mPrev = node;
	pos->mNext = node;
	mSize++;
}

template<class T>
T LinkedList<T>::erase(unsigned int index) {
	if (index >= size()) {
		return T();
	}

	if (index == 0) {
		return popFront();
	}

	if (index == size() - 1) {
		return popBack();
	}

	Node* pos = mHead->next(index - 1);
	T res = pos->next()->data;
	pos->mNext = pos->next(2);
	delete pos->next()->prev();
	pos->next()->mPrev = pos;
	mSize--;

	return res;
}

template<class T>
void LinkedList<T>::pushBack(Node* node) {
	if (empty()) {
		mHead = mTail = node;
	} else {
		node->mPrev = mTail;
		mTail->mNext = node;
		mTail = mTail->next();
	}

	mSize++;
}

template<class T>
void LinkedList<T>::pushBack(const T &value) {
	pushBack(new Node(value));
}

template<class T>
T LinkedList<T>::popBack() {
	if (mTail == nullptr) {
		return T();
	}

	T res = mTail->data;
	if (mTail->prev() == nullptr) {
		delete mTail;
		mHead = mTail = nullptr;
	} else {
		mTail = mTail->prev();
		delete mTail->next();
		mTail->mNext = nullptr;
	}

	mSize--;

	return res;
}

template<class T>
void LinkedList<T>::pushFront(Node* node) {
	if (empty()) {
		mHead = mTail = node;
	} else {
		node->mNext = mHead;
		mHead->mPrev = node;
		mHead = mHead->prev();
	}

	mSize++;
}

template<class T>
void LinkedList<T>::pushFront(const T &value) {
	pushFront(new Node(value));
}

template<class T>
T LinkedList<T>::popFront() {
	if (mHead == nullptr) {
		return T();
	}

	T res = mHead->data;
	if (mHead->next() == nullptr) {
		delete mHead;
		mHead = mTail = nullptr;
	} else {
		mHead = mHead->next();
		delete mHead->prev();
		mHead->mPrev = nullptr;
	}

	mSize--;

	return res;
}

#endif // LINKED_LIST_HPP