#pragma once
#pragma warning (disable: 26823)
#include <string>
#include <vector>
#include <regex>
#include "Validator.h"

using std::string;
using std::regex;

string Validator::validateId(const int& id) const {
	if (id < 0 || id > 100)
		return "Numar identificare carte invalid, introduceti un numar cuprins intre 0 si 99!";
	else
		return "";
}

string Validator::validateTitle(const string& title) const {
	regex invalid_characters("[0-9;'=~^`?|/+<>,@#]+");
	if (title.length() < 2 || regex_search(title, invalid_characters))
		return "Titlu invalid, introduceti un sir de caractere valid!";
	else
		return "";
}

string Validator::validateAuthor(const string& author) const {
	regex invalid_characters("[0-9:;'=~^!`?|/+<>,@#&()]+");
	if (author.length() < 2 || regex_search(author, invalid_characters))
		return "Nume autor invalid, introduceti un sir de caractere valid!";
	else
		return "";
}

string Validator::validateGenre(const string& genre) const {
	regex invalid_characters("[0-9;'=~^!`?|/.+<>,@#]+");
	if (genre.length() < 2 || regex_search(genre, invalid_characters))
		return "Gen invalid, introduceti un sir de caractere valid!";
	else
		return "";
}

string Validator::validatePublisher(const string& publisher) const {
	regex invalid_characters("[();'=~^!`?|/.+<>,@#]+");

	if (publisher.length() < 2 || regex_search(publisher, invalid_characters))
		return "Editura invalida, introduceti un sir de caractere valid!";
	else
		return "";
}

string Validator::validateYear(const int& year) const {
	if (year <= 1000 || year > 2023)
		return "An invalid, introduceti un numar valid!";
	else
		return "";
}

void Validator::validateBook(const Book& book) const {
	vector<string> errors;
	if (validateId(book.getId()).length() > 0)
		errors.push_back(validateId(book.getId()));
	if (validateTitle(book.getTitle()).length() > 0)
		errors.push_back(validateTitle(book.getTitle()));
	if (validateAuthor(book.getAuthor()).length() > 0)
		errors.push_back(validateAuthor(book.getAuthor()));
	if (validateGenre(book.getGenre()).length() > 0)
		errors.push_back(validateGenre(book.getGenre()));
	if (validatePublisher(book.getPublisher()).length() > 0)
		errors.push_back(validatePublisher(book.getPublisher()));
	if (validateYear(book.getYear()).length() > 0)
		errors.push_back(validateYear(book.getYear()));

	if (errors.size() > 0)
		throw ValidationException(errors);
}