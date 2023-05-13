#pragma once
#include "Book.h"
#include "Repository.h"

//Clasa abstracta, metoda de undo este pur virtuala
class UndoAction {
public:
	virtual void doUndo() = 0;

	virtual ~UndoAction() = default;
};

//Clasa specifica operatiei de Undo pentru adaugare
class UndoAdd : public UndoAction {
private:
	Book addedBook;
	BookRepository& bookRepository;
public:
	UndoAdd(BookRepository& repo, const Book& book) : bookRepository{ repo }, addedBook{ book } {};

	void doUndo() override {
		bookRepository.deleteBook(addedBook);
	}
};

//Clasa specifica operatiei de Undo pentru stergere
class UndoDelete : public UndoAction {
private:
	Book deletedBook;
	BookRepository& bookRepository;
public:
	UndoDelete(BookRepository& repo, const Book& book) : bookRepository{ repo }, deletedBook{ book } {};

	void doUndo() override {
		bookRepository.addBook(deletedBook);
	}
};

//Clasa specifica operatiei de Undo pentru modificare
class UndoUpdate : public UndoAction {
private:
	Book oldBook;
	BookRepository& bookRepository;
public:
	UndoUpdate(BookRepository& repo, const Book& book) : bookRepository{ repo }, oldBook{ book } {};

	void doUndo() override {
		bookRepository.updateBook(oldBook);
	}
};