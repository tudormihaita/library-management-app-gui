#pragma once
#include "Book.h"
#include "Exceptions.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using std::vector;

class Wishlist {
private:
	vector<Book> wishlistBooks;

public:

	//Constructor Wishlist
	Wishlist() = default;

	/*
	* Adauga o carte din biblioteca in wishlist
	* @param book - entitate Book existenta in biblioteca
	* @return -
	*/
	void addBookToWishlist(const Book& book);

	/*
	* Goleste wishlist-ul curent
	*/
	void emptyWishlist() noexcept;

	/*
	* Adauga un numar dat de carti random din lista de carti curenta, disponibile din biblioteca
	* @param allBooks - lista curenta de carti disponibile din biblioteca
			 howMany - int, numarul de carti ce urmeaza a fi adaugate in wishlist
	  @return -
	  @throws - RepoException daca nu exista suficiente carti disponibile in biblioteca
	*/
	void addRandomBooks(vector<Book> allBooks, int howMany);

	/*
	* Returneaza lista de carti curenta din wishlist
	* @param -
	* @return - o lista de carti regasite in wishlist
	*/
	const vector<Book>& getAllWishlistBooks() noexcept;

	/*
	* Returneaza numarul de carti din wishlistul curent
	*/
	int getWishlistSize() const noexcept;

	/*
	* Exporta wishlist-ul curent intr-un fisier CSV cu un nume dat
	* @param fileName - string, numele fisierului de export
	* @return -
	* @throws - RepoException daca fisierului cu numele fileName nu s-a putut deschide cu succes
	*/
	void exportBookWishlist(string fileName);

};