#pragma once
#include <vector>
#include <string>
#include <random>
#include <Qtwidgets/QApplication>
#include <QLabel>
#include <QPainter>
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
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

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
	QPushButton* btnAddRandomSpinboxWishlist;
	QPushButton* btnAddRandomSliderWishlist;
	QPushButton* btnClearWishlist;
	QPushButton* btnCloseWishlist;
	QPushButton* btnReloadWishlist;

	QLabel* lblAddRandomToWishlist = new QLabel{ "Adauga carti random in wishlist:" };
	QSpinBox* generateRandomSpinbox;

	QListWidget* lstWishlist;

	QLabel* currentLabel;
	QSlider* sliderWishlist;

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

class WishlistDrawGUI : public QWidget, public Observer {
private:
	Library& bookService;

	QLayout* viewLayout = new QHBoxLayout;

	void initializeGUI() {
		this->setLayout(viewLayout);

		this->setFixedSize(500, 500);
		this->setWindowTitle("Wishlist Read-Only");

		QIcon iconWishlitReadOnly("wishlist-readonly.png");
		this->setWindowIcon(iconWishlitReadOnly);

		bookService.addObserver(this);
	}

public:
	WishlistDrawGUI(Library& srv) : bookService{ srv } {
		initializeGUI();
	}

	~WishlistDrawGUI() {
		bookService.removeObserver(this);
	}

	void update() override {
		qDeleteAll(this->viewLayout->findChildren<QObject*>());
		this->repaint();
	}


	void paintEvent(QPaintEvent*) override {
		QPainter painter{ this };

		mt19937 mt{ random_device{}() };
		uniform_int_distribution<> distH(0, this->height() - 100);
		uniform_int_distribution<> distW(0, this->width() - 100);

		for (int i = 0; i < bookService.getWishlistSize(); i++) {
			auto randH = distH(mt);
			auto randW = distW(mt);

			if (i % 3 == 0) {
				painter.setBrush(QBrush{ Qt::yellow });
			}
			else if (i % 3 == 1) {
				painter.setBrush(QBrush{ Qt::red });
			}
			else {
				painter.setBrush(QBrush{ Qt::blue });
			}

			painter.drawEllipse(randW, randH, 50, 50);
		}
	}
};