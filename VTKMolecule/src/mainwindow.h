#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkMoleculeMapper.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButtonQuit_clicked();
private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkMoleculeMapper> molmapper;

};

#endif // MAINWINDOW_H
