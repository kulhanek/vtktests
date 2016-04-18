#include <QFileDialog>
#include <QGLFormat>
#include <QtOpenGL>
#include <QVTKWidget2.h>
#include <QVTKWidget.h>
//VTK
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include <vtkVersion.h>
#include <vtkPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkCellArray.h>
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkNew.h"
#include <vtkCellArray.h>
#include <vtkTubeFilter.h>
#include <vtkLineSource.h>
#include <vtkFloatArray.h>
//-------test----------
#include <vtkPiecewiseFunction.h>
#include <vtkAbstractElectronicData.h>
#include "vtkSmartVolumeMapper.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkImageData.h"
#include "vtkImageShiftScale.h"
#include "vtkFixedPointVolumeRayCastMapper.h"
#include <vtkContourGrid.h>
#include <vtkOutlineFilter.h>
#include <vtkProbeFilter.h>
#include <vtkImplicitDataSet.h>
#include <vtkCutter.h>
#include <vtkUnstructuredGrid.h>

//-------test----------


#include <vtkGaussianCubeReader.h>
#include <vtkMoleculeToAtomBallFilter.h>
#include <vtkAtom.h>
#include <vtkBond.h>
#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>
#include <vtkContourFilter.h>
#include <vtkEdgePoints.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataNormals.h>
#include <vtkImageShiftScale.h>
#include <vtkVolume.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkStructuredGrid.h>
#include <vtkProbeFilter.h>
#include <vtkPointData.h>
//OB
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/griddata.h>
#include <openbabel/generic.h>
#include <openbabel/grid.h>

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




void MainWindow::on_pushButton_clicked(){
////---------------- IMPLICITNE ISOVRSTVY----------------------------
//    QString Qfilename = QFileDialog::getOpenFileName(this, tr("Open File"));
//    MoleculReader *p_mr = new MoleculReader();

//    const char* fileName = Qfilename.toStdString().c_str();

//    p_mr->readToMol(fileName);

//    vtkSmartPointer<vtkMolecule> vtkmol = vtkSmartPointer<vtkMolecule>::New();
//    vtkmol->Initialize();

//    for( unsigned int i = 1; i < (p_mr->getMol().NumAtoms())+1; i++){
//        double x = p_mr->getMol().GetAtom(i)->GetX();
//        double y = p_mr->getMol().GetAtom(i)->GetY();
//        double z = p_mr->getMol().GetAtom(i)->GetZ();
//        unsigned int A =  p_mr->getMol().GetAtom(i)->GetAtomicNum();
//        vtkmol->AppendAtom(A,x,y,z);
//    }


//    vtkSmartPointer<vtkMoleculeToAtomBallFilter> molfilter = vtkSmartPointer<vtkMoleculeToAtomBallFilter>::New();
//        molfilter->SetInputData(vtkmol);
//        molfilter->SetRadiusSource(1);
//        molfilter->SetRadiusScale(1.4);
//        molfilter->SetResolution(30);

//       ZISKAT IZOVRSTVU Z vdW MODELU





//-----------------NACITANIE GAUSSIAN CUBE POMOCOU VTK ------------------------

        // Reads density cube file
        vtkNew<vtkGaussianCubeReader> denReader;
        denReader->SetFileName("/home/piader1/bakalarka/respfit/benzoat/volume/density.cube");
        denReader->SetHBScale(1.1);
        denReader->SetBScale(10);
        denReader->Update();
        // Reads potential cube file
        vtkNew<vtkGaussianCubeReader> potReader;
        potReader->SetFileName("/home/piader1/bakalarka/respfit/benzoat/volume/potential.cube");
        potReader->SetHBScale(1.1);
        potReader->SetBScale(10);
        potReader->Update();

//----------------------------------------------------------------------
        //Outline Box
        vtkNew<vtkOutlineFilter> bounds;
        bounds->SetInputData(denReader->GetGridOutput());

        vtkNew<vtkPolyDataMapper> boundsMapper;
        boundsMapper->SetInputConnection(bounds->GetOutputPort());

        vtkNew<vtkActor> boundsActor;
        boundsActor->SetMapper(boundsMapper.GetPointer());
        boundsActor->GetProperty()->SetColor(0.5, 0.6, 0.7);



//        double range[2];

//        denReader->GetGridOutput()->GetScalarRange(range);
//        cout << "ImageData range: " << range[0] <<" "<< range[1] << "\n";
//        vtkNew<vtkUnstructuredGrid> potgrid;
//        potgrid->SetInputConnection(potReader->GetOutputPort());
//        potgrid = potReader->GetGridOutput();

        vtkNew<vtkImageShiftScale> t;
        t->SetInputData(denReader->GetGridOutput());

        vtkNew<vtkImageShiftScale> pot;
        pot->SetInputData(potReader->GetGridOutput());


//         Contour filter
        vtkNew<vtkContourFilter> contour;
        contour->SetInputConnection(t->GetOutputPort());
        contour->SetNumberOfContours(1);
        contour->SetValue(0, .002);



//        vtkNew<vtkContourFilter> potcontour;
//        potcontour->SetInputConnection(pot->GetOutputPort());

        //Probe filer
        vtkNew<vtkProbeFilter> probe;
        probe->SetInputConnection(contour->GetOutputPort());
        probe->SetSourceConnection(pot->GetOutputPort());
        probe->Update();
        probe->GetOutput()->GetPointData()->SetNormals(contour->GetOutput()->GetPointData()->GetNormals());
//        probe->SpatialMatchOn();
        double rangeESP[2];
        probe->GetOutput()->GetScalarRange(rangeESP);

        double range = rangeESP[1]-rangeESP[0];
        double MinESP = rangeESP[0];
        double MaxESP = rangeESP[1];


        vtkNew<vtkColorTransferFunction> ctfESP;
//        ctfESP->AddRGBPoint(-MaxESP,      1.0, 0.7, 0.7);
        ctfESP->AddRGBPoint(/*0.5**/MinESP,  1.0, 0.0, 0.0);
         ctfESP->AddRGBPoint((MinESP+MaxESP)/2, 0.0, 0.1, 0.0);
//        ctfESP->AddRGBPoint( 1e-4*range, 0.0, 0.6, 0.4);
        ctfESP->AddRGBPoint( MaxESP,  0.0, 0.0, 1.0);
//        ctfESP->AddRGBPoint( MaxESP,      0.7, 0.7, 1.0);
        ctfESP->Build();


        vtkNew<vtkPolyDataMapper> conmapper;
        conmapper->SetInputConnection(probe->GetOutputPort());
        conmapper->SetLookupTable(ctfESP.GetPointer());
        conmapper->ScalarVisibilityOn();
        conmapper->SetScalarRange(pot->GetOutput()->GetScalarRange());


        vtkNew<vtkActor> conactor;
        conactor->SetMapper(conmapper.GetPointer());
        conactor->GetProperty()->SetSpecular(0);
        conactor->GetProperty()->SetDiffuse(1);
//        conactor->SetScale(0.12);



//-----------------DISPLAY SCENE-----------------------------------

    vtkNew<vtkRenderer> ren;
        ren->AddActor(conactor.GetPointer());
        ren->AddActor(boundsActor.GetPointer());
    vtkNew<vtkRenderWindow> renwin;
        renwin->AddRenderer(ren.GetPointer());

    vtkNew<vtkRenderWindowInteractor> iren;
        iren->SetRenderWindow(renwin.GetPointer());


    ren->SetBackground(1.0, 1.0, 1.0);
    iren->Start();
    renwin->Render();

}

void MainWindow::on_pushButtonQuit_clicked(){
    close();
}
