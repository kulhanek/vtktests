#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moleculreader.h"
#include<QFileDialog>
//VTK
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include <QVTKWidget.h>
#include <vtkVersion.h>
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph3D.h>
#include <vtkCellArray.h>

#include <vtkCellArray.h>
#include <vtkTubeFilter.h>
#include <vtkLineSource.h>

//OB
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/atom.h>

using namespace std;
using namespace OpenBabel;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{

    QString Qfilename = QFileDialog::getOpenFileName(this, tr("Open File"));
    MoleculReader *p_mr = new MoleculReader();

    const char* fileName = Qfilename.toStdString().c_str();

    p_mr->readToMol(fileName);


    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();


    //cout<< p_mr->getMol().NumAtoms()<<endl;

    for( unsigned int i = 1; i < (p_mr->getMol().NumAtoms())+1; i++){
            double x = p_mr->getMol().GetAtom(i)->GetX();
            double y = p_mr->getMol().GetAtom(i)->GetY();
            double z = p_mr->getMol().GetAtom(i)->GetZ();

            vtkSmartPointer<vtkSphereSource> sphereSource =
                vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetCenter(x,y,z);
            sphereSource->SetThetaResolution(30);

            vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
              mapper->SetInputConnection(sphereSource->GetOutputPort());

              vtkSmartPointer<vtkActor> actor =
                vtkSmartPointer<vtkActor>::New();
              actor->SetMapper(mapper);


            unsigned int A =  p_mr->getMol().GetAtom(i)->GetAtomicNum();

//            switch (A){
//                case (1): //H
//                    sphereSource->SetRadius(0.5);
//                    actor->GetProperty()->SetColor(1.0, .0, 1.0);
//                casebay.come (6): //C
//                    sphereSource->SetRadius(2.5);
//                    actor->GetProperty()->SetColor(0.4, 0.4, 0.4);
//                case (8): //O
//                    sphereSource->SetRadius(1.5);
//                    actor->GetProperty()->SetColor(1.0, .0, .0);
//                case (7): //N
//                    sphereSource->SetRadius(1.0);
//                    actor->GetProperty()->SetColor(.0, .0, 1.0);
//                default: //Else
//                    sphereSource->SetRadius(0.5);
//                    actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
//               }
            if(A == 1){
              sphereSource->SetRadius(0.3);
              actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
            } else if (A == 6) {
              sphereSource->SetRadius(.5);
              actor->GetProperty()->SetColor(0.4, 0.4, 0.4);
            } else if (A == 8) {
               sphereSource->SetRadius(.4);
               actor->GetProperty()->SetColor(1.0, .0, .0);
            } else if (A == 7) {
               sphereSource->SetRadius(.4);
               actor->GetProperty()->SetColor(1.0, .0, .0);
            } else {
               sphereSource->SetRadius(0.2);
               actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
            }
        renderer->AddActor(actor);
        }

        for( unsigned int i = 0; i < p_mr->getMol().NumBonds(); i++){

            double xb =  p_mr->getMol().GetBond(i)->GetBeginAtom()->GetX();
            double xe =  p_mr->getMol().GetBond(i)->GetEndAtom()->GetX();
            double yb =  p_mr->getMol().GetBond(i)->GetBeginAtom()->GetY();
            double ye =  p_mr->getMol().GetBond(i)->GetEndAtom()->GetY();
            double zb =  p_mr->getMol().GetBond(i)->GetBeginAtom()->GetZ();
            double ze =  p_mr->getMol().GetBond(i)->GetEndAtom()->GetZ();


            double xm = (xb+xe)/2;
            double ym = (yb+ye)/2;
            double zm = (zb+ze)/2;
            // Create a line

            vtkSmartPointer<vtkLineSource> lineSource1 =
                vtkSmartPointer<vtkLineSource>::New();
            lineSource1->SetPoint1(xb, yb, zb);
            lineSource1->SetPoint2(xm, ym, zm);

            vtkSmartPointer<vtkLineSource> lineSource2 =
                vtkSmartPointer<vtkLineSource>::New();
            lineSource2->SetPoint1(xm, ym, zm);
            lineSource2->SetPoint2(xe, ye, ze);

            vtkSmartPointer<vtkPolyDataMapper> lineMapper1 =
                vtkSmartPointer<vtkPolyDataMapper>::New();
            lineMapper1->SetInputConnection(lineSource1->GetOutputPort());

            vtkSmartPointer<vtkActor> lineActor1 =
                vtkSmartPointer<vtkActor>::New();
            lineActor1->GetProperty()->SetColor(0.0,0.0,1.0);
            lineActor1->SetMapper(lineMapper1);

            vtkSmartPointer<vtkPolyDataMapper> lineMapper2 =
                vtkSmartPointer<vtkPolyDataMapper>::New();
            lineMapper2->SetInputConnection(lineSource2->GetOutputPort());

            vtkSmartPointer<vtkActor> lineActor2 =
                vtkSmartPointer<vtkActor>::New();
            lineActor2->GetProperty()->SetColor(0.0,0.0,1.0);
            lineActor2->SetMapper(lineMapper2);

            //create a tubes aroun the lines

             vtkSmartPointer<vtkTubeFilter> tubeFilter1 =
               vtkSmartPointer<vtkTubeFilter>::New();
             tubeFilter1->SetInputConnection(lineSource1->GetOutputPort());
             tubeFilter1->SetRadius(.15); //default is .5
             tubeFilter1->SetNumberOfSides(10);
             tubeFilter1->Update();

             vtkSmartPointer<vtkTubeFilter> tubeFilter2 =
               vtkSmartPointer<vtkTubeFilter>::New();
             tubeFilter2->SetInputConnection(lineSource2->GetOutputPort());
             tubeFilter2->SetRadius(.15); //default is .5
             tubeFilter2->SetNumberOfSides(10);
             tubeFilter2->Update();
             //create a tube mappers
             vtkSmartPointer<vtkPolyDataMapper> tubeMapper1 =
                 vtkSmartPointer<vtkPolyDataMapper>::New();
             tubeMapper1->SetInputConnection(tubeFilter1->GetOutputPort());

             vtkSmartPointer<vtkPolyDataMapper> tubeMapper2 =
                     vtkSmartPointer<vtkPolyDataMapper>::New();
             tubeMapper2->SetInputConnection(tubeFilter2->GetOutputPort());

             // create a tube actors
             vtkSmartPointer<vtkActor> tubeActor1 =
               vtkSmartPointer<vtkActor>::New();
             //tubeActor1->GetProperty()->SetOpacity(0.5);
             tubeActor1->SetMapper(tubeMapper1);

             vtkSmartPointer<vtkActor> tubeActor2 =
               vtkSmartPointer<vtkActor>::New();
             //tubeActor2->GetProperty()->SetOpacity(0.5);
             tubeActor2->SetMapper(tubeMapper2);



           unsigned int Ab = p_mr->getMol().GetBond(i)->GetBeginAtom()->GetAtomicNum();
            if(Ab == 1){
              tubeActor1->GetProperty()->SetColor(1.0, 1.0, 1.0);
            } else if (Ab == 6) {
              tubeActor1->GetProperty()->SetColor(0.4, 0.4, 0.4);
            } else if (Ab == 8) {
                tubeActor1->GetProperty()->SetColor(1.0, .0, .0);
            } else if (Ab == 7) {
                tubeActor1->GetProperty()->SetColor(1.0, .0, .0);
            } else {
               tubeActor1->GetProperty()->SetColor(1.0, 1.0, 1.0);
            }


            unsigned int Ae = p_mr->getMol().GetBond(i)->GetEndAtom()->GetAtomicNum();
             if(Ae == 1){
               tubeActor2->GetProperty()->SetColor(1.0, 1.0, 1.0);
             } else if (Ae == 6) {
               tubeActor2->GetProperty()->SetColor(0.4, 0.4, 0.4);
             } else if (Ae == 8) {
                 tubeActor2->GetProperty()->SetColor(1.0, .0, .0);
             } else if (Ae == 7) {
                 tubeActor2->GetProperty()->SetColor(1.0, .0, .0);
             } else {
                tubeActor2->GetProperty()->SetColor(1.0, 1.0, 1.0);
             }

//            cout<<p_mr->getMol().GetBond(i)->GetBeginAtom()->GetAtomicNum()<<endl;
//            switch(p_mr->getMol().GetBond(i)->GetBeginAtom()->GetAtomicNum()){
//            case(1):
//                tubeActor1->GetProperty()->SetColor(1.0, 1.0, 1.0);
//                //tubeActor2->GetProperty()->SetColor(1.0, 1.0, 1.0);
//            case(6):
//                tubeActor1->GetProperty()->SetColor(0.4, 0.4, 0.4);
//                //tubeActor2->GetProperty()->SetColor(0.4, 0.4, 0.4);
//            case(8):
//                tubeActor1->GetProperty()->SetColor(1.0, .0, .0);
//                //tubeActor2->GetProperty()->SetColor(1.0, .0, .0);
//            case(7):
//                tubeActor1->GetProperty()->SetColor(1.0, .0, .0);
//                //tubeActor2->GetProperty()->SetColor(1.0, .0, .0);
//            default:
//                tubeActor1->GetProperty()->SetColor(1.0, 1.0, 1.0);
//                //tubeActor2->GetProperty()->SetColor(1.0, 1.0, 1.0);
//            }
            renderer->AddActor(lineActor1);
            renderer->AddActor(lineActor2);
            renderer->AddActor(tubeActor1);
            renderer->AddActor(tubeActor2);
        }


        vtkSmartPointer<vtkRenderWindow> renderWindow =
          vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        vtkSmartPointer<vtkRenderWindowInteractor> iren =
          vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iren->SetRenderWindow(renderWindow);

        renderer->SetBackground(.0, .0, .0);
        iren->Start();
        renderWindow->Render();

     }





          //ui->vtkw->SetRenderWindow(renderWindow);



