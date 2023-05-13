#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab10_11BibliotecaGUI.h"

class Lab10_11BibliotecaGUI : public QMainWindow
{
    Q_OBJECT

public:
    Lab10_11BibliotecaGUI(QWidget *parent = nullptr);
    ~Lab10_11BibliotecaGUI();

private:
    Ui::Lab10_11BibliotecaGUIClass ui;
};
