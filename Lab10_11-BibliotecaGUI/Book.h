#pragma once
#include <string>
#include <iostream>
#include <fstream>

using std::ostream;
using std::ofstream;
using std::string;
using std::to_string;


class Book {
private:
	string ISBN;

	int id = 0;
	string title;
	string author;
	string genre;
	string publisher;
	int year = 0;

	static int nrEntities;

	friend ostream& operator<<(ostream& os, const Book& b);

public:

	//Default Constructor
	Book() = default;

	//Class Constructor
	Book(const int& id, const string& title, const string& author, const string& genre, const string& publisher, const int& year) :
		id{ id },
		title{ title },
		author{ author },
		genre{ genre },
		publisher{ publisher },
		year{ year } {
		if (title.length() > 0 && author.length() > 0) {
			ISBN = to_string(id); ISBN += "RO"; ISBN += this->title.at(0); ISBN += this->author.at(0);
			if (to_string(this->year).length() >= 2)
				ISBN += to_string(this->year).substr(to_string(this->year).size() - 2);
		}
	};

	//Copy Constructor
	Book(const Book& book) :
		ISBN{ book.ISBN },
		id{ book.id },
		title{ book.title },
		author{ book.author },
		genre{ book.genre },
		publisher{ book.publisher },
		year{ book.year } {
		//std::cout << "Copy Constructor called here!!\n";
	};

	//Move Constructor
	Book(Book&& other) = default;

	//Book Class Copy Assignment Operator
	Book& operator=(const Book&) = default;

	//Book Class Move Assignment Operator
	Book& operator=(Book&& other) = default;

	//Destructor Book
	~Book() = default;

	//Overload operator == pentru verificarea egalitatii dintre 2 carti prin compararea atributelor unice
	bool operator==(const Book& other) const noexcept {
		return this->title == other.title && this->author == other.author && this->year == other.year;
	}

	//Overload operator != pentru verificarea inegalitatii dintre 2 carti prin compararea atributelor unice
	bool operator!=(const Book& other) const noexcept {
		return this->title != other.title || this->author != other.author || this->year != other.year;
	}

	//Returneaza ISBN-ul unei carti
	string getISBN() const;

	//Returneaza id-ul unei carti
	int getId() const noexcept;

	//Returneaza titlul unei carti
	string getTitle() const;

	//Returneaza autorul unei carti
	string getAuthor() const;

	//Returneaza genul unei carti
	string getGenre() const;

	//Returneaza editura unei carti
	string getPublisher() const;

	//Returneaza anul aparitiei unei carti
	int getYear() const noexcept;

	//Seteaza titlul unei carti la valoarea string-ului dat
	void setTitle(const string& newTitle);

	//Seteaza autorul unei carti la valoarea string-ului dat
	void setAuthor(const string& newAuthor);

	//Seteaza genul unei carti la valoarea string-ului dat
	void setGenre(const string& newGenre);

	//Seteaza editura uei carti la valorea string-ului dat
	void setPublisher(const string& newPublisher);

	//Seteaza anul aparitiei unei carti la valoarea numarului intreg dat
	void setYear(const int& newReleaseYear) noexcept;

};

//Supraincarcare operator de insertie pentru afisarea unei entitati Book cu toate atributele sale
ostream& operator<<(ostream& os, const Book& b);

//Functie de comparare a titlurilor cartilor pentru sortare dupa titlu
bool cmpByTitle(const Book& b1, const Book& b2, bool reversed);

//Functie de comparare a autorilor cartilor pentru sortare dupa autor
bool cmpByAuthor(const Book& b1, const Book& b2, bool reversed);

//Functie de comparare a anul aparitiei si a genului cartilor pentru sortare dupa an+gen
bool cmpByReleaseYearAndGenre(const Book& b1, const Book& b2, bool reversed);


//Clasa de tip Data Transfer Object pentru entitati Book
class BookReportDTO {
private:
	string genre;
	int count = 0;

public:
	BookReportDTO() = default;

	BookReportDTO(string genre) : genre{ genre } {
		count = 0;
	}

	string getGenre() const noexcept {
		return this->genre;
	}

	int getCount() const noexcept {
		return this->count;
	}

	void setCount(int oldCount) noexcept {
		this->count = oldCount;
	}
};
