#include "LibraryGUI.h"
#pragma warning (disable: 26409) // avoid calling new and delete explicitly
#pragma warning (disable: 4267) // conversion from size_t to int

void LibraryGUI::initializeGUI() {
	//setup main layout, right side - comenzi , left side - tabel booklist si wishlist ( + panel butoane dinamice )
	QHBoxLayout* mainLayout = new QHBoxLayout;
	this->setLayout(mainLayout);

	QWidget* leftSide = new QWidget;
	QVBoxLayout* leftLayout = new QVBoxLayout;
	leftSide->setLayout(leftLayout);

	//setup groupbox comenzi
	QLayout* commandsLayout = new QVBoxLayout;
	commandsBox->setLayout(commandsLayout);

	//setup groupbox operatii CRUD
	btnAdd = new QPushButton("&Adauga");
	btnUpdate = new QPushButton("&Modifica");
	btnDelete = new QPushButton("&Sterge");
	btnFind = new QPushButton("&Cauta");

	QLayout* operationsLayout = new QHBoxLayout;
	operationsBox->setLayout(operationsLayout);
	operationsLayout->addWidget(btnAdd);
	operationsLayout->addWidget(btnUpdate);
	operationsLayout->addWidget(btnDelete);
	operationsLayout->addWidget(btnFind);

	commandsLayout->addWidget(operationsBox);

	//setup groupbox sortari
	QLayout* sortLayout = new QVBoxLayout;
	sortBox->setLayout(sortLayout);
	QWidget* formSort = new QWidget;
	QHBoxLayout* formSortLayout = new QHBoxLayout;
	formSort->setLayout(formSortLayout);

	QVBoxLayout* radioBoxSortLayout = new QVBoxLayout;
	this->groupBoxSortCriteria->setLayout(radioBoxSortLayout);
	radioBoxSortLayout->addWidget(radioSortByAuthor);
	radioBoxSortLayout->addWidget(radioSortByTitle);
	radioBoxSortLayout->addWidget(radioSortByReleaseYearAndGenre);

	formSortLayout->addWidget(groupBoxSortCriteria);

	QVBoxLayout* radioBoxSortOrderLayout = new QVBoxLayout;
	this->groupBoxSortOrder->setLayout(radioBoxSortOrderLayout);
	radioBoxSortOrderLayout->addWidget(radioSortAscending);
	radioBoxSortOrderLayout->addWidget(radioSortDescending);

	formSortLayout->addWidget(groupBoxSortOrder);
	sortLayout->addWidget(formSort);

	btnSortBooks = new QPushButton("Sorteaza carti");
	sortLayout->addWidget(btnSortBooks);

	commandsLayout->addWidget(sortBox);

	//setup groupbox filtrari
	QLayout* filterLayout = new QVBoxLayout;
	filterBox->setLayout(filterLayout);
	QWidget* formFilterYear = new QWidget;
	QFormLayout* formFilterYearLayout = new QFormLayout;
	formFilterYear->setLayout(formFilterYearLayout);

	minFilterYear = new QLineEdit;
	maxFilterYear = new QLineEdit;

	formFilterYearLayout->addRow(lblFilterByReleaseYear);
	formFilterYearLayout->addRow(minFilterYear, maxFilterYear);

	btnFilterByReleaseYear = new QPushButton("Filtreaza carti dupa anul aparitiei");

	filterLayout->addWidget(formFilterYear);
	filterLayout->addWidget(btnFilterByReleaseYear);

	QWidget* formFilterGenre = new QWidget;
	QFormLayout* formFilterGenreLayout = new QFormLayout;
	formFilterGenre->setLayout(formFilterGenreLayout);

	editFilterGenre = new QLineEdit;
	formFilterGenreLayout->addRow(lblFilterByGenre, editFilterGenre);
	btnFilterByGenre = new QPushButton("Filtreaza carti dupa gen");

	filterLayout->addWidget(formFilterGenre);
	filterLayout->addWidget(btnFilterByGenre);

	commandsLayout->addWidget(filterBox);

	//setup groupbox utilitati: undo, reload list, wishlist window
	QHBoxLayout* utilitiesLayout = new QHBoxLayout;
	utilitiesBox->setLayout(utilitiesLayout);

	btnClose = new QPushButton("Inchide");
	utilitiesLayout->addWidget(btnClose);

	btnReloadData = new QPushButton("Reload booklist");
	utilitiesLayout->addWidget(btnReloadData);

	btnUndoAction = new QPushButton("Undo");
	utilitiesLayout->addWidget(btnUndoAction);

	btnWishlistMenu = new QPushButton("Wishlist");
	utilitiesLayout->addWidget(btnWishlistMenu);

	commandsLayout->addWidget(utilitiesBox);

	leftLayout->addWidget(commandsBox);
	mainLayout->addWidget(leftSide);

	//setup right side booklist
	QWidget* rightSide = new QWidget;
	QVBoxLayout* rightLayout = new QVBoxLayout;
	rightSide->setLayout(rightLayout);

	QVBoxLayout* viewLayout = new QVBoxLayout;
	viewBox->setLayout(viewLayout);

	//const int nrLines = 20;
	//const int nrColumns = 6;
	//this->tableBooklist = new QTableWidget{ nrLines, nrColumns };

	//setup groupbox vizualizare tabele carti si wishlist
	QVBoxLayout* booklistLayout = new QVBoxLayout;
	booklistBox->setLayout(booklistLayout);

	this->tblBooklist = new QTableWidget;
	this->tblBooklist->setColumnCount(6);

	QStringList tblHeaderBookList;
	tblHeaderBookList << "ISBN" << "Titlu" << "Autor" << "Gen" << "Editura" << "An aparitie";
	this->tblBooklist->setHorizontalHeaderLabels(tblHeaderBookList);
	this->tblBooklist->setSelectionBehavior(QAbstractItemView::SelectRows);

	this->tblBooklist->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->tblBooklist->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	booklistLayout->addWidget(this->tblBooklist);

	QVBoxLayout* wishlistLayout = new QVBoxLayout;
	wishlistBox->setLayout(wishlistLayout);

	this->tblWishlist = new QTableWidget;
	this->tblWishlist->setColumnCount(6);
	this->tblWishlist->setSelectionBehavior(QAbstractItemView::SelectRows);

	this->tblWishlist->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->tblWishlist->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	wishlistLayout->addWidget(this->tblWishlist);

	btnGenerateReport = new QPushButton("Genereaza statistica genuri carti");

	//adaugare toate componente in main layout
	viewLayout->addWidget(booklistBox);
	viewLayout->addWidget(wishlistBox);
	viewLayout->addWidget(btnGenerateReport);
	rightLayout->addWidget(viewBox);
	mainLayout->addWidget(leftSide);
	mainLayout->addWidget(rightSide);

	//setup main window initial look
	QIcon iconMain("book-icon.png");
	this->setWindowIcon(iconMain);
	this->setWindowTitle("Library");
}


void LibraryGUI::reloadBooklist(vector<Book> booklist) {
	this->tblBooklist->clearContents();
	this->tblBooklist->setRowCount(booklist.size());

	int lineNumber = 0;
	for (auto& book : booklist) {
		this->tblBooklist->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(book.getISBN())));
		this->tblBooklist->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(book.getTitle())));
		this->tblBooklist->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
		this->tblBooklist->setItem(lineNumber, 3, new QTableWidgetItem(QString::fromStdString(book.getGenre())));
		this->tblBooklist->setItem(lineNumber, 4, new QTableWidgetItem(QString::fromStdString(book.getPublisher())));
		this->tblBooklist->setItem(lineNumber, 5, new QTableWidgetItem(QString::number(book.getYear())));

		lineNumber++;
	}
}

void LibraryGUI::reloadWishlist(vector<Book> wishlist) {
	this->tblWishlist->clearContents();
	this->tblWishlist->setRowCount(wishlist.size());

	int lineNumber = 0;
	for (auto& book : wishlist) {
		this->tblWishlist->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(book.getISBN())));
		this->tblWishlist->setItem(lineNumber, 1, new QTableWidgetItem(QString::fromStdString(book.getTitle())));
		this->tblWishlist->setItem(lineNumber, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
		this->tblWishlist->setItem(lineNumber, 3, new QTableWidgetItem(QString::fromStdString(book.getGenre())));
		this->tblWishlist->setItem(lineNumber, 4, new QTableWidgetItem(QString::fromStdString(book.getPublisher())));
		this->tblWishlist->setItem(lineNumber, 5, new QTableWidgetItem(QString::number(book.getYear())));

		lineNumber++;
	}
}

void LibraryGUI::reloadGenreReport() {
	QVBoxLayout* reportLayout = new QVBoxLayout;

	qDeleteAll(reportBox->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
	delete reportBox->layout();

	reportBox->setLayout(reportLayout);

	for (auto& stat : bookService.getBookReport()) {
		QPushButton* btnGenre = new QPushButton(QString::fromStdString(stat.first));
		QObject::connect(btnGenre, &QPushButton::clicked, this, [=]() {
			QMessageBox::information(this, "Info", QString::fromStdString("Numarul cartilor din genul " + stat.first + ": " + to_string(stat.second.getCount())));
		});

		reportLayout->addWidget(btnGenre);
	}

	reportLayout->addStretch();
	this->layout()->addWidget(reportBox);

}


void LibraryGUI::connectSignalsSlots() {
	bookService.addObserver(this);

	//adaugare
	QObject::connect(btnAdd, &QPushButton::clicked, this, &LibraryGUI::addBookGUI);

	//modificare
	QObject::connect(btnUpdate, &QPushButton::clicked, this, &LibraryGUI::updateBookGUI);

	//cautare
	QObject::connect(btnFind, &QPushButton::clicked, this, &LibraryGUI::findBookGUI);

	//afisare report
	QObject::connect(btnGenerateReport, &QPushButton::clicked, this, &LibraryGUI::generateReportGUI);

	//stergere
	QObject::connect(btnDelete, &QPushButton::clicked, this, [this]() {
		auto selectedBooks = tblBooklist->selectedItems();

		QList<QTableWidgetItem*> booksToRemove;
		foreach(QTableWidgetItem* book, selectedBooks) {
			booksToRemove.append(book->clone());
		}

		if (!selectedBooks.isEmpty()) {
			for (int i = 0; i <booksToRemove.size(); i+=tblBooklist->columnCount()) {
				string ISBN = booksToRemove.at(i)->data(0).toString().toStdString();
				//qDebug() << ISBN;
				this->bookService.removeBook(ISBN);
			}

			//this->reloadBooklist(this->bookService.getAllBooks());
			//this->reloadGenreReport();

			QMessageBox::information(this, "Info", QString::fromStdString("Cartile selectate au fost sterse cu succes!"));
		}
		else {
			QMessageBox::warning(this, "Warning", QString::fromStdString("Selectati cartile pe care doriti sa le stergeti!"));
		}
	});

	//sortare
	QObject::connect(btnSortBooks, &QPushButton::clicked, this, [&]() {
		if (this->radioSortByTitle->isChecked() && this->radioSortAscending->isChecked())
			this->reloadBooklist(bookService.sortByTitle(false));
		else if (this->radioSortByTitle->isChecked() && this->radioSortDescending->isChecked())
			this->reloadBooklist(bookService.sortByTitle(true));
		else if (this->radioSortByAuthor->isChecked() && this->radioSortAscending->isChecked())
			this->reloadBooklist(bookService.sortByAuthor(false));
		else if (this->radioSortByAuthor->isChecked() && this->radioSortDescending->isChecked())
			this->reloadBooklist(bookService.sortByAuthor(true));
		else if (this->radioSortByReleaseYearAndGenre->isChecked() && this->radioSortAscending->isChecked())
			this->reloadBooklist(bookService.sortByReleaseYearAndGenre(false));
		else if (this->radioSortByReleaseYearAndGenre->isChecked() && this->radioSortDescending->isChecked())
			this->reloadBooklist(bookService.sortByReleaseYearAndGenre(true));
		else {
			QMessageBox::warning(this, "Warning", QString::fromStdString("Selectati un criteriu de sortare si ordinea dorita!"));
		}

	});

	//filtrare
	QObject::connect(btnFilterByReleaseYear, &QPushButton::clicked, this, &LibraryGUI::filterByReleaseYearGUI);
	QObject::connect(btnFilterByGenre, &QPushButton::clicked, this, &LibraryGUI::filterByGenreGUI);

	//refresh
	QObject::connect(btnReloadData, &QPushButton::clicked, [&]() {
		this->reloadBooklist(bookService.getAllBooks());
	});

	//undo
	QObject::connect(btnUndoAction, &QPushButton::clicked, this, [this]() {
		try {
			this->bookService.undo();
			//this->reloadBooklist(bookService.getAllBooks());
			this->reloadGenreReport();

			QMessageBox::information(this, "Info", "Undo efectuat cu succes!");
		}
		catch (UndoException& ue) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(ue.get_error_message()));
		}
	});

	//wishlist menu
	QObject::connect(btnWishlistMenu, &QPushButton::clicked, this, [this]() {
		this->wishlistWindow = new WishlistGUI{ bookService };
		this->wishlistWindow->show();
	});

	//close
	QObject::connect(btnClose, &QPushButton::clicked, this, [this]() {
		this->close();
	});
}

void LibraryGUI::addBookGUI() {
	QWidget* addWindow = new QWidget;
	QFormLayout* addLayout = new QFormLayout;
	addWindow->setLayout(addLayout);
	addWindow->setWindowTitle("Adauga carte");

	editId = new QLineEdit;
	editTitle = new QLineEdit;
	editAuthor = new QLineEdit;
	editGenre = new QLineEdit;
	editPublisher = new QLineEdit;
	editReleaseYear = new QLineEdit;

	addLayout->addRow(lblId, editId);
	addLayout->addRow(lblTitle, editTitle);
	addLayout->addRow(lblAuthor, editAuthor);
	addLayout->addRow(lblGenre, editGenre);
	addLayout->addRow(lblPublisher, editPublisher);
	addLayout->addRow(lblReleaseYear, editReleaseYear);
	
	
	QPushButton* btnAddBook = new QPushButton("Adauga carte");
	QPushButton* btnAddClose = new QPushButton("Inchide");


	addLayout->addWidget(btnAddBook);
	addLayout->addWidget(btnAddClose);

	addWindow->show();

	QObject::connect(btnAddClose, &QPushButton::clicked, addWindow, [addWindow]() {
		addWindow->close();
	});

	QObject::connect(btnAddBook, &QPushButton::clicked, addWindow, [=]() {
		try {
			int id = stoi(editId->text().toStdString());
			string title = editTitle->text().toStdString();
			string author = editAuthor->text().toStdString();
			string genre = editGenre->text().toStdString();
			string publisher = editPublisher->text().toStdString();
			int releaseYear = editReleaseYear->text().toInt();

			editId->clear();
			editTitle->clear();
			editAuthor->clear();
			editGenre->clear();
			editPublisher->clear();
			editReleaseYear->clear();

			this->bookService.storeBook(id, title, author, genre, publisher, releaseYear);
			//this->reloadBooklist(this->bookService.getAllBooks());
			//this->reloadGenreReport();

			QMessageBox::information(addWindow, "Info", QString::fromStdString("Carte adaugata cu succes!"));

			addWindow->close();

		}
		catch (std::invalid_argument&) {
			QMessageBox::warning(addWindow, "Warning", QString::fromStdString("Input invalid pentru numar intreg!"));
		}
		catch (RepoException& re) {
			QMessageBox::warning(addWindow, "Warning", QString::fromStdString(re.get_error_message()));
		}
		catch (ValidationException& ve) {
			QMessageBox::warning(addWindow, "Warning", QString::fromStdString(ve.get_error_messages()));
		}

		addWindow->close();
	});

}

void LibraryGUI::updateBookGUI() {
	QWidget* updateWindow = new QWidget;
	QFormLayout* updateLayout = new QFormLayout;
	updateWindow->setLayout(updateLayout);
	updateWindow->setWindowTitle("Modifica carte");

	editISBN = new QLineEdit;
	editNewGenre = new QLineEdit;
	editNewPublisher = new QLineEdit;

	updateLayout->addRow(lblISBN, editISBN);
	updateLayout->addRow(lblNewGenre, editNewGenre);
	updateLayout->addRow(lblNewPublisher, editNewPublisher);

	QPushButton* btnUpdateBook = new QPushButton("Modifica carte");
	QPushButton* btnUpdateClose = new QPushButton("Inchide");

	updateLayout->addWidget(btnUpdateBook);
	updateLayout->addWidget(btnUpdateClose);

	updateWindow->show();

	QObject::connect(btnUpdateClose, &QPushButton::clicked, updateWindow, [updateWindow]() {
		updateWindow->close();
	});

	QObject::connect(btnUpdateBook, &QPushButton::clicked, updateWindow, [=]() {
		try {
			string ISBN = editISBN->text().toStdString();
			string newGenre = editNewGenre->text().toStdString();
			string newPublisher = editNewPublisher->text().toStdString();

			editISBN->clear();
			editNewGenre->clear();
			editNewPublisher->clear();

			this->bookService.updateBook(ISBN, newGenre, newPublisher);
			//this->reloadBooklist(this->bookService.getAllBooks());
			//this->reloadGenreReport();

			QMessageBox::information(updateWindow, "Info", QString::fromStdString("Carte modificata cu succes!"));


			updateWindow->close();
		}
		catch (RepoException& re) {
			QMessageBox::warning(updateWindow, "Warning", QString::fromStdString(re.get_error_message()));
		}
		catch (ValidationException& ve) {
			QMessageBox::warning(updateWindow, "Warning", QString::fromStdString(ve.get_error_messages()));
		}

		updateWindow->close();
	});

}

void LibraryGUI::findBookGUI() {
	QWidget* findWindow = new QWidget;
	QFormLayout* findLayout = new QFormLayout;
	findWindow->setLayout(findLayout);
	findWindow->setWindowTitle("Cauta carte");

	editSearchedISBN = new QLineEdit;
	findLayout->addRow(lblSearchedISBN, editSearchedISBN);

	QPushButton* btnFindBook = new QPushButton("Cauta carte");
	QPushButton* btnFindClose = new QPushButton("Inchide");

	findLayout->addWidget(btnFindBook);
	findLayout->addWidget(btnFindClose);

	findWindow->show();

	QObject::connect(btnFindClose, &QPushButton::clicked, findWindow, [findWindow]() {
		findWindow->close();
	});

	QObject::connect(btnFindBook, &QPushButton::clicked, findWindow, [=]() {
		try {
			string ISBN = editSearchedISBN->text().toStdString();

			auto foundBook = this->bookService.findBook(ISBN);

			auto success = QString::fromStdString("Carte gasita: " + foundBook.getISBN() + " | " + foundBook.getTitle() + " | " +
				foundBook.getAuthor() + " | " + foundBook.getGenre() + " | " + foundBook.getPublisher() + " | " + to_string(foundBook.getYear()));

			QMessageBox::information(findWindow, "Info", success);
		}
		catch (RepoException& re) {
			QMessageBox::warning(findWindow, "Warning", QString::fromStdString(re.get_error_message()));
		}
		findWindow->close();
	});

}

void LibraryGUI::filterByReleaseYearGUI() {
	if (minFilterYear->text().isEmpty() || maxFilterYear->text().isEmpty())
		QMessageBox::warning(this, "Warning", QString::fromStdString("Introduceti valori valide pentru realizarea filtrarii!"));
	else {
		int minYear = this->minFilterYear->text().toInt();
		int maxYear = this->maxFilterYear->text().toInt();

		minFilterYear->clear();
		maxFilterYear->clear();

		this->reloadBooklist(bookService.filterByReleaseYear(minYear, maxYear));
		if (bookService.filterByReleaseYear(minYear, maxYear).size() == 0) {
			QMessageBox::information(this, "Info", QString::fromStdString("Au fost filtrate toate cartile!"));
		}
	}
}

void LibraryGUI::filterByGenreGUI() {
	if (editFilterGenre->text().isEmpty()) {
		QMessageBox::warning(this, "Warning", QString::fromStdString("Introduceti o valoare valida pentru realizarea filtrarii!"));
	}
	else {
		string genre = this->editFilterGenre->text().toStdString();

		editFilterGenre->clear();

		this->reloadBooklist(bookService.filterByGenre(genre));
		if (bookService.filterByGenre(genre).size() == 0) {
			QMessageBox::information(this, "Info", QString::fromStdString("Au fost filtrate toate cartile!"));
		}
	}
}

void LibraryGUI::generateReportGUI() {
	QWidget* reportWindow = new QWidget;
	QFormLayout* reportLayout = new QFormLayout;
	reportWindow->setLayout(reportLayout);
	reportWindow->setWindowTitle("Statistica genuri carti");

	QTableWidget* tblReport = new QTableWidget;
	tblReport->setColumnCount(2);

	QStringList tblHeaderReport;
	tblHeaderReport << "Gen" << "Numar exemplare";
	tblReport->setHorizontalHeaderLabels(tblHeaderReport);
	tblReport->setSelectionBehavior(QAbstractItemView::SelectRows);

	tblReport->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	tblReport->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	unordered_map<string, BookReportDTO> bookReport = this->bookService.getBookReport();

	if (bookReport.empty()) {
		QMessageBox::warning(reportWindow, "Warning", QString::fromStdString("Nu exista carti adaugate in lista, nu se poate genera o statistica!"));
		reportWindow->close();
		return;
	}

	tblReport->clearContents();
	tblReport->setRowCount(bookReport.size());

	int lineNumber = 0;
	for (auto& book : bookReport) {
		tblReport->setItem(lineNumber, 0, new QTableWidgetItem(QString::fromStdString(book.first)));
		tblReport->setItem(lineNumber, 1, new QTableWidgetItem(QString::number(book.second.getCount())));

		lineNumber++;
	}

	reportLayout->addWidget(tblReport);

	reportWindow->show();

}