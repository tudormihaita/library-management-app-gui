#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <functional>
#include "Tests.h"
#include "Validator.h"
#include "Exceptions.h"
#include "Book.h"
#include "Wishlist.h"
#include "Repository.h"
#include "Service.h"

using std::ifstream;
using std::stringstream;
using std::getline;
using std::remove;

/* ----- TEMPLATE CONTAINERS TESTS ----- */

template<typename TContainer>
void addBooks(TContainer& c, int howMany) {
	for (int i = 0; i < howMany; i++) {
		Book b{ i, to_string(i) + "_title", to_string(i) + "_author", to_string(i) + "_genre", to_string(i) + "_publisher", 2000 + i };
		c.push_back(b);
	}
}

template<typename TContainer>
void testCreateCopyAssign() {
	TContainer c;
	addBooks(c, 10);

	assert(c.size() == 10);
	assert(c.at(1).getTitle() == "1_title");

	TContainer c1{ c };
	assert(c1.size() == 10);
	assert(c1.at(2).getAuthor() == "2_author");

	TContainer c2;
	c2 = c;
	assert(c2.size() == 10);
	assert(c2.at(3).getYear() == 2003);

	c2 = c1;
	assert(c2.size() == 10);

	TContainer c3;
	TContainer c4(c3);
	addBooks(c3, 1);
	TContainer c5(c3);

	c3 = c3;
}

template<typename TContainer>
void testMoveConstrAssign() {
	vector<TContainer> v;

	v.push_back(TContainer{});
	v.insert(v.begin(), TContainer{});

	assert(v.size() == 2);

	TContainer c;
	addBooks(c, 50);
	assert(c.size() == 50);
	c = TContainer{};
	assert(c.size() == 0);

	TContainer c1;
	addBooks(c1, 20);
	assert(c1.size() == 20);
	c = c1;
	assert(c1.size() == 20);
}

template<typename TContainer>
void testAddRemoveFindUpdate() {
	TContainer c;

	addBooks(c, 10);
	Book b1 = c.at(1);
	c.set(b1, 0);
	assert(c.at(0) == b1);

	Book b{ 12, "In Search of Lost Time", "Marcel Proust", "Drama", "Corint", 1967 };
	c.push_back(b);
	assert(c.size() == 11);
	assert(c.at(10) == b);

	auto bFound = c.find(c.begin(), c.end(), b);
	assert(bFound.element() == b);

	c.erase(bFound);
	assert(c.size() == 10);

	auto begin = c.begin();
	c.erase(begin);
	assert(c.size() == 9);

	const int length = c.distance(c.begin(), c.end());
	assert(length == c.size());
}

template<typename TContainer>
void runAllContainerTests() {
	testCreateCopyAssign<TContainer>();
	testMoveConstrAssign<TContainer>();
	testAddRemoveFindUpdate<TContainer>();
}

/*  --------------------------------------- */


void Tests::runAllTests() {
	std::cout << "---------------------------------------------\n";
	std::cout << "Ruleaza teste domain...\n";
	runDomainTests();
	std::cout << "Teste domain trecute cu succes!\n";
	std::cout << "Ruleaza teste validator...\n";
	runValidationTests();
	std::cout << "Teste validator trecute cu succes!\n";
	std::cout << "Ruleaza teste repository...\n";
	runMemoryRepoTests();
	runFileRepoTests();
	runLabRepoTests();
	std::cout << "Teste repository trecute cu succes!\n";
	std::cout << "Ruleaza teste service...\n";
	runServiceTests();
	std::cout << "Teste service trecute cu succes!\n";
	std::cout << "Ruleaza teste wishlist...\n";
	runWishlistTests();
	std::cout << "Teste wishlist trecute cu succes!\n";
	std::cout << "Ruleaza teste undo...\n";
	runUndoTests();
	std::cout << "Teste undo trecute cu succes!\n";
	std::cout << "Ruleaza teste containere template...\n";
	runAllContainerTests<DynamicVector<Book>>();
	runAllContainerTests<DoublyLinkedList<Book>>();
	std::cout << "Teste containere template trecute cu succes!\n";
	std::cout << "---------------------------------------------\n";

}

void Tests::runDomainTests() {
	Book book(9, "War and Peace", "Lev Tolstoi", "Nuvela istorica", "Humanitas", 1869);

	assert(book.getTitle() == "War and Peace");
	assert(book.getAuthor() == "Lev Tolstoi");
	assert(book.getGenre() == "Nuvela istorica");
	assert(book.getPublisher() == "Humanitas");
	assert(book.getYear() == 1869);
	assert(book.getId() == 9);
	assert(book.getISBN() == "9ROWL69");

	stringstream sout;
	sout << book;
	auto display = sout.str();
	//std::cout << display;
	assert(display == "| 9ROWL69 | War and Peace | Lev Tolstoi | Nuvela istorica | Humanitas | 1869 |");

	Book identical_book(9, "War and Peace", "Lev Tolstoi", "Fictiune", "Corint", 1869);
	assert(book == identical_book);

	Book copy_book = book;
	assert(copy_book.getTitle() == "War and Peace");
	assert(copy_book.getAuthor() == "Lev Tolstoi");
	assert(copy_book.getISBN() == book.getISBN());

	book.setYear(1871);
	assert(book.getYear() == 1871);

	book.setTitle("In Search of Lost Time");
	assert(book.getTitle() == "In Search of Lost Time");

	book.setAuthor("Marcel Proust");
	assert(book.getAuthor() == "Marcel Proust");

	book.setGenre("Fictiune");
	assert(book.getGenre() == "Fictiune");

	book.setPublisher("Art");
	assert(book.getPublisher() == "Art");

	BookReportDTO DTOBookDefault;
	assert(DTOBookDefault.getCount() == 0);

	BookReportDTO DTOBook("Fictiune");
	DTOBook.setCount(2);

	assert(DTOBook.getCount() == 2);
	assert(DTOBook.getGenre() == "Fictiune");
}

void Tests::runValidationTests() {
	Validator bookValidator;
	Book book(9, "War and Peace", "Lev Tolstoi", "Nuvela istorica", "Humanitas", 1869);
	//std::cout << book.get_ISBN() << "\n";

	bookValidator.validateBook(book);

	Book invalidBook(-2, "a", "a!!~", "b", "c", 10);
	try {
		bookValidator.validateBook(invalidBook);
		assert(false);
	}
	catch (ValidationException& ve) {
		assert(ve.get_error_messages() == "Numar identificare carte invalid, introduceti un numar cuprins intre 0 si 99!\n"
			"Titlu invalid, introduceti un sir de caractere valid!\nNume autor invalid, introduceti un sir de caractere valid!\n"
			"Gen invalid, introduceti un sir de caractere valid!\nEditura invalida, introduceti un sir de caractere valid!\n"
			"An invalid, introduceti un numar valid!\n");
	}

}

void Tests::runMemoryRepoTests() {
	MemoryBookRepository bookRepository;
	assert(bookRepository.getSize() == 0);
	Book book1{ 9, "War and Peace", "Lev Tolstoi", "Nuvela istorica", "Corint", 1869 };
	bookRepository.addBook(book1);
	assert(bookRepository.getSize() == 1);

	Book book2{ 12, "In Search of Lost Time", "Marcel Proust", "Fictiune", "Nemira", 1913 };
	bookRepository.addBook(book2);
	assert(bookRepository.getSize() == 2);

	Book identicalBook{ 17, "War and Peace", "Lev Tolstoi", "Biografie", "Art", 1869 };
	try {
		bookRepository.addBook(identicalBook);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul War and Peace si autorul Lev Tolstoi aparuta in 1869 exista deja in lista!\n");
	}
	assert(bookRepository.getSize() == 2);

	vector<Book> booklist = bookRepository.getAllBooks();
	assert(booklist.size() == 2);

	string lookedUpISBN = book1.getISBN();
	Book lookedUpBook = bookRepository.findBook(lookedUpISBN);
	assert(lookedUpBook == book1);

	try {
		Book missingBook = bookRepository.findBook("2ROIL15");
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu ISBN-ul 2ROIL15 nu exista in lista!\n");
	}

	bookRepository.deleteBook(book2);
	assert(bookRepository.getSize() == 1);

	try {
		bookRepository.deleteBook(book2);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul In Search of Lost Time si autorul Marcel Proust aparuta in 1913 nu exista in lista!\n");
	}

	Book updatedBook{ 9, "War and Peace", "Lev Tolstoi", "Poveste de razboi", "Litera", 1869 };
	bookRepository.updateBook(updatedBook);
	string updatedISBN = updatedBook.getISBN();
	assert(bookRepository.findBook(updatedISBN).getGenre() == "Poveste de razboi");

	try {
		Book missingBook{ 23,  "To Kill a Mockingbird", "Harper Lee", "Thriller", "Art", 1960 };
		bookRepository.updateBook(missingBook);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul To Kill a Mockingbird si autorul Harper Lee aparuta in 1960 nu exista in lista!\n");
	}
}

void Tests::runFileRepoTests() {
	try {
		FileBookRepository bookRepository("");
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Nu s-a putut deschide fisierul !\n");
	}

	FileBookRepository bookRepository("test_books.txt");

	assert(bookRepository.getSize() == 0);
	Book book1{ 9, "A Song of Ice & Fire", "George R. R. Martin", "Fictiune", "Nemira", 1997 };
	bookRepository.addBook(book1);
	assert(bookRepository.getSize() == 1);

	Book book2{ 12, "In Search of Lost Time", "Marcel Proust", "Fictiune", "Nemira", 1913 };
	bookRepository.addBook(book2);
	assert(bookRepository.getSize() == 2);

	Book identicalBook{ 17, "A Song of Ice & Fire", "George R. R. Martin", "Fantasy", "Art", 1997 };
	try {
		bookRepository.addBook(identicalBook);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul A Song of Ice & Fire si autorul George R. R. Martin aparuta in 1997 exista deja in lista!\n");
	}
	assert(bookRepository.getSize() == 2);

	vector<Book> booklist = bookRepository.getAllBooks();
	assert(booklist.size() == 2);

	string lookedUpISBN = book1.getISBN();
	Book lookedUpBook = bookRepository.findBook(lookedUpISBN);
	assert(lookedUpBook == book1);

	try {
		Book missingBook = bookRepository.findBook("2ROIL15");
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu ISBN-ul 2ROIL15 nu exista in lista!\n");
	}

	bookRepository.deleteBook(book2);
	assert(bookRepository.getSize() == 1);

	try {
		bookRepository.deleteBook(book2);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul In Search of Lost Time si autorul Marcel Proust aparuta in 1913 nu exista in lista!\n");
	}

	Book updatedBook{ 9, "A Song of Ice & Fire", "George R. R. Martin", "Fantasy medieval", "Litera", 1997 };
	bookRepository.updateBook(updatedBook);
	string updatedISBN = updatedBook.getISBN();
	assert(bookRepository.findBook(updatedISBN).getGenre() == "Fantasy medieval");

	try {
		Book missingBook{ 23,  "To Kill a Mockingbird", "Harper Lee", "Thriller", "Art", 1960 };
		bookRepository.updateBook(missingBook);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul To Kill a Mockingbird si autorul Harper Lee aparuta in 1960 nu exista in lista!\n");
	}

	bookRepository.clearFile();
}


void Tests::runLabRepoTests() {
	LabRepository bookRepository{ 0.0 };

	assert(bookRepository.getSize() == 0);

	Book book1{ 9, "War and Peace", "Lev Tolstoi", "Nuvela istorica", "Corint", 1869 };
	bookRepository.addBook(book1);
	assert(bookRepository.getSize() == 1);

	Book book2{ 12, "In Search of Lost Time", "Marcel Proust", "Fictiune", "Nemira", 1913 };
	bookRepository.addBook(book2);
	assert(bookRepository.getSize() == 2);

	Book identicalBook{ 65, "War and Peace", "Lev Tolstoi", "Nuvela istorica", "Macmillan", 1869 };
	try {
		bookRepository.addBook(identicalBook);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul War and Peace si autorul Lev Tolstoi aparuta in 1869 exista deja in lista!\n");
	}
	assert(bookRepository.getSize() == 2);

	vector<Book> booklist = bookRepository.getAllBooks();
	assert(booklist.size() == 2);

	string lookedUpISBN = book1.getISBN();
	Book lookedUpBook = bookRepository.findBook(lookedUpISBN);
	assert(lookedUpBook == book1);

	try {
		Book missingBook = bookRepository.findBook("2ROIL15");
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu ISBN-ul 2ROIL15 nu exista in lista!\n");
	}

	bookRepository.deleteBook(book2);
	assert(bookRepository.getSize() == 1);

	try {
		bookRepository.deleteBook(book2);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul In Search of Lost Time si autorul Marcel Proust aparuta in 1913 nu exista in lista!\n");
	}

	Book updatedBook{ 9, "War and Peace", "Lev Tolstoi", "Poveste de razboi", "Litera", 1869 };
	bookRepository.updateBook(updatedBook);
	string updatedISBN = updatedBook.getISBN();
	assert(bookRepository.findBook(updatedISBN).getGenre() == "Poveste de razboi");

	try {
		Book missingBook{ 23,  "To Kill a Mockingbird", "Harper Lee", "Thriller", "Art", 1960 };
		bookRepository.updateBook(missingBook);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul To Kill a Mockingbird si autorul Harper Lee aparuta in 1960 nu exista in lista!\n");
	}

	LabRepository bookRepositoryInvalid{ 1.0 };
	try {
		bookRepositoryInvalid.addBook(book2);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Nu s-a putut efectua operatia!\n");
	}
}

void Tests::runServiceTests() {
	FileBookRepository bookRepository("test_books.txt");
	Validator bookValidator;
	Library bookLibrary{ bookRepository, bookValidator };

	assert(bookLibrary.getSize() == 0);

	bookLibrary.storeBook(23, "To Kill a Mockingbird", "Harper Lee", "Thriller", "Humanitas", 1960);
	bookLibrary.storeBook(17, "War and Peace", "Lev Tolstoi", "War story", "Corint", 1869);

	assert(bookLibrary.getSize() == 2);
	vector<Book> booklist = bookLibrary.getAllBooks();
	assert(booklist.size() == 2);

	try {
		bookLibrary.storeBook(31, "To Kill a Mockingbird", "Harper Lee", "Roman juridic", "Art", 1960);
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu titlul To Kill a Mockingbird si autorul Harper Lee aparuta in 1960 exista deja in lista!\n");
	}

	Book oldBook = bookLibrary.updateBook("23ROTH60", "Roman juridic", "Corint");
	assert(oldBook.getISBN() == "23ROTH60");
	assert(bookLibrary.findBook("23ROTH60").getGenre() == "Roman juridic");
	assert(bookLibrary.findBook("23ROTH60").getPublisher() == "Corint");

	try {
		bookLibrary.updateBook("19ROIM71", "Roman psihologic", "Arthur");
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Cartea cu ISBN-ul 19ROIM71 nu exista in lista!\n");
	}

	const Book& deletedBook = bookLibrary.removeBook("17ROWL69");
	assert(deletedBook.getId() == 17);
	assert(deletedBook.getISBN() == "17ROWL69");
	assert(bookLibrary.getSize() == 1);

	try {
		bookLibrary.removeBook("19ROIM71");
	}
	catch (RepoException&) {
		assert(true);
	}

	bookLibrary.removeBook("23ROTH60");
	assert(bookLibrary.getSize() == 0);

	bookLibrary.storeBook(14, "In Search of Lost Time", "Marcel Proust", "Fictiune filosofica", "Humanitas", 1890);
	bookLibrary.storeBook(67, "The Trial", "Franz Kafka", "Fictiune filosofica", "Art", 1925);
	bookLibrary.storeBook(6, "Les Miserables", "Victor Hugo", "Tragedie", "Humanitas", 1862);
	bookLibrary.storeBook(19, "The Castle", "Franz Kafka", "Comedie", "Corint", 1926);

	assert(bookLibrary.getSize() == 4);


	vector<Book> filteredBooksReleaseYear = bookLibrary.filterByReleaseYear(1860, 1900);
	assert(filteredBooksReleaseYear.size() == 2);


	vector<Book> filteredBooksGenre = bookLibrary.filterByGenre("Fictiune filosofica");
	assert(filteredBooksGenre.size() == 2);


	vector<Book> sortedBooksByTitle = bookLibrary.sortByTitle(false);
	assert(sortedBooksByTitle.at(0).getTitle() == "In Search of Lost Time");
	vector<Book> sortedBooksByTitleReversed = bookLibrary.sortByTitle(true);
	assert(sortedBooksByTitleReversed.at(0).getTitle() == "The Trial");

	vector<Book> sortedBooksByAuthor = bookLibrary.sortByAuthor(false);
	assert(sortedBooksByAuthor.at(0).getAuthor() == "Franz Kafka");
	vector<Book> sortedBooksByAuthorReversed = bookLibrary.sortByAuthor(true);
	assert(sortedBooksByAuthorReversed.at(0).getAuthor() == "Victor Hugo");

	bookLibrary.storeBook(96, "Madame Bovary", "Gustave Flaubert", "Tragedie", "Humanitas", 1862);
	bookLibrary.storeBook(12, "The Divine Comedy", "Dante Alighieri", "Drama", "Arthur", 1862);
	bookLibrary.storeBook(2, "The Brothers Karamazov", "Fyodor Dostoyevsky", "Tragic", "Humanitas", 1862);

	vector<Book> sortedBooksByReleaseYearAndGenre = bookLibrary.sortByReleaseYearAndGenre(false);
	assert(sortedBooksByReleaseYearAndGenre.at(0).getYear() == 1862);
	vector<Book> sortedBooksByReleaseYearAndGenreReversed = bookLibrary.sortByReleaseYearAndGenre(true);
	assert(sortedBooksByReleaseYearAndGenreReversed.at(0).getYear() == 1926);

	vector<Book> genericSortByReleaseYearAndGenre = bookLibrary.sortBooks(cmpByReleaseYearAndGenre, false);
	assert(genericSortByReleaseYearAndGenre.at(0).getYear() == 1862);

	vector<Book> genericSortByReleaseYearAndGenreReversed = bookLibrary.sortBooks(cmpByReleaseYearAndGenre, true);
	assert(genericSortByReleaseYearAndGenreReversed.at(0).getYear() == 1926);

	vector<Book> genericSortByAuthor = bookLibrary.sortBooks(cmpByAuthor, false);
	assert(genericSortByAuthor.at(0).getAuthor() == "Dante Alighieri");

	vector<Book> genericSortByAuthorReversed = bookLibrary.sortBooks(cmpByAuthor, true);
	assert(genericSortByAuthorReversed.at(0).getAuthor() == "Victor Hugo");


	vector<Book> genericSortByTitle = bookLibrary.sortBooks(cmpByTitle, false);
	assert(genericSortByTitle.at(0).getTitle() == "In Search of Lost Time");

	vector<Book> genericSortByTitleReversed = bookLibrary.sortBooks(cmpByTitle, true);
	assert(genericSortByTitleReversed.at(0).getTitle() == "The Trial");

	unordered_map<string, BookReportDTO> bookReport = bookLibrary.getBookReport();
	assert(bookReport["Fictiune filosofica"].getCount() == 2);

	bookRepository.clearFile();

}

void Tests::runWishlistTests() {
	FileBookRepository bookRepository("test_books.txt");
	Validator bookValidator;
	Library bookService{ bookRepository, bookValidator };

	try {
		bookService.addRandomToWishlist(6);
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}

	bookService.storeBook(23, "To Kill a Mockingbird", "Harper Lee", "Thriller", "Humanitas", 1960);
	bookService.storeBook(17, "War and Peace", "Lev Tolstoi", "War story", "Corint", 1869);
	bookService.storeBook(15, "War and Peace", "Lev Tolstoi", "War story", "Arthur", 1887);
	bookService.storeBook(14, "In Search of Lost Time", "Marcel Proust", "Fictiune filosofica", "Humanitas", 1890);
	bookService.storeBook(67, "The Trial", "Franz Kafka", "Fictiune filosofica", "Art", 1925);
	bookService.storeBook(6, "Les Miserables", "Victor Hugo", "Tragedie", "Humanitas", 1862);
	bookService.storeBook(19, "The Castle", "Franz Kafka", "Comedie", "Corint", 1926);

	vector<Book> matchedBooks = bookService.matchWishlistBooks("War and Peace");
	assert(matchedBooks.size() == 2);

	assert(bookService.matchWishlistBooks("Anna Karenina").size() == 0);

	bookService.addToWishlist("23ROTH60");
	assert(bookService.getWishlistBooks().size() == 1);

	try {
		bookService.addToWishlist("23ROTH60");
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}

	bookService.emptyWishlist();
	assert(bookService.getWishlistBooks().size() == 0);

	try {
		bookService.addToWishlist("14ROAH47");
		assert(false);
	}
	catch (RepoException&) {
		assert(true);
	}

	bookService.addRandomToWishlist(3);
	assert(bookService.getWishlistSize() == 3);

	bookService.exportWishlist("test_export.txt");

	ifstream fin("test_export.txt");

	string line;
	int lineNr = 0;
	while (getline(fin, line)) {
		if (!line.empty())
			lineNr++;
	}

	assert(lineNr == 3);
	fin.close();

	try {
		bookService.exportWishlist("");
		assert(false);
	}
	catch (RepoException& re) {
		assert(re.get_error_message() == "Nu s-a putut deschide fisierul!\n");

	}

	bookRepository.clearFile();
	//remove("test_export.txt");
}

void Tests::runUndoTests() {
	FileBookRepository bookRepository("test_books.txt");
	Validator bookValidator;
	Library bookService{ bookRepository, bookValidator };

	try {
		bookService.undo();
	}
	catch (UndoException& ue) {
		assert(ue.get_error_message() == "Nu mai exista operatii pentru care sa se efectueze undo!\n");
	}

	bookService.storeBook(23, "To Kill a Mockingbird", "Harper Lee", "Thriller", "Humanitas", 1960);
	bookService.storeBook(17, "War and Peace", "Lev Tolstoi", "War story", "Corint", 1869);
	assert(bookService.getSize() == 2);

	bookService.undo();
	assert(bookService.getSize() == 1);

	bookService.storeBook(14, "In Search of Lost Time", "Marcel Proust", "Fictiune filosofica", "Humanitas", 1890);
	bookService.storeBook(67, "The Trial", "Franz Kafka", "Fictiune filosofica", "Art", 1925);
	assert(bookService.getSize() == 3);
	bookService.removeBook("14ROIM90");

	try {
		auto foundBook = bookService.findBook("14ROIM90");
		assert(false);
	}
	catch (const RepoException&) {
		assert(true);

	}

	assert(bookService.getSize() == 2);

	bookService.undo();
	assert(bookService.getSize() == 3);

	bookService.updateBook("67ROTF25", "Roman psihologic", "Macmillan");
	assert(bookService.findBook("67ROTF25").getGenre() == "Roman psihologic");
	assert(bookService.findBook("67ROTF25").getPublisher() == "Macmillan");

	bookService.undo();
	assert(bookService.findBook("67ROTF25").getGenre() == "Fictiune filosofica");
	assert(bookService.findBook("67ROTF25").getPublisher() == "Art");

	bookRepository.clearFile();
}