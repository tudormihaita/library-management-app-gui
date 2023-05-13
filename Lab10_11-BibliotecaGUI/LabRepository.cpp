#pragma once
#pragma warning (disable: 26472) // don't use static_cast
#include "LabRepository.h"
#include <random>

using std::mt19937;
using std::uniform_real_distribution;
using std::random_device;

void LabRepository::generateRandom() {

	mt19937 mt{ random_device{}() };
	uniform_real_distribution<> dist(0, 1);


	const double prob = dist(mt);
	if (prob < probability) {
		throw RepoException("Nu s-a putut efectua operatia!\n");
	}
}


int LabRepository::getSize() {
	generateRandom();

	return static_cast<int>(this->booklist.size());
}

const Book& LabRepository::findBook(const string& ISBN) {
	generateRandom();

	if (this->booklist.find(ISBN) != this->booklist.end()) {
		return (*(this->booklist.find(ISBN))).second;
	}
	else {
		throw RepoException("Cartea cu ISBN-ul " + ISBN + " nu exista in lista!\n");
	}
}

void LabRepository::addBook(const Book& book) {
	generateRandom();

	//if (this->booklist.find(book.getISBN()) != this->booklist.end()) {
	for (auto& _book : this->booklist) {
		if (_book.second == book) {
			throw RepoException("Cartea cu titlul " + book.getTitle() + " si autorul " + book.getAuthor() +
				" aparuta in " + to_string(book.getYear()) + " exista deja in lista!\n");
		}
	}

	this->booklist.insert({ book.getISBN(), book });
}

const vector<Book> LabRepository::getAllBooks() {
	generateRandom();

	vector<Book> allBooks;

	for (auto& book : this->booklist) {
		allBooks.push_back(book.second);
	}

	return allBooks;
}

void LabRepository::deleteBook(const Book& bookToDelete) {
	generateRandom();

	if (this->booklist.find(bookToDelete.getISBN()) == this->booklist.end()) {
		throw RepoException("Cartea cu titlul " + bookToDelete.getTitle() + " si autorul " + bookToDelete.getAuthor() + " aparuta in "
			+ to_string(bookToDelete.getYear()) + " nu exista in lista!\n");
	}

	this->booklist.erase(this->booklist.find(bookToDelete.getISBN()));
}

void LabRepository::updateBook(const Book& updatedBook) {
	generateRandom();

	if (this->booklist.find(updatedBook.getISBN()) == this->booklist.end()) {
		throw RepoException("Cartea cu titlul " + updatedBook.getTitle() + " si autorul " + updatedBook.getAuthor() + " aparuta in "
			+ to_string(updatedBook.getYear()) + " nu exista in lista!\n");
	}

	this->booklist[updatedBook.getISBN()] = updatedBook;
}