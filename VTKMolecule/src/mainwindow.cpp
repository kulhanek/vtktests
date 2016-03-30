#include <QFileDialog>
//VTK
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkNew.h"
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

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

//test

#include "vtkLight.h"


#include "vtkNew.h"

#include "vtkSequencePass.h"
#include "vtkShadowMapBakerPass.h"
#include "vtkShadowMapPass.h"
#include "vtkOpenGLRenderer.h"
#include "vtkCameraPass.h"
#include "vtkRenderPassCollection.h"


#include "vtkLookupTable.h"
#include "vtkPeriodicTable.h"

#include "vtkTimerLog.h"
#include "vtkCamera.h"
//Test

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

void MainWindow::on_comboBox_currentIndexChanged(int index){
    if(!molmapper)return;
    if(index==0){
        molmapper->UseVDWSpheresSettings();
    }else if(index==1){
        molmapper->UseLiquoriceStickSettings();
    }else if(index==2){
        molmapper->UseBallAndStickSettings();
    }
}

void MainWindow::on_pushButton_clicked()
{
    //Gaining filename from file dialog.
    QString Qfilename = QFileDialog::getOpenFileName(this, tr("Open File"));
    const char* fileName = Qfilename.toStdString().c_str();

    //Reading selected file to MoleculReader data object.
    MoleculReader *p_mr = new MoleculReader();
    p_mr->readToMol(fileName);

    //Creating and generating vtkMolecule object wich contains informations about molecule.
    vtkMolecule *vtkmol = vtkMolecule::New();
      vtkmol->Initialize();
    //This creates atoms.
      for( unsigned int i = 1; i < (p_mr->getMol().NumAtoms())+1; i++){
         double x = p_mr->getMol().GetAtom(i)->GetX();
         double y = p_mr->getMol().GetAtom(i)->GetY();
         double z = p_mr->getMol().GetAtom(i)->GetZ();
         unsigned int A =  p_mr->getMol().GetAtom(i)->GetAtomicNum();
         vtkmol->AppendAtom(A,x,y,z);
    }
    //This creates Bonds.
      for( unsigned int i = 0; i < p_mr->getMol().NumBonds(); i++){
        unsigned int bOr = p_mr->getMol().GetBond(i)->GetBondOrder();
        unsigned int h1 = p_mr->getMol().GetBond(i)->GetBeginAtom()->GetIndex();
        unsigned int h2 = p_mr->getMol().GetBond(i)->GetEndAtom()->GetIndex();
        vtkmol->AppendBond(h1,h2,bOr);
    }

        molmapper = vtkSmartPointer<vtkMoleculeMapper>::New();
        molmapper->SetInputData(vtkmol);

    //This alterantes avalible models.
    int Id = ui->comboBox->currentIndex();
    if(Id==0){
        molmapper->UseVDWSpheresSettings();
    }else if(Id==1){
        molmapper->UseLiquoriceStickSettings();
    }else if(Id==2){
        molmapper->UseBallAndStickSettings();
    }




    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(molmapper);
        actor->GetProperty()->SetColor(0.5,0.6,0.2);
//        // we override the default shader very slightly so that
//        // the ambient color component is scaled off the diffuse
//        molmapper->GetFastAtomMapper()->AddShaderReplacement(
//          vtkShader::Fragment,  // in the fragment shader
//          "//VTK::Color::Impl",
//          true, // before the standard replacements
//          "//VTK::Color::Impl\n" // we still want the default
//          "  ambientColor = diffuseColor*0.2;\n", //but we add this
//          false // only do it once
//          );actor->GetProperty()->SetDiffuse(0.7);




    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
       renderer->AddActor(actor);


//------------------DISPLAY SCENE--------------------------------------
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer(renderer);
        renderWindow->SetSize(600,600);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iren->SetRenderWindow(renderWindow);





//        renderer->SetBackground(0.0,0.0,0.0);
        renderer->SetBackground2(0.2, 0.2, 0.3);
        renderer->SetBackground(0.1, 0.1, 0.15);
        renderer->SetOcclusionRatio(0.5);
        renderer->SetAmbient(0.7,0.7,0.7);
        renderer->GradientBackgroundOn();
        renderer->ResetCamera();

        renderWindow->SetMultiSamples(0); //
        renderWindow->GetInteractor()->Initialize();
        iren->Start();
        renderWindow->Render();

     }



void MainWindow::on_pushButtonQuit_clicked(){
    close();
}

