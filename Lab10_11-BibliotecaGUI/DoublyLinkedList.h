#pragma once

template<typename TElem>
class IteratorList;

template<typename TElem>
class DoublyLinkedList;

template<typename TElem>
class Node {
private:
	TElem elem;

	Node* next;

	Node* prev;

public:

	Node() = default;

	Node(TElem e);

	Node(const Node& other);

	TElem& getElem();

	Node<TElem>* getNext();

	Node<TElem>* getPrev();

	void setEelem(TElem e);

	friend class DoublyLinkedList<TElem>;
	friend class Node<TElem>;

};


template <typename TElem>
class DoublyLinkedList {
private:
	Node<TElem>* head;

	Node<TElem>* tail;

	int length;

public:

	DoublyLinkedList() noexcept;

	DoublyLinkedList(const DoublyLinkedList& other);

	DoublyLinkedList(DoublyLinkedList&& other) noexcept;

	DoublyLinkedList& operator=(const DoublyLinkedList& other);
	DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;
	TElem& operator[](int pos);
	TElem operator[](int pos) const;

	~DoublyLinkedList();

	void push_back(TElem elem);

	TElem& at(int pos) noexcept;

	void erase(IteratorList<TElem>& it) noexcept;

	void set(TElem elem, int pos);

	int size() const noexcept;

	friend class IteratorList<TElem>;

	IteratorList<TElem> begin() noexcept;
	IteratorList<TElem> end() noexcept;
	IteratorList<TElem> find(IteratorList<TElem> begin, IteratorList<TElem> end, TElem e) noexcept;
	int distance(IteratorList<TElem> begin, IteratorList<TElem> end) noexcept;

};

template<typename TElem>
class IteratorList {
private:
	const DoublyLinkedList<TElem>& list;
	Node<TElem>* current = list.head;

public:

	IteratorList(const DoublyLinkedList<TElem>& l) noexcept;
	IteratorList(const DoublyLinkedList<TElem>& l, Node<TElem>* it) noexcept;

	bool valid() const noexcept;
	TElem& element() const;
	void next();

	TElem& operator*();
	IteratorList& operator++();
	IteratorList& operator+(int new_pos);

	bool operator==(const IteratorList& other) noexcept;
	bool operator!=(const IteratorList& other) noexcept;

	friend class DoublyLinkedList<TElem>;
};

template<typename TElem>
Node<TElem>::Node(TElem e) {
	this->elem = e;
	this->next = nullptr;
	this->prev = nullptr;
}

template<typename TElem>
Node<TElem>::Node(const Node& other) {
	this->elem = other.elem;
	this->next = other.next;
	this->prev = other.prev;
}

template<typename TElem>
TElem& Node<TElem>::getElem() {
	return this->elem;
}

template<typename TElem>
Node<TElem>* Node<TElem>::getNext() {
	return this->next;
}

template<typename TElem>
Node<TElem>* Node<TElem>::getPrev() {
	return this->prev;
}

template<typename TElem>
void Node<TElem>::setEelem(TElem e) {
	this->elem = e;
}

template<typename TElem>
DoublyLinkedList<TElem>::DoublyLinkedList() noexcept {
	this->head = nullptr;
	this->tail = nullptr;
	this->length = 0;
}

template<typename TElem>
DoublyLinkedList<TElem>::DoublyLinkedList(const DoublyLinkedList& other) {
	if (other.head == nullptr) {
		this->head = this->tail = nullptr;
		this->length = other.length;
	}
	else {
		Node<TElem>* tempOther = other.head;

		this->head = this->tail = new Node<TElem>;
		this->head->prev = nullptr;

		this->head->elem = tempOther->elem;

		tempOther = tempOther->getNext();

		if (tempOther == nullptr)
			this->head->next = nullptr;
		else {
			Node<TElem>* temp = this->head;
			while (tempOther != nullptr) {
				temp->next = new Node<TElem>;
				temp->next->prev = temp;
				temp = temp->next;

				temp->elem = tempOther->elem;
				tempOther = tempOther->getNext();
			}

			temp->next = nullptr;
			this->tail = temp;
		}

		this->length = other.length;
	}

}

template<typename TElem>
DoublyLinkedList<TElem>::DoublyLinkedList(DoublyLinkedList&& other) noexcept {
	this->head = other.head;
	this->tail = other.tail;
	this->length = other.length;

	other.head = nullptr;
	other.tail = nullptr;
	other.length = 0;
}

template<typename TElem>
DoublyLinkedList<TElem>& DoublyLinkedList<TElem>::operator=(const DoublyLinkedList& other) {
	if (&other == this) {
		return *this;
	}
	else {
		if (this->head != nullptr) {
			Node<TElem>* p = head;

			while (p != nullptr) {
				Node<TElem>* temp = p;
				p = p->next;
				delete temp;
			}

			head = tail = nullptr;
		}

		if (other.head == nullptr) {
			head = tail = nullptr;
		}
		else {
			Node<TElem>* tempOther = other.head;

			head = tail = new Node<TElem>;
			head->prev = nullptr;

			head->setEelem(tempOther->elem);
			tempOther = tempOther->getNext();

			if (tempOther == nullptr)
				head->next = nullptr;
			else {
				Node<TElem>* temp = head;

				while (tempOther != nullptr) {
					temp->next = new Node<TElem>;
					temp->next->prev = temp;
					temp = temp->next;

					temp->setEelem(tempOther->elem);
					tempOther = tempOther->next;
				}

				temp->next = nullptr;
				tail = temp;
			}
		}
		length = other.length;
	}

	return *this;
}

template<typename TElem>
DoublyLinkedList<TElem>& DoublyLinkedList<TElem>::operator=(DoublyLinkedList&& other) noexcept {
	if (&other == this) {
		return *this;
	}

	Node<TElem>* p = head;
	while (p != nullptr) {
		Node<TElem>* temp = p;
		p = p->next;
		delete temp;
	}

	this->head = other.head;
	this->tail = other.tail;
	this->length = other.length;

	other.head = nullptr;
	other.tail = nullptr;
	other.length = 0;

	return *this;
}

template<typename TElem>
TElem& DoublyLinkedList<TElem>::operator[](int pos) {
	Node<TElem>* node = this->head;
	while (pos != 0) {
		node = node->next;
		pos--;
	}

	return node->elem();
}

template<typename TElem>
TElem DoublyLinkedList<TElem>::operator[](int pos) const {
	Node<TElem>* node = this->head;
	while (pos != 0) {
		node = node->next;
		pos--;
	}

	return node->elem();
}


template<typename TElem>
DoublyLinkedList<TElem>::~DoublyLinkedList() {
	if (this->head != nullptr) {
		Node<TElem>* p = head;
		while (p != nullptr) {
			Node<TElem>* temp = p;
			p = p->next;
			delete temp;
		}
	}
}

template<typename TElem>
void DoublyLinkedList<TElem>::push_back(TElem elem) {
	Node<TElem>* newNode = new Node<TElem>(elem);

	if (this->head == nullptr) {
		this->head = newNode;
		this->tail = newNode;
	}
	else {
		newNode->prev = this->tail;
		this->tail->next = newNode;
		this->tail = newNode;
	}
}

template<typename TElem>
TElem& DoublyLinkedList<TElem>::at(int pos) noexcept {
	Node<TElem>* node = head;
	while (pos != 0) {
		node = node->next;
		pos--;
	}

	return node->elem;
}

template<typename TElem>
void DoublyLinkedList<TElem>::erase(IteratorList<TElem>& it) noexcept {
	Node<TElem>* node = it.current;

	if (node == head) {
		head = head->next;
		if (head == nullptr)
			tail = nullptr;
	}
	else if (node == tail) {
		it.current->prev->next = nullptr;
		tail = it.current->prev;
	}
	else {
		it.current->prev->next = it.current->next;
		it.current->next->prev = it.current->prev;
	}
	delete node;
}

template<typename TElem>
void DoublyLinkedList<TElem>::set(TElem e, int pos) {
	Node<TElem>* node = head;
	while (pos != 0) {
		node = node->next;
		pos--;
	}

	node->setEelem(e);
}

template<typename TElem>
int DoublyLinkedList<TElem>::size() const noexcept {
	if (head == nullptr) {
		return 0;
	}
	else {
		int dim = 0;
		Node<TElem>* node = head;
		while (node != nullptr) {
			dim++;
			node = node->next;
		}

		return dim;
	}
}

template<typename TElem>
IteratorList<TElem> DoublyLinkedList<TElem>::begin() noexcept {
	return IteratorList<TElem>(*this);
}

template<typename TElem>
IteratorList<TElem> DoublyLinkedList<TElem>::end() noexcept {
	return IteratorList<TElem>(*this, tail->next);
}

template<typename TElem>
IteratorList<TElem> DoublyLinkedList<TElem>::find(IteratorList<TElem> begin, IteratorList<TElem> end, TElem e) noexcept {
	IteratorList<TElem> it = begin;
	while (it != end) {
		if (it.element() == e)
			return it;
		it.next();
	}

	return it;
}

template<typename TElem>
int DoublyLinkedList<TElem>::distance(IteratorList<TElem> begin, IteratorList<TElem> end) noexcept {
	int pos = 0;
	while (begin != end) {
		pos++;
		begin.next();
	}

	return pos;
}

template<typename TElem>
IteratorList<TElem>::IteratorList(const DoublyLinkedList<TElem>& l) noexcept : list{ l } {};

template<typename TElem>
IteratorList<TElem>::IteratorList(const DoublyLinkedList<TElem>& l, Node<TElem>* it) noexcept : list{ l }, current{ it } {};

template<typename TElem>
bool IteratorList<TElem>::valid() const noexcept {
	return this->current != nullptr;
}

template<typename TElem>
TElem& IteratorList<TElem>::element() const {
	return this->current->getElem();
}

template<typename TElem>
void IteratorList<TElem>::next() {
	current = current->getNext();
}

template<typename TElem>
TElem& IteratorList<TElem>::operator*() {
	return this->element();
}

template<typename TElem>
IteratorList<TElem>& IteratorList<TElem>::operator++() {
	this->next();
	return *this;
}

template<typename TElem>
bool IteratorList<TElem>::operator==(const IteratorList& other) noexcept {
	return this->current == other.current;
}

template<typename TElem>
bool IteratorList<TElem>::operator!=(const IteratorList& other) noexcept {
	return !(this->current == other.current);
}

template<typename TElem>
IteratorList<TElem>& IteratorList<TElem>::operator+(int new_pos) {
	while (new_pos != 0) {
		this->current = this->current->next;
		new_pos--;
	}

	return (*this);
}