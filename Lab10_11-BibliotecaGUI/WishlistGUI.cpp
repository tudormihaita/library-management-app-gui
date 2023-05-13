#include "WishlistGUI.h"
#pragma warning (disable: 26409) // avoid calling new and delete explicitly
#pragma warning (disable: 4267) // conversion from size_t to int


void WishlistGUI::intializeGUI() {
	//setup main layout : left side - lista carti wishlist, right side - comenzi wishlist
	QHBoxLayout* wishlistMainLayout = new QHBoxLayout;
	this->setLayout(wishlistMainLayout);

	//setup groupbox view lista wishlist
	QVBoxLayout* wishlistViewLayout = new QVBoxLayout;
	wishlistViewBox->setLayout(wishlistViewLayout);

	this->lstWishlist = new QListWidget;
	this->lstWishlist->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->lstWishlist->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);

	wishlistViewLayout->addWidget(this->lstWishlist);

	//setup groupbox comenzi wishlist
	QVBoxLayout* wishlistCommandsLayout = new QVBoxLayout;
	wishlistCommandsBox->setLayout(wishlistCommandsLayout);

	QFormLayout* wishlistOperationsLayout = new QFormLayout;
	wishlistOperationsBox->setLayout(wishlistOperationsLayout);

	editTitleWishlist = new QLineEdit;

	wishlistOperationsLayout->addRow(lblAddToWishlist, editTitleWishlist);

	btnMatchWishlistBooks = new QPushButton("Cauta carti");
	btnAddToWishlist = new QPushButton("Adauga carte in wishlist");
	QWidget* wishlistAddBtns = new QWidget;
	QHBoxLayout* wishlistAddBtnsLayout = new QHBoxLayout;
	wishlistAddBtns->setLayout(wishlistAddBtnsLayout);

	wishlistAddBtnsLayout->addWidget(btnMatchWishlistBooks);
	wishlistAddBtnsLayout->addWidget(btnAddToWishlist);

	wishlistOperationsLayout->addWidget(wishlistAddBtns);

	//setup slider - work in progress

	/*QVBoxLayout* sliderLayout = new QVBoxLayout;
	sliderWishlistBox->setLayout(sliderLayout);

	sliderWishlist = new QSlider(Qt::Horizontal);
	sliderWishlist->setMinimum(1);
	sliderWishlist->setMaximum(bookService.getAllBooks().size());
	sliderWishlist->hasTracking();

	QWidget* labelBox = new QWidget;
	QHBoxLayout* labelsLayout = new QHBoxLayout;
	labelBox->setLayout(labelsLayout);
	minimumLabel = new QLabel(QString::fromStdString("Numarul minim de carti de generat: " + to_string(sliderWishlist->minimum())));
	maximumLabel = new QLabel(QString::fromStdString("Numarul maxim de carti de generat: " + to_string(sliderWishlist->maximum())));
	currentLabel = new QLabel(QString::fromStdString("Numarul de carti de generat: " + to_string(sliderWishlist->value())));
	
	labelsLayout->addWidget(minimumLabel);
	labelsLayout->addWidget(currentLabel);
	labelsLayout->addWidget(maximumLabel);

	sliderLayout->addWidget(labelBox);
	sliderLayout->addWidget(sliderWishlist);

	wishlistOperationsLayout->addWidget(sliderWishlistBox);*/

	//setup spinbox generare random wishlist
	generateRandomSpinbox = new QSpinBox;
	generateRandomSpinbox->setRange(1, bookService.getAllBooks().size());
	generateRandomSpinbox->setValue(1);

	btnAddRandomToWishlist = new QPushButton("Adauga carti random");
	wishlistOperationsLayout->addRow(lblAddRandomToWishlist, generateRandomSpinbox);
	wishlistOperationsLayout->addWidget(btnAddRandomToWishlist);


	editExportFileName = new QLineEdit;
	wishlistOperationsLayout->addRow(lblExportWishlist, editExportFileName);

	btnExportWishlist = new QPushButton("Exporta wishlist");
	wishlistOperationsLayout->addWidget(btnExportWishlist);

	QHBoxLayout* wishlistUtilitiesLayout = new QHBoxLayout;
	wishlistUtilitiesBox->setLayout(wishlistUtilitiesLayout);

	btnReloadWishlist = new QPushButton("Reload wishlist");
	btnClearWishlist = new QPushButton("Sterge wishlist");
	btnCloseWishlist = new QPushButton("Inchide");

	wishlistUtilitiesLayout->addWidget(btnReloadWishlist);
	wishlistUtilitiesLayout->addWidget(btnClearWishlist);
	wishlistUtilitiesLayout->addWidget(btnCloseWishlist);

	wishlistCommandsLayout->addWidget(wishlistOperationsBox);
	wishlistCommandsLayout->addWidget(wishlistUtilitiesBox);

	wishlistMainLayout->addWidget(wishlistViewBox);
	wishlistMainLayout->addWidget(wishlistCommandsBox);

	//setup main window initial look
	QIcon iconWishlist("wishlist-icon.png");
	this->setWindowIcon(iconWishlist);
	this->setWindowTitle("Wishlist");
}

void WishlistGUI::reloadWishlist(vector<Book> wishlist) {
	this->lstWishlist->clear();

	int lineNumber = 0;
	for (auto& book : wishlist) {
		this->lstWishlist->insertItem(lineNumber, QString::fromStdString(book.getISBN() + " | " + book.getTitle() + " | " + book.getAuthor()
			+ " | " + book.getGenre() + " | " + book.getPublisher() + " " + to_string(book.getYear())));

		lineNumber++;
	}

	this->lstWishlist->setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);

}

void WishlistGUI::connectSignalsSlots() {
	bookService.addObserver(this);

	//adaugare
	QObject::connect(btnMatchWishlistBooks, &QPushButton::clicked, this, &WishlistGUI::matchWishlistBooksGUI);

	QObject::connect(btnAddToWishlist, &QPushButton::clicked, this, &WishlistGUI::addWishlistGUI);

	QObject::connect(btnExportWishlist, &QPushButton::clicked, this, [&]() {
		string fileName = editExportFileName->text().toStdString();
		try {
			this->bookService.exportWishlist(fileName);

			QMessageBox::information(this, "Info", QString::fromStdString("Wishlist exportat cu succes in fisierul " + fileName));
		}
		catch (RepoException& re) {
			QMessageBox::warning(this, "Warning", QString::fromStdString(re.get_error_message()));
		}
	});

	QObject::connect(btnAddRandomToWishlist, &QPushButton::clicked, this, [&]() {
		const int howMany = generateRandomSpinbox->value();
		this->bookService.addRandomToWishlist(howMany);

		QMessageBox::information(this, "Info", QString::fromStdString("Wishlist random generat cu succes!"));
	});

	//refresh
	QObject::connect(btnReloadWishlist, &QPushButton::clicked, this, [&]() {
		this->reloadWishlist(bookService.getWishlistBooks());
		});

	//clear
	QObject::connect(btnClearWishlist, &QPushButton::clicked, this, [&]() {
		this->bookService.emptyWishlist();

		QMessageBox::information(this, "Info", QString::fromStdString("Wishlist golit cu succes!"));
		});

	//close
	QObject::connect(btnCloseWishlist, &QPushButton::clicked, this, [&]() {
		this->close();
		});
}

void WishlistGUI::matchWishlistBooksGUI() {
	string searchedTitle = this->editTitleWishlist->text().toStdString();

	editTitleWishlist->clear();
	//qDebug() << searchedTitle;

	auto matchedBooks = this->bookService.matchWishlistBooks(searchedTitle);
	if (matchedBooks.size() > 0) {
		this->lstWishlist->clear();

		int lineNumber = 0;
		for (auto& book : matchedBooks) {
			this->lstWishlist->insertItem(lineNumber, QString::fromStdString(book.getISBN() + " | " + book.getTitle() + " | " + book.getAuthor()
				+ " | " + book.getGenre() + " | " + book.getPublisher() + " | " + to_string(book.getYear())));


			QListWidgetItem* currentBook = this->lstWishlist->item(lineNumber);
			currentBook->setFlags(currentBook->flags() | Qt::ItemIsUserCheckable);
			currentBook->setCheckState(Qt::Unchecked);
			currentBook->setData(Qt::UserRole, QString::fromStdString(book.getISBN()));

			lineNumber++;
		}


		QMessageBox::information(this, "Info", QString::fromStdString("Au fost afisate cartile gasite in biblioteca cu titlul dat,"
			"selectati cartile pe care doriti sa le adaugati la wishlist."));
	}
	else {
		QMessageBox::warning(this, "Warning", QString::fromStdString("Nu au fost gasite carti cu titlul dat!"));
	}
}

void WishlistGUI::addWishlistGUI() {
	bool checked = false;

	QListWidget* matchedList = new QListWidget;
	for (int i = 0; i < lstWishlist->count(); i++) {
		auto book = lstWishlist->item(i)->clone();
		matchedList->insertItem(i, book);
	}

	for (int i = 0; i < matchedList->count(); i++) {
		auto book = matchedList->item(i);
		if (book->checkState() == Qt::Checked) {
			try {
				bookService.addToWishlist(book->data(Qt::UserRole).toString().toStdString());
				checked = true;
			}
			catch (RepoException& re) {
				QMessageBox::warning(this, "Warning", QString::fromStdString(re.get_error_message()));
			}
		}
	}

	if (checked == true) {
		QMessageBox::information(this, "Info", QString::fromStdString("Cartile selectate au fost adaugate la wishlist cu succes!"));
	}
	else {
		QMessageBox::warning(this, "Warning", QString::fromStdString("Nicio carte nu a fost selectata, nu se poate efectua adaugarea!"));
	}
}