#include "mainwindow.h"
#include <QApplication>

int __stdcall  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
    QApplication a(__argc, __argv);
    MainWindow w;
    w.show();

    return a.exec();
}
