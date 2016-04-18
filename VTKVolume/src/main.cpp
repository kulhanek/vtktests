#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include "mainwindow.h"


int main(int argc,char* argv[])
{
    QApplication qapp(argc,argv);



   MainWindow* window = new MainWindow();


   window->show();

   qapp.exec();


    return (0);
}


