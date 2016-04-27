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

#include <vtkSmartPointer.h>
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include <vtkPolyData.h>
#include "vtkCommand.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkNew.h"
#include <vtkPiecewiseFunction.h>
#include "vtkImageData.h"
#include "vtkImageShiftScale.h"
#include <vtkOutlineFilter.h>
#include <vtkProbeFilter.h>
#include <vtkGaussianCubeReader.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkColorTransferFunction.h>
#include <vtkProbeFilter.h>
#include <vtkPointData.h>
#include <vtkObjectFactory.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

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
        Contour->SetValue(0, .02);
      }
      if(key == "s") {
        Contour->SetValue(0, .002);
      }
      if(key == "d"){
        Contour->SetValue(0, .0002);
      }
      // Forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
    vtkContourFilter* Contour;
};

vtkStandardNewMacro(KeyPressInteractorStyle);

//===========================================================================


void MainWindow::on_pushButton_clicked() {
    // open file dialogs for density and potencial files
    QString Qfilenameden = QFileDialog::getOpenFileName(this, tr("Open Density File"));
    QString Qfilenamepot = QFileDialog::getOpenFileName(this, tr("Open Potential File"));

    // reads density cube file
    vtkNew<vtkGaussianCubeReader> denReader;
        denReader->SetFileName(Qfilenameden.toLatin1().constData());
        denReader->Update();

    // reads potential cube file
    vtkNew<vtkGaussianCubeReader> potReader;
        potReader->SetFileName(Qfilenamepot.toLatin1().constData());
        potReader->Update();

//     outline Box
    vtkNew<vtkOutlineFilter> bounds;
        bounds->SetInputData(denReader->GetGridOutput());

    vtkNew<vtkPolyDataMapper> boundsMapper;
        boundsMapper->SetInputConnection(bounds->GetOutputPort());

    vtkNew<vtkActor> boundsActor;
        boundsActor->SetMapper(boundsMapper.GetPointer());
        boundsActor->GetProperty()->SetColor( 0, 0, 0 );

    // shift scale
    vtkNew<vtkImageShiftScale> den;
        den->SetInputData(denReader->GetGridOutput());

    vtkNew<vtkImageShiftScale> pot;
        pot->SetInputData(potReader->GetGridOutput());


    // contour filter creates isosurface
    contour = vtkContourFilter::New();
        
        contour->SetInputConnection(den->GetOutputPort());
        contour->SetNumberOfContours(1);
        contour->SetValue(0, .002);
   
    // Contour fom style to contour
    vtkSmartPointer<KeyPressInteractorStyle> style = vtkSmartPointer<KeyPressInteractorStyle>::New();
        style->Contour = contour;

    // Probe filer
    vtkNew<vtkProbeFilter> probe;
        probe->SetInputConnection(contour->GetOutputPort());
        probe->SetSourceConnection(pot->GetOutputPort());
        probe->Update();
        probe->GetOutput()->GetPointData()->SetNormals(contour->GetOutput()->GetPointData()->GetNormals());

    // get scale from isosurface
    double rangeESP[2];
    probe->GetOutput()->GetScalarRange(rangeESP);

    double MinESP = rangeESP[0];
    double MaxESP = rangeESP[1];

    // create color scale
    vtkNew<vtkColorTransferFunction> ctfESP;
        ctfESP->AddRGBPoint( MinESP,  1.0, 0.0, 0.0 );
        ctfESP->AddRGBPoint( MaxESP,  0.0, 0.0, 1.0 );
        ctfESP->Build();

    // mapper
    vtkNew<vtkPolyDataMapper> conmapper;
        conmapper->SetInputConnection(probe->GetOutputPort());
        conmapper->SetLookupTable(ctfESP.GetPointer());
        conmapper->ScalarVisibilityOn();
        conmapper->SetScalarRange(pot->GetOutput()->GetScalarRange());

    // actor
    vtkNew<vtkActor> conactor;
        conactor->SetMapper(conmapper.GetPointer());
        conactor->GetProperty()->SetSpecular(0);
        conactor->GetProperty()->SetDiffuse(1);

//DISPLAY SCENE------------------------------
    // render
    vtkNew<vtkRenderer> ren;
        ren->AddActor(conactor.GetPointer());
        ren->AddActor(boundsActor.GetPointer());

    // render window
    vtkNew<vtkRenderWindow> renwin;
        renwin->AddRenderer(ren.GetPointer());

    // interctor
    vtkNew<vtkRenderWindowInteractor> iren;
        iren->SetRenderWindow(renwin.GetPointer());
        iren->SetInteractorStyle(style);

    ren->SetBackground( 1.0, 1.0, 1.0 );
    iren->Start();
    renwin->Render();

}

void MainWindow::on_pushButtonQuit_clicked(){
    close();
}
