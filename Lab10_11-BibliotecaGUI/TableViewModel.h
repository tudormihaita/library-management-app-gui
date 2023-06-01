#pragma once
#include "Book.h"
#include <vector>
#include <QAbstractTableModel>
#include <QFont>
#include <QBrush>

using std::vector;

class TableViewModel : public QAbstractTableModel {
private:
	vector<Book> books;

public:
	TableViewModel(const vector<Book> books) : books{ books } {};

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void setBooks(const vector<Book> booklist);

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	Qt::ItemFlags flags(const QModelIndex&) const {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}
};