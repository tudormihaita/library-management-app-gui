#pragma once
#include "Repository.h"
#include "Book.h"
#include <map>

using std::map;

class LabRepository : public BookRepository {
private:
	//reprezentare clasa LabRepository, foloseste un container de tip map pentru stocarea cartilor 
	//si un numar real intre 0 si 1, echivalent probabilitatii de efectuare a unei operatii repo 
	map<string, Book> booklist;
	double probability;

	//Metoda privata de generare a unui numar random si compararea acestuia cu probabilitatea data ca atribut
	//Daca numarul generat va fi mai mic decat probabilitatea, operatia nu se va efectua si se va arunca exceptie
	void generateRandom();

public:
	LabRepository(double prob) : BookRepository(), probability{ prob } {};

	LabRepository(const LabRepository& other) = delete;

	void addBook(const Book& book) override;

	const vector<Book> getAllBooks() override;

	const Book& findBook(const string& ISBN) override;

	void deleteBook(const Book& bookToDelete) override;

	void updateBook(const Book& updatedBook) override;

	int getSize() override;

	~LabRepository() = default;
};