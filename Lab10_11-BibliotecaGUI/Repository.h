#pragma once
#include "Book.h"
#include "Exceptions.h"
#include "DynamicVector.h"
#include "DoublyLinkedList.h"
#include <vector>
#include <algorithm>

using std::vector;
using std::find_if;


class BookRepository {
public:
	virtual void addBook(const Book& book) = 0;

	virtual const vector<Book> getAllBooks() = 0;

	virtual const Book& findBook(const string& ISBN) = 0;

	virtual void deleteBook(const Book& book) = 0;

	virtual void updateBook(const Book& book) = 0;

	virtual int getSize() = 0;

	virtual ~BookRepository() = default;


};


class MemoryBookRepository : public BookRepository {
private:
	vector<Book> booklist;

	//DynamicVector<Book> booklist;
	//DoublyLinkedList<Book> booklist;

public:
	//Constructor Default Repo
	MemoryBookRepository() = default;

	//Constructor Repo prin copiere, sters pentru a nu permite copierea
	MemoryBookRepository(const MemoryBookRepository& other) = delete;

	/*
	* Adauga o carte in lista
	* @param book - cartea care se adauga (Book)
	* @return -
	* @throws - RepoException daca o carte cu acelasi titlu, autor si an aparitie
				exista deja in lista
	*/
	void addBook(const Book& book) override;

	/*
	* Returneaza o lista cu toate cartile
	* @return - lista cu carti (Booklist)
	*/
	const vector<Book> getAllBooks() noexcept override;


	/*
	* Cauta o carte cu ISBN-ul dat
	* @param ISBN - string, cod unic de identificare a unei carti
	* @return - entitatea Book cu ISBN-ul cautat
	* @throws - RepoException daca nu exista nicio carte cu acel ISBN
	*/
	const Book& findBook(const string& ISBN) override;


	/*
	* Returneaza pozitia pe care se afla in lista o carte cautata
	* @param lookedUpBook - entitate Book cautata in lista
	* @return - int, pozitia in lista a cartii cautate
	* @throws - RepoException daca nu exista nicio carte identica cu cea data in lista
	*/
	int getBookPosition(const Book& lookedUpBook);


	/*
	* Sterge o carte data din lista
	* @param bookToDelete - entitate Book cautata in lista pentru a fi stearsa
	* @return -
	* @throws - RepoException daca nu exista cartea data in lista
	*/
	void deleteBook(const Book& bookToDelete) override;


	/*
	* Modifica genul si editura unei carti date
	* @param updatedBook - entitate Book cautata in lista pentru modificare
	* @return -
	* @throws - RepoException daca cartea data nu exista in lista
	*/
	void updateBook(const Book& updatedBook) override;


	/*
	* Returneaza numarul curent de carti din lista
	* @param -
	* @return - int, lungimea listei de carti (Booklist size)
	*/
	int getSize() noexcept override;


	//Destructor Repo
	virtual ~MemoryBookRepository() = default;
};
