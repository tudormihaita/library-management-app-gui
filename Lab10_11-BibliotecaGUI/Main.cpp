#include "Lab10_11BibliotecaGUI.h"
#include "Tests.h"
#include "LibraryGUI.h"
#include <iostream>
#include <QtWidgets/QApplication>

void run() {
    Tests tests;
    tests.runAllTests();
}


int main(int argc, char *argv[])
{
    //Lab10_11BibliotecaGUI w;
    // w.show();

    run();

    QApplication app(argc, argv);
    FileBookRepository bookRepository{ "books.txt" };
    Validator bookValidator;
    Library bookLibrary{ bookRepository, bookValidator };
    LibraryGUI guiLibrary{ bookLibrary };

    //guiLibrary.resize(1200, 600);

    app.setStyle("fusion");

    guiLibrary.show();
    return app.exec();
}
