#include <QtWidgets/QApplication>

#include "MainWindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    // NOTE: Запускаем цикл обработки событий приложения.
    return app.exec();
}
