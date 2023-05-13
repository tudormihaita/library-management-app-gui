#pragma once
#include "Repository.h"
#include "FileRepository.h"
#include "LabRepository.h"
#include "UndoAction.h"
#include "Validator.h"
#include "Wishlist.h"
#include "Observer.h"
#include <functional>
#include <algorithm>
#include <memory>
#include <unordered_map>

using std::copy_if;
using std::sort;
using std::reverse;
using std::make_unique;
using std::function;
using std::unique_ptr;
using std::unordered_map;

//typedef bool (*CompareFct)(const Carte&, const Carte&, bool reversed);
//typedef bool (*FilterFct)(const Carte&, const Carte&);

class Library : public Observable {
private:
	//BookRepository& bookRepository;
	BookRepository& bookRepository;
	Validator& bookValidator;

	Wishlist currentWishlist;

	vector<unique_ptr<UndoAction>> undoActions;

public:
	//Constructor Default Service
	Library() = default;

	//Constructor Service
	Library(BookRepository& repo, Validator& validator) noexcept :
		bookRepository{ repo }, bookValidator{ validator } {};

	//Constructor Service prin copiere, sters pentru a nu permite copierea
	Library(const Library& other) = delete;

	//Destructor Service
	~Library() = default;

	/*
	* Creeaza o carte cu atributele date, o valideaza si o adauga in lista de carti
	* @param id - int, title - string, author - string, genre - string, publisher - string, release_year - int
	* @return -
	* @throws - RepoException daca cartea creata cu atributele date exista deja in lista
	*			ValidationException daca cel putin unul dintre atributele introduse este invalid
	*/
	void storeBook(const int& id, const string& title, const string& author, const string& genre, const string& publisher, const int& year);


	/*
	* Returneaza o lista cu toate cartile introduse in biblioteca
	* @return - lista cu toate cartile disponibile in biblioteca
	*/
	const vector<Book> getAllBooks() const;


	/*
	* Cauta o carte in biblioteca identificata prin codul unic ISBN
	* @param ISBN - string, cod de identificare unic pentru carti
	* @return - cartea cautata in cazul in care aceasta exista
	* @throws - RepoException daca cartea cautata nu exista in biblioteca
	*/
	const Book& findBook(const string& ISBN);


	/*
	* Cauta o carte in biblioteca prin codul unic ISBN si o elimina din lista daca aceasta exista
	* @param ISBN - string, cod de identificare unic pentru carti
	* @return - cartea stearsa in cazul in care aceasta a fost identificata cu succes
	* @throws - RepoException daca cartea cautata pentru stergere nu exista in lista
	*/
	const Book removeBook(const string& ISBN);


	/*
	* Cauta o carte in biblioteca prin codul unic ISBN si modifica valorile pentru gen si editura la noile valori date, daca acestea sunt valide
	* @param ISBN - string, cod de identificare unic pentru carti, genreUpdated - noua valoare pentru genul cartii, publisherUpdated - noua valoarea pentru editura cartii
	* @return - cartea gasita, inainte de modificare, daca aceasta a fost identificata cu succes
	* @throws - RepoException daca cartea cautata pentru modificare nu exista in lista
	*			ValidationException - daca valorile noi specificare pentru gen si editura nu sunt valide
	*/
	Book updateBook(const string& ISBN, const string& genreUpdated, const string& publisherUpdated);


	/*
	* Returneaza numarul de carti disponibile in biblioteca
	* @return - int, dimensiunea listei de carti din biblioteca
	*/
	int getSize() const noexcept;

	/*
	* Returneaza o lista de carti filtrata corespunzator dupa criteriul specificat
	* Functie generica de filtrare
	* @param filter - pointer la o functie de comparare a cartilor dupa criteriul de filtrare
	*				criteriilor de filtrare, astfel incat sa fie comparata cu cartile din bilbioteca
	* @return - lista de carti filtrate corespunzator
	*/
	vector<Book> filterBooks(function<bool(const Book&)> filter);

	/*
	* Returneaza o lista de carti filtrata astfel incat sa nu contina cartile publicate intre anii specificati
	* @param yearMin - int, anul minim de publicare
	*		 yearMax - int, anul maxim de publicare
	* @return - lista de carti filtrate corespunzator
	*/
	vector<Book> filterByReleaseYear(const int& yearMin, const int& yearMax);

	/*
	* Returneaza o lista de carti filtrata astfel incat sa nu contina cartile din genul specificat
	* @param filterGenre - string, genul de carte filtrat
	* @return - lista de carti filtrate corespunzator
	*/
	vector<Book> filterByGenre(const string& filterGenre);

	/*
	* Returneaza o lista de carti sortata dupa titlu, corespunzator criteriului specificat
	* @param reversed - bool, specificator de sortare crescatoare sau descrescatoare
	* @return - lista de carti sortate corespunzator
	*/
	vector<Book> sortByTitle(const bool& reversed);

	/*
	* Returneaza o lista de carti sortata dupa autor, corespunzator criteriului specificat
	* @param reversed - bool, specificator de sortare crescatoare sau descrescatoare
	* @return - lista de carti sortate corespunzator
	*/
	vector<Book> sortByAuthor(const bool& reversed);

	/*
	* Returneaza o lista de carti sortata dupa an de aparitie si gen, corespunzator criteriului specificat
	* @param reversed - bool, specificator de sortare crescatoare sau descrescatoare
	* @return - lista de carti sortate corespunzator
	*/
	vector<Book> sortByReleaseYearAndGenre(const bool& reversed);

	/*
	* Returneaza o lista de carti sortata corespunzator dupa criteriile specificate
	* @param cmp - pointer la o functie de comparare prin care se verifica indeplinirea relatiei de sortare dintre entitatile listei de carti din biblioteca
	* @return - lista de carti sortate corespunzator
	*/
	vector<Book> sortBooks(function<bool(const Book& b1, const Book& b2, bool reversed)> cmp, bool reversed);

	/*
	* Returneaza o lista de carti din biblioteca cu titlul cautat
	* @param title - string, titlul cartilor cautate
	* @return - o lista de carti din biblioteca cu titlul dat
	*/
	const vector<Book> matchWishlistBooks(const string& title);

	/*
	* Adauga o carte cu ISBN-ul dat in wishlist
	* @param ISBN - string, codul unic de identificare al cartii
	* @return -
	* @throws - RepoException, daca nu exista nicio carte in biblioteca cu ISBN-ul dat
	*/
	void addToWishlist(const string& ISBN);

	/*
	* Adauga un numar dat de carti random din biblioteca in wishlist
	* @param howMany - int, numarul de carti ce urmeaza a fi adaugate in wishlist
	* @return -
	*/
	void addRandomToWishlist(int howMany);

	/*
	* Goleste wishlist-ul curent
	* @param -
	* @return -
	*/
	void emptyWishlist() noexcept;

	/*
	* Returneaza o lista cu toate cartile din wishlist-ul curent
	* @param -
	* @return - lista cu toate cartile adaugate in wishlist
	*/
	const vector<Book>& getWishlistBooks() noexcept;

	/*
	* Returneaza numarul de carti din wishlist-ul curent
	* @param -
	* @return - int, dimensiunea wishlist-ului
	*/
	int getWishlistSize() const noexcept;

	/*
	* Creeaza un fisier de export care va contine cartile din wishlist-ul curent
	* @param fileName - string, numele fisierului in care se va efectua exportul
	* @return -
	* @throws - RepoException, daca nu s-a putut deschide cu succes fisierul cu numele dat
	*/
	void exportWishlist(string fileName);

	/*
	* Genereaza si returneaza o statistica a cartilor din biblioteca, organizate dupa gen
	* @param -
	* @return - bookReport, unordered_map, cu perechi de tipul <gen, BookDTO>, unde cheia este genul cartii, iar DTO-ul pentru carti
	*			contine genul si numarul de carti din fiecare gen existent in biblioteca
	*/
	unordered_map<string, BookReportDTO> getBookReport();


	/*
	* Inverseaza efectul ultimei operatii efectuate asupra listei de carti disponibile in bilioteca
	* @param -
	* @return -
	* @throws - UndoException, daca nu exista operatii pentru care sa se efectueze undo
	*/
	void undo();
};