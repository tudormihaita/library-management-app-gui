#pragma once
#pragma warning (disable: 26823)
#include <string>
#include <vector>

using std::string;
using std::vector;

/*
* Clasa de exceptii specifice Validatorului
*/
class ValidationException {
	vector<string> error_msg;
public:
	ValidationException(vector<string>& error_messages) :
		error_msg{ error_messages } {};

	string get_error_messages() {
		string errors = "";
		for (const string& e : error_msg) {
			errors += e + "\n";
		}

		return errors;
	}

};


/*
* Clasa de exceptii specifice Repo
*/
class RepoException {
private:
	const string error_msg;
public:
	RepoException(const string& error) : error_msg{ error } {};

	string get_error_message() {
		return this->error_msg;
	}

};

/*
* Clasa de exceptii specifice UndoAction
*/
class UndoException {
private:
	const string error_msg;
public:
	UndoException(const string& error) : error_msg{ error } {};

	string get_error_message() {
		return this->error_msg;
	}
};


class InvalidInputException {
private:
	const string error_msg;
public:
	InvalidInputException(const string& error) : error_msg{ error } {};

	operator string() {
		return this->error_msg;
	}
};