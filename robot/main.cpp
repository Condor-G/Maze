#include "mainwindow.h"
#include "display.h"
#include <QApplication>
#include <QTextCodec>


using namespace std;

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    MainWindow w;

    w.show();


    return a.exec();
}
