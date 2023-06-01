#pragma once
#include "TableViewModel.h"

int TableViewModel::rowCount(const QModelIndex&) const {
	return static_cast<int>(books.size());
}

int TableViewModel::columnCount(const QModelIndex&) const {
	return 6;
}

QVariant TableViewModel::data(const QModelIndex& parent, int role) const {
	const int row = parent.row();
	const int column = parent.column();

	if (role == Qt::DisplayRole) {
		auto book = this->books.at(row);
		if (column == 0) {
			return QString::fromStdString(book.getISBN());
		}
		else if (column == 1) {
			return QString::fromStdString(book.getTitle());
		}
		else if (column == 2) {
			return QString::fromStdString(book.getAuthor());
		}
		else if (column == 3) {
			return QString::fromStdString(book.getGenre());
		}
		else if (column == 4) {
			return QString::fromStdString(book.getPublisher());
		}
		else if (column == 5) {
			return QString::number(book.getYear());
		}
	}
	else if (role == Qt::FontRole) {
		QFont font;
		if (column == 0) {
			font.setBold(true);
		}
		else if (column == 1) {
			font.setItalic(true);
			font.setBold(true);
			font.setUnderline(true);
		}
		else if (column == 2) {
			font.setBold(true);
		}
		else if (column == 4) {
			font.setItalic(true);
		}
		else if (column == 5) {
			font.setBold(true);
		}

		return font;
	}
	else if (role == Qt::BackgroundRole) {
		if (row % 2 == 0) {
			return QBrush{ Qt::black };
		}
		else {
			return QBrush{ Qt::magenta };
		}
	}
	else if (role == Qt::ForegroundRole) {
		if (row % 2 == 0) {
			return QBrush{ Qt::white };
		}
		else {
			return QBrush{ Qt::black };
		}
	}

	return QVariant{};
}

QVariant TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
			case 0:
				return tr("ISBN");
			case 1:
				return tr("Titlu");
			case 2:
				return tr("Autor");
			case 3:
				return tr("Gen");
			case 4:
				return tr("Editura");
			case 5:
				return tr("An aparitie");
			default:
				return QString("");
			}
		}
	}

	return QVariant();

}

void TableViewModel::setBooks(const vector<Book> booklist) {
	this->books = booklist;
	auto topLeft = createIndex(0, 0);
	auto bottomRight = createIndex(rowCount(), columnCount());
	emit dataChanged(topLeft, bottomRight);
}