#include <QApplication>
#include "MainWindow.h"

/*
 * Функция main - точка входа в приложение
 * Входные параметры:
 *   argc - количество аргументов командной строки
 *   argv - массив аргументов командной строки
 * Выходные данные:
 *   int - код возврата приложения (0 - успешное завершение)
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // MainWindow window;
    // window.show();
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}