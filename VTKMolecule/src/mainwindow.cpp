// =============================================================================
// NEMESIS - Molecular Modelling Package
// -----------------------------------------------------------------------------
//    Copyright (C) 2016 Matej Stevuliak, 423943@mail.muni.cz
//    Copyright (C) 2016 Petr Kulhanek, kulhanek@chemi.muni.cz
//
//     This program sin free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
//
//     This program sin distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License along
//     with this program; if not, write to the Free Software Foundation, Inc.,
//     51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// =============================================================================
#include <QFileDialog>
//VTK
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkLight.h>


#include <vtkAtom.h>
#include <vtkBond.h>
#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>
#include <vtkMoleculeToAtomBallFilter.h>

//OB
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/atom.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moleculreader.h"


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

// Subclass for setting own inretactor style ==============================

class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);


    virtual void OnKeyPress()
    {
      // Get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();

      // keypress model setting
      if(key == "a") {
        Molmapper->UseVDWSpheresSettings();
      }
      if(key == "s") {
        Molmapper->UseLiquoriceStickSettings();
      }
      if(key == "d"){
        Molmapper->UseBallAndStickSettings();
      }
      // Forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
    vtkMoleculeMapper* Molmapper;
};

vtkStandardNewMacro(KeyPressInteractorStyle);

//==========================================================================



void MainWindow::on_pushButton_clicked()
{
    // gain filename from file dialog
    QString Qfilename = QFileDialog::getOpenFileName(this, tr("Open File"));
    const char* fileName = Qfilename.toLatin1().constData();

    // read selected file to MoleculReader data object
    MoleculReader *p_mr = new MoleculReader();
    p_mr->readToMol(fileName);

    // creating and generating vtkMolecule object wich contains informations about molecule
    vtkMolecule *vtkmol = vtkMolecule::New();
      vtkmol->Initialize();

      double x, y, z;
      unsigned int bOr;
      unsigned int h1, h2;
      unsigned int A;

      // this creates atoms
      for( unsigned int i = 1; i < (p_mr->getMol().NumAtoms())+1; i++) {
          x = p_mr->getMol().GetAtom(i)->GetX();
          y = p_mr->getMol().GetAtom(i)->GetY();
          z = p_mr->getMol().GetAtom(i)->GetZ();
          A = p_mr->getMol().GetAtom(i)->GetAtomicNum();

         vtkmol->AppendAtom(A,x,y,z);
    }

      // this creates bonds
      for( unsigned int i = 0; i < p_mr->getMol().NumBonds(); i++) {
         bOr = p_mr->getMol().GetBond(i)->GetBondOrder();
         h1 = p_mr->getMol().GetBond(i)->GetBeginAtom()->GetIndex();
         h2 = p_mr->getMol().GetBond(i)->GetEndAtom()->GetIndex();

        vtkmol->AppendBond(h1,h2,bOr);
    }

    // mapper that draws molecule
    molmapper = vtkMoleculeMapper::New();
    molmapper->SetInputData(vtkmol);

    vtkSmartPointer<KeyPressInteractorStyle> style = vtkSmartPointer<KeyPressInteractorStyle>::New();
    style->Molmapper = molmapper;

    // this alterantes avalible models
    int Id = ui->comboBox->currentIndex();
    if(Id == 0){
        molmapper->UseVDWSpheresSettings();
    } else if (Id == 1){
        molmapper->UseLiquoriceStickSettings();
    } else if (Id == 2){
        molmapper->UseBallAndStickSettings();
    }

    // actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(molmapper);
    // renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddActor(actor);


//------------------DISPLAY SCENE--------------------------------------
    // render window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        renderWindow->SetSize(600,600);

    // inrteractor
    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iren->SetRenderWindow(renderWindow);        
        iren->SetInteractorStyle(style);
        style->SetCurrentRenderer(renderer);

        renderer->SetBackground(1.0,1.0,1.0);
        renderer->SetOcclusionRatio(0.2);
        renderer->ResetCamera();

        renderWindow->SetMultiSamples(0);
        renderWindow->GetInteractor()->Initialize();
        renderWindow->Render();
        iren->Start();
     }

void MainWindow::on_pushButtonQuit_clicked(){
    close();
}

