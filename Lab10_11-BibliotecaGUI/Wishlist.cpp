#pragma once
#pragma warning (disable: 26823) // dereferencing null pointer
#pragma warning (disable: 26472) // static_cast arithmetic conversion
#include "Wishlist.h"

using std::shuffle;
using std::find_if;
using std::default_random_engine;
using std::random_device;
using std::ofstream;

void Wishlist::addBookToWishlist(const Book& book) {
	if (find_if(this->wishlistBooks.begin(), this->wishlistBooks.end(), [book](const Book& _book) noexcept {
		return book == _book;  }) != this->wishlistBooks.end()) {
		throw RepoException("Cartea cu titlul " + book.getTitle() + " si autorul " + book.getAuthor() +
			" aparuta in " + to_string(book.getYear()) + " exista deja in wishlist!\n");
	}

	this->wishlistBooks.push_back(book);
}

void Wishlist::emptyWishlist() noexcept {
	this->wishlistBooks.clear();
}

const vector<Book>& Wishlist::getAllWishlistBooks() noexcept {
	return this->wishlistBooks;
}

int Wishlist::getWishlistSize() const noexcept {
	return static_cast<int>(this->wishlistBooks.size());
}


void Wishlist::addRandomBooks(vector<Book> allBooks, int howMany) {
	if (allBooks.size() < howMany) {
		throw RepoException("Nu exista carti suficiente disponibile in oferta, nu s-a putut crea wishlist-ul!\n");
	}
	else {
		shuffle(allBooks.begin(), allBooks.end(), default_random_engine(random_device{}()));
		while (wishlistBooks.size() < howMany && allBooks.size() > 0) {
			wishlistBooks.push_back(allBooks.back());
			allBooks.pop_back();
		}
	}
}

void Wishlist::exportBookWishlist(string fileName) {
	ofstream out(fileName, std::ios::trunc);
	if (!out.is_open()) {
		throw RepoException("Nu s-a putut deschide fisierul!\n");
	}

	for (const Book& book : getAllWishlistBooks()) {
		out << book.getISBN() << ", " << book.getTitle() << ", " << book.getAuthor() << ", " << book.getGenre() << ", "
			<< book.getPublisher() << ", " << book.getYear() << "\n";
	}

	out.close();
}