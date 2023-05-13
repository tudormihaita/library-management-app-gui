#pragma once
constexpr auto INITIAL_CAPACITY = 2;

template<typename TElem>
class IteratorVector;

template<typename TElem>
class DynamicVector {
private:
	TElem* elems;
	int capacity;
	int length;

	void resize();

public:

	DynamicVector();

	DynamicVector(const DynamicVector& other);

	DynamicVector(DynamicVector&& other) noexcept;

	DynamicVector& operator=(const DynamicVector& other);
	DynamicVector& operator=(DynamicVector&& other) noexcept;
	TElem& operator[](int pos);
	TElem operator[](int pos) const;

	~DynamicVector();

	void push_back(TElem elem);

	void erase(const IteratorVector<TElem>& it);

	TElem& at(int pos) noexcept;

	void set(TElem elem, int pos);

	int size() const noexcept;

	friend class IteratorVector<TElem>;

	IteratorVector<TElem> begin() noexcept;
	IteratorVector<TElem> end() noexcept;
	IteratorVector<TElem> find(IteratorVector<TElem> begin, IteratorVector<TElem> end, TElem elem) noexcept;
	int distance(IteratorVector<TElem> begin, IteratorVector<TElem> end) noexcept;
};

template<typename TElem>
class IteratorVector {
private:
	const DynamicVector<TElem>& vector;
	int pos = 0;

public:
	IteratorVector(const DynamicVector<TElem>& v) noexcept;
	IteratorVector(const DynamicVector<TElem>& v, int pos) noexcept;

	bool valid() const noexcept;
	TElem& element() const noexcept;
	void next() noexcept;

	TElem& operator*();
	IteratorVector& operator++();
	IteratorVector& operator+(int new_pos);

	bool operator==(const IteratorVector& other) noexcept;
	bool operator!=(const IteratorVector& other) noexcept;
};


template<typename TElem>
DynamicVector<TElem>::DynamicVector() {
	this->elems = new TElem[INITIAL_CAPACITY];
	this->length = 0;
	this->capacity = INITIAL_CAPACITY;
}

template<typename TElem>
DynamicVector<TElem>::DynamicVector(const DynamicVector& other) {
	this->elems = new TElem[other.capacity];

	for (int i = 0; i < other.length; i++) {
		this->elems[i] = other.elems[i];
	}

	this->length = other.length;
	this->capacity = other.capacity;
}

template<typename TElem>
DynamicVector<TElem>::DynamicVector(DynamicVector&& other) noexcept {
	this->elems = other.elems;
	this->length = other.length;
	this->capacity = other.capacity;

	other.elems = nullptr;
	other.length = 0;
	other.capacity = 0;
}

template<typename TElem>
DynamicVector<TElem>& DynamicVector<TElem>::operator=(const DynamicVector& other) {
	if (this == &other) {
		return *this;
	}

	delete[] this->elems;
	this->elems = new TElem[other.capacity];

	for (int i = 0; i < other.length; i++) {
		elems[i] = other.elems[i];
	}

	this->length = other.length;
	this->capacity = other.capacity;

	return *this;
}

template<typename TElem>
DynamicVector<TElem>& DynamicVector<TElem>::operator=(DynamicVector&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	delete[] this->elems;

	this->elems = other.elems;
	this->length = other.length;
	this->capacity = other.capacity;

	other.elems = nullptr;
	other.length = 0;
	other.capacity = 0;

	return *this;
}

template<typename TElem>
TElem& DynamicVector<TElem>::operator[](int pos) {
	return this->elems[pos];
}

template<typename TElem>
TElem DynamicVector<TElem>::operator[](int pos) const {
	return this->elems[pos];
}

template<typename TElem>
void DynamicVector<TElem>::resize() {
	const int new_capacity = this->capacity * 2;
	TElem* new_elems = new TElem[new_capacity];

	for (int i = 0; i < this->length; i++) {
		new_elems[i] = this->elems[i];
	}

	delete[] this->elems;
	this->elems = new_elems;
	this->capacity = new_capacity;
}

template<typename TElem>
DynamicVector<TElem>::~DynamicVector() {
	delete[] this->elems;
}

template<typename TElem>
void DynamicVector<TElem>::push_back(TElem elem) {
	if (this->length == this->capacity)
		resize();

	this->elems[this->length] = elem;
	this->length++;
}

template<typename TElem>
void DynamicVector<TElem>::erase(const IteratorVector<TElem>& it) {
	/*for (int i = pos; i < this->length - 1; i++)
		this->elems[i] = this->elems[i + 1];
	this->length--;*/

	const IteratorVector<TElem> begin = (*this);
	const int pos = distance(begin, it);

	for (int i = pos; i < this->length - 1; i++)
		this->elems[i] = this->elems[i + 1];
	this->length--;
}

template<typename TElem>
TElem& DynamicVector<TElem>::at(int pos) noexcept {
	return this->elems[pos];
}

template<typename TElem>
void DynamicVector<TElem>::set(TElem elem, int pos) {
	this->elems[pos] = elem;
}

template<typename TElem>
int DynamicVector<TElem>::size() const noexcept {
	return this->length;
}

template<typename TElem>
IteratorVector<TElem> DynamicVector<TElem>::begin() noexcept {
	return IteratorVector<TElem>(*this);
}

template<typename TElem>
IteratorVector<TElem> DynamicVector<TElem>::end() noexcept {
	return IteratorVector<TElem>(*this, this->length);
}

template<typename TElem>
IteratorVector<TElem> DynamicVector<TElem>::find(IteratorVector<TElem> begin, IteratorVector<TElem> end, TElem elem) noexcept {
	IteratorVector<TElem> it = begin;
	while (it != end) {
		if (it.element() == elem)
			return it;
		it.next();
	}
	return it;
}

template<typename TElem>
int DynamicVector<TElem>::distance(IteratorVector<TElem> begin, IteratorVector<TElem> end) noexcept {
	int pos = 0;
	while (begin != end) {
		pos++;
		begin.next();
	}
	return pos;
}

template<typename TElem>
IteratorVector<TElem>::IteratorVector(const DynamicVector<TElem>& v) noexcept : vector{ v } {};

template<typename TElem>
IteratorVector<TElem>::IteratorVector(const DynamicVector<TElem>& v, int pos) noexcept : vector{ v }, pos{ pos } {};

template<typename TElem>
bool IteratorVector<TElem>::valid() const noexcept {
	return this->pos < this->vector.length;
}

template<typename TElem>
TElem& IteratorVector<TElem>::element() const noexcept {
	return this->vector.elems[this->pos];
}

template<typename TElem>
void IteratorVector<TElem>::next() noexcept {
	this->pos++;
}

template<typename TElem>
TElem& IteratorVector<TElem>::operator*() {
	return this->element();
}

template<typename TElem>
IteratorVector<TElem>& IteratorVector<TElem>::operator++() {
	this->next();
	return *this;
}

template<typename TElem>
bool IteratorVector<TElem>::operator==(const IteratorVector& other) noexcept {
	return this->pos == other.pos;
}

template<typename TElem>
bool IteratorVector<TElem>::operator!=(const IteratorVector& other) noexcept {
	return !(*this == other);
}

template<typename TElem>
IteratorVector<TElem>& IteratorVector<TElem>::operator+(int new_pos) {
	for (int i = 0; i < new_pos; i++)
		this->pos++;
	return (*this);
}