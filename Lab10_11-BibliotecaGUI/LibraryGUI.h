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
#include <QRadioButton>
#include "Service.h"
#include "Observer.h"
#include "WishlistGUI.h"
#include "TableViewModel.h"

using std::vector;
using std::string;

class LibraryGUI : public QWidget, public Observer {
private:
	Library& bookService;

	QGroupBox* commandsBox = new QGroupBox(tr("Comenzi"));
	QGroupBox* operationsBox = new QGroupBox(tr("Operatii"));
	QGroupBox* filterBox = new QGroupBox(tr("Filtrari"));
	QGroupBox* sortBox = new QGroupBox(tr("Sortari"));
	QGroupBox* utilitiesBox = new QGroupBox(tr("Utilitati"));

	QGroupBox* reportBox = new QGroupBox(tr("Statistici Genuri"));

	QGroupBox* viewBox = new QGroupBox(tr("Vizualizare carti"));
	QGroupBox* booklistBox = new QGroupBox(tr("Lista de carti"));
	QGroupBox* wishlistBox = new QGroupBox(tr("Lista de carti Wishlist"));


	QPushButton* btnAdd;
	QPushButton* btnUpdate;
	QPushButton* btnDelete;
	QPushButton* btnFind;


	QLabel* lblId = new QLabel{ "Id carte:" };
	QLabel* lblTitle = new QLabel{ "Titlu carte:" };
	QLabel* lblAuthor = new QLabel{ "Autor carte:" };
	QLabel* lblGenre = new QLabel{ "Gen carte:" };
	QLabel* lblPublisher = new QLabel{ "Editura carte:" };
	QLabel* lblReleaseYear = new QLabel{ "An aparitie carte:" };

	QLineEdit* editId;
	QLineEdit* editTitle;
	QLineEdit* editAuthor;
	QLineEdit* editGenre;
	QLineEdit* editPublisher;
	QLineEdit* editReleaseYear;


	QLabel* lblISBN = new QLabel{ "ISBN carte cautata:" };
	QLabel* lblNewGenre = new QLabel{ "Gen nou carte:" };
	QLabel* lblNewPublisher = new QLabel{ "Editura noua carte:" };

	QLineEdit* editISBN;
	QLineEdit* editNewGenre;
	QLineEdit* editNewPublisher;

	QLabel* lblSearchedISBN = new QLabel{ "ISBN carte cautata:" };
	QLineEdit* editSearchedISBN;


	QGroupBox* groupBoxSortCriteria = new QGroupBox(tr("Criteriu sortare"));
	QGroupBox* groupBoxSortOrder = new QGroupBox(tr("Ordine sortare"));
	QGroupBox* groupBoxFilter = new QGroupBox(tr("Criteriu filtrare"));

	QRadioButton* radioSortByTitle = new QRadioButton(QString::fromStdString("Titlu"));
	QRadioButton* radioSortByAuthor = new QRadioButton(QString::fromStdString("Autor"));
	QRadioButton* radioSortAscending = new QRadioButton(QString::fromStdString("Crescator"));
	QRadioButton* radioSortDescending = new QRadioButton(QString::fromStdString("Descrescator"));
	QRadioButton* radioSortByReleaseYearAndGenre = new QRadioButton(QString::fromStdString("An aparitie si gen"));
	
	QPushButton* btnSortBooks;

	QLabel* lblFilterByReleaseYear = new QLabel{ "Filtrare carti aparute intre anii dati" };
	QLineEdit* minFilterYear;
	QLineEdit* maxFilterYear;
	QPushButton* btnFilterByReleaseYear;

	QLabel* lblFilterByGenre = new QLabel{ "Filtrare carti de un anumit gen:" };
	QLineEdit* editFilterGenre;
	QPushButton* btnFilterByGenre;

	WishlistGUI* wishlistWindow;
	WishlistDrawGUI* wishlistViewWindow;

	QPushButton* btnReloadData;
	QPushButton* btnUndoAction;
	QPushButton* btnWishlistMenu;
	QPushButton* btnClose;

	QTableView* tblViewBooklist;
	TableViewModel* modelBooklist;

	QTableWidget* tblBooklist;
	QListWidget* lstBooklist;
	QTableWidget* tblWishlist;

	QPushButton* btnGenerateReport;

	void update() override {
		//reloadTblBooklist(bookService.getAllBooks());
		//reloadLstBooklist(bookService.getAllBooks());
		reloadTblViewBooklist(bookService.getAllBooks());
		reloadWishlist(bookService.getWishlistBooks());
		reloadGenreReport();
	}

	void initializeGUI();

	void connectSignalsSlots();

	void reloadTblBooklist(vector<Book> booklist);

	void reloadLstBooklist(vector<Book> booklist);

	void reloadTblViewBooklist(vector<Book> booklist);

	void reloadWishlist(vector<Book> wishlist);

	void reloadGenreReport();


public:
	LibraryGUI(Library& srv) : bookService{ srv } {
		initializeGUI();
		connectSignalsSlots();
		//reloadTblBooklist(bookService.getAllBooks());
		//reloadLstBooklist(bookService.getAllBooks());
		reloadTblViewBooklist(bookService.getAllBooks());
		reloadWishlist(bookService.getWishlistBooks());
		reloadGenreReport();
	};

	void addBookGUI();
	void updateBookGUI();
	void findBookGUI();

	void filterByReleaseYearGUI();
	void filterByGenreGUI();

	void generateReportGUI();

	~LibraryGUI() {
		bookService.removeObserver(this);
	}
};
