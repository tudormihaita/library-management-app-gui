#pragma once
#pragma warning (disable: 26446) // use gsl::at() instead of subscript operator []
#pragma warning (disable: 26458) // use gsl::at() instead of subscript operator []
#include <fstream>
#include <sstream>
#include <vector>
#include "FileRepository.h"
#include "Exceptions.h"

using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::remove_if;
using std::isspace;
using std::getline;

void FileBookRepository::loadFromFile() {
	ifstream fin(this->fileName);
	if (!fin.is_open()) {
		throw RepoException("Nu s-a putut deschide fisierul " + fileName + "!\n");
	}

	string line;

	if (fin.peek() == EOF) {
		return;
	}

	while (!fin.eof()) {
		string atribute;
		vector<string> atr;
		getline(fin, line);
		if (!line.empty()) {
			stringstream ss(line);
			while (getline(ss, atribute, ',')) {
				atr.push_back(atribute);
			}

			int id = -1;
			if (isdigit(atr[0].at(0)) && isdigit(atr[0].at(1)))
				id = stoi(atr[0].substr(0, 2));
			else
				id = stoi(atr[0].substr(0, 1));

			const string title = atr[1];
			const string author = atr[2];
			const string genre = atr[3];
			const string publisher = atr[4];
			const int releaseYear = stoi(atr[5]);

			const Book b{ id, title, author, genre, publisher, releaseYear };
			MemoryBookRepository::addBook(b);
		}
	}

	fin.close();
}

void FileBookRepository::writeToFile() {
	ofstream fout(this->fileName);

	if (!fout.is_open()) {
		throw RepoException("Nu s-a putut deschide fisierul " + fileName + "!\n");
	}

	for (const Book& book : getAllBooks()) {
		fout << book.getISBN() << "," << book.getTitle() << "," << book.getAuthor() << "," << book.getGenre() << ","
			<< book.getPublisher() << "," << book.getYear() << "\n";
	}

	fout.close();
}

void FileBookRepository::clearFile() {
	ofstream fout;

	fout.open(this->fileName, ofstream::out | ofstream::trunc);

	if (!fout.is_open()) {
		throw RepoException("Nu s-a putut deschide fisierul " + fileName + "!\n");
	}

	fout.close();
}