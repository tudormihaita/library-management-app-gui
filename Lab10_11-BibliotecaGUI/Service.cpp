#pragma once
#pragma warning (disable: 26823)
#include "Service.h"


void Library::storeBook(const int& id, const string& title, const string& author, const string& genre, const string& publisher, const int& year) {
	Book book{ id, title, author, genre, publisher, year };
	this->bookValidator.validateBook(book);
	this->bookRepository.addBook(book);

	this->undoActions.push_back(make_unique<UndoAdd>(bookRepository, book));
	notify();
}

const vector<Book> Library::getAllBooks() const {
	return this->bookRepository.getAllBooks();
}

const Book& Library::findBook(const string& ISBN) {
	return this->bookRepository.findBook(ISBN);
}

const Book Library::removeBook(const string& ISBN) {
	const Book deletedBook = this->bookRepository.findBook(ISBN);
	this->bookRepository.deleteBook(deletedBook);

	this->undoActions.push_back(make_unique<UndoDelete>(bookRepository, deletedBook));
	notify();

	return deletedBook;
}

Book Library::updateBook(const string& ISBN, const string& genreUpdated, const string& publisherUpdated) {
	Book oldBook = this->bookRepository.findBook(ISBN);
	Book updatedBook{ oldBook.getId(), oldBook.getTitle(), oldBook.getAuthor(), genreUpdated, publisherUpdated, oldBook.getYear() };
	this->bookValidator.validateBook(updatedBook);
	this->bookRepository.updateBook(updatedBook);

	this->undoActions.push_back(make_unique<UndoUpdate>(bookRepository, oldBook));
	notify();

	return oldBook;
}

int Library::getSize() const noexcept {
	return this->bookRepository.getSize();
}


vector<Book> Library::filterBooks(function<bool(const Book&)> filter) {
	const vector<Book>& allBooks = getAllBooks();
	vector<Book> filteredBooks;
	copy_if(allBooks.begin(), allBooks.end(), back_inserter(filteredBooks), filter);
	return filteredBooks;
}

vector<Book> Library::filterByReleaseYear(const int& yearMin, const int& yearMax) {
	return filterBooks([=](const Book& book) noexcept {
		return book.getYear() >= yearMin && book.getYear() <= yearMax;
		});
}

vector<Book> Library::filterByGenre(const string& filterGenre) {
	return filterBooks([filterGenre](const Book& book) noexcept {
		return book.getGenre() == filterGenre;
		});
}


vector<Book> Library::sortBooks(function<bool(const Book& b1, const Book& b2, bool reversed)> cmp, bool reversed) {
	vector<Book> sortedBooks{ this->bookRepository.getAllBooks() };
	sort(sortedBooks.begin(), sortedBooks.end(), [&](const Book& b1, const Book& b2) {
		return cmp(b1, b2, reversed);
		});

	return sortedBooks;
}


vector<Book> Library::sortByTitle(const bool& reversed) {
	vector<Book> sortedBooks{ this->bookRepository.getAllBooks() };
	sort(sortedBooks.begin(), sortedBooks.end(), [&](const Book& b1, const Book& b2) {
		if (reversed == false)
			return b1.getTitle() < b2.getTitle();
		else
			return b1.getTitle() > b2.getTitle();
		});

	return sortedBooks;
}

vector<Book> Library::sortByAuthor(const bool& reversed) {
	vector<Book> sortedBooks{ this->bookRepository.getAllBooks() };
	sort(sortedBooks.begin(), sortedBooks.end(), [](const Book& b1, const Book& b2) {
		return b1.getAuthor() < b2.getAuthor();
		});

	if (reversed == true) {
		reverse(sortedBooks.begin(), sortedBooks.end());
	}

	return sortedBooks;
}

vector<Book> Library::sortByReleaseYearAndGenre(const bool& reversed) {
	vector<Book> sortedBooks{ this->bookRepository.getAllBooks() };
	sort(sortedBooks.begin(), sortedBooks.end(), [](const Book& b1, const Book& b2) {
		if (b1.getYear() == b2.getYear())
			return b1.getGenre() < b2.getGenre();
		else
			return b1.getYear() < b2.getYear(); });

	if (reversed == true) {
		reverse(sortedBooks.begin(), sortedBooks.end());
	}

	return sortedBooks;
}

const vector<Book> Library::matchWishlistBooks(const string& title) {
	const vector<Book>& allBooks = getAllBooks();
	vector<Book> matchedBooks;

	copy_if(allBooks.begin(), allBooks.end(), back_inserter(matchedBooks), [&](const Book& book) {
		return book.getTitle() == title; });

	if (matchedBooks.size() > 0)
		return matchedBooks;
	else
		return {};
}

void Library::addToWishlist(const string& ISBN) {
	const Book& wishlistedBook = findBook(ISBN);
	currentWishlist.addBookToWishlist(wishlistedBook);
	notify();
}

void Library::emptyWishlist() noexcept {
	this->currentWishlist.emptyWishlist();
	notify();
}

const vector<Book>& Library::getWishlistBooks() noexcept {
	return this->currentWishlist.getAllWishlistBooks();
}

int Library::getWishlistSize() const noexcept {
	return this->currentWishlist.getWishlistSize();
}

void Library::addRandomToWishlist(int howMany) {
	this->currentWishlist.emptyWishlist();
	this->currentWishlist.addRandomBooks(this->getAllBooks(), howMany);
	notify();

}

void Library::exportWishlist(string fileName) {
	this->currentWishlist.exportBookWishlist(fileName);

}

unordered_map<string, BookReportDTO> Library::getBookReport() {
	unordered_map<string, BookReportDTO> bookReport;
	const vector<Book>& allBooks = this->bookRepository.getAllBooks();

	for (const Book& book : allBooks) {
		if (bookReport.find(book.getGenre()) == bookReport.end()) {
			bookReport.insert({ book.getGenre(), {book.getGenre()} });
			bookReport[book.getGenre()].setCount(bookReport[book.getGenre()].getCount() + 1);
		}
		else {
			bookReport[book.getGenre()].setCount(bookReport[book.getGenre()].getCount() + 1);
		}
	}

	return bookReport;
}

void Library::undo() {
	if (undoActions.empty()) {
		throw UndoException("Nu mai exista operatii pentru care sa se efectueze undo!\n");
	}

	undoActions.back()->doUndo();
	undoActions.pop_back();

	notify();
}


/* old genericSort for books */
//vector<Book> Library::sortBooks(function<bool(const Book& b1, const Book& b2)> cmp) {
//	vector<Book> sortedBooks{ this->bookRepository.getAllBooks() };
//	int sorted = 0;
//	while (!sorted) {
//		sorted = 1;
//		for (size_t i = 0; i < sortedBooks.size() - 1; i++) {
//			if (cmp(sortedBooks.at(i), sortedBooks.at(i+1))) {
//				sorted = 0;
//				Book aux = sortedBooks.at(i);
//				sortedBooks.at(i) = sortedBooks.at(i+1);
//				sortedBooks.at(i+1) = aux;
//			}
//		}
//	}
//
//	return sortedBooks;
//}