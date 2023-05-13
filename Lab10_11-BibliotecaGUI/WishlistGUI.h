#pragma once
#include <vector>
#include <string>
#include <Qtwidgets/QApplication>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QListWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include "Observer.h"
#include "Service.h"

using std::vector;
using std::string;

class WishlistGUI : public QWidget, public Observer {
private:
	Library& bookService;

	QGroupBox* viewBox = new QGroupBox(tr("Vizualizare carti"));
	QGroupBox* booklistBox = new QGroupBox(tr("Lista de carti"));
	QGroupBox* wishlistBox = new QGroupBox(tr("Lista de carti Wishlist"));


	QGroupBox* wishlistCommandsBox = new QGroupBox(tr("Comenzi wishlist"));
	QGroupBox* wishlistOperationsBox = new QGroupBox(tr("Operatii wishlist"));
	QGroupBox* wishlistUtilitiesBox = new QGroupBox(tr("Utilitati wishlist"));
	QGroupBox* wishlistViewBox = new QGroupBox(tr("Vizualizare wishlist"));
	//QGroupBox* sliderWishlistBox = new QGroupBox(tr("Generare wishlist"));

	QLabel* lblAddToWishlist = new QLabel{ "Adaugare carte in wishlist dupa titlu:" };
	QLineEdit* editTitleWishlist;

	QLabel* lblExportWishlist = new QLabel{ "Exporta wishlist in fisier:" };
	QLineEdit* editExportFileName;

	QPushButton* btnMatchWishlistBooks;
	QPushButton* btnAddToWishlist;
	QPushButton* btnExportWishlist;
	QPushButton* btnAddRandomToWishlist;
	QPushButton* btnClearWishlist;
	QPushButton* btnCloseWishlist;
	QPushButton* btnReloadWishlist;

	QLabel* lblAddRandomToWishlist = new QLabel{ "Adauga carti random in wishlist:" };
	QSpinBox* generateRandomSpinbox;

	QListWidget* lstWishlist;

	/*QLabel* minimumLabel;
	QLabel* maximumLabel;
	QLabel* currentLabel;
	QSlider* sliderWishlist;*/

	void update() override {
		reloadWishlist(bookService.getWishlistBooks());
	}

	void intializeGUI();

	void connectSignalsSlots();

	void reloadWishlist(vector<Book> wishlist);

public:
	WishlistGUI(Library& srv) : bookService{ srv } {
		intializeGUI();
		connectSignalsSlots();
		reloadWishlist(bookService.getWishlistBooks());
	}

	void matchWishlistBooksGUI();
	void addWishlistGUI();

	~WishlistGUI() {
		bookService.removeObserver(this);
	}
};