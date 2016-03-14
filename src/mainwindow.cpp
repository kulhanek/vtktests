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
#include <vtkCubeSource.h>
#include <vtkPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph3D.h>
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

#include <vtkGaussianCubeReader.h>
#include <vtkDotProductSimilarity.h>
#include <vtkMoleculeToAtomBallFilter.h>
#include <vtkAtom.h>
#include <vtkBond.h>
#include <vtkMolecule.h>
#include <vtkMoleculeMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkClipClosedSurface.h>
#include <vtkPieceScalars.h>
#include <vtkContourFilter.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkEdgePoints.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataNormals.h>
#include <vtkImageShiftScale.h>
#include <vtkVolume.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkStructuredGrid.h>

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



void MainWindow::on_pushButton_clicked()
{

    QString Qfilename = QFileDialog::getOpenFileName(this, tr("Open File"));
    MoleculReader *p_mr = new MoleculReader();

    const char* fileName = Qfilename.toStdString().c_str();

    p_mr->readToMol(fileName);



//--------------NEW CPK USING vtkMolecule--------------------------
    vtkMolecule *vtkmol = vtkMolecule::New();
      vtkmol->Initialize();
    for( unsigned int i = 0; i < p_mr->getMol().NumBonds(); i++){

        double xb =  p_mr->getMol().GetBond(i)->GetBeginAtom()->GetX();
        double xe =  p_mr->getMol().GetBond(i)->GetEndAtom()->GetX();
        double yb =  p_mr->getMol().GetBond(i)->GetBeginAtom()->GetY();
        double ye =  p_mr->getMol().GetBond(i)->GetEndAtom()->GetY();
        double zb =  p_mr->getMol().GetBond(i)->GetBeginAtom()->GetZ();
        double ze =  p_mr->getMol().GetBond(i)->GetEndAtom()->GetZ();

        unsigned int Ab = p_mr->getMol().GetBond(i)->GetBeginAtom()->GetAtomicNum();
        unsigned int Ae = p_mr->getMol().GetBond(i)->GetEndAtom()->GetAtomicNum();
        unsigned int bOr = p_mr->getMol().GetBond(i)->GetBondOrder();

        vtkAtom h1 = vtkmol->AppendAtom(Ab,xb,yb,zb);   //problem je ze priraduje kazdy atom 2x
        vtkAtom h2 = vtkmol->AppendAtom(Ae,xe,ye,ze);
        vtkmol->AppendBond(h1,h2,bOr);
    }

    vtkSmartPointer<vtkMoleculeMapper> molmapper = vtkSmartPointer<vtkMoleculeMapper>::New();
        molmapper->SetInputData(vtkmol);

//       SWICH BETWEN FILTERS
//        molmapper->UseVDWSpheresSettings();
//        molmapper->UseLiquoriceStickSettings();
        molmapper->UseBallAndStickSettings();

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(molmapper);
        actor->GetProperty()->SetColor(0.5,0.6,0.2);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
       renderer->AddActor(actor);


//------------------DISPLAY SCENE--------------------------------------
//       vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//       renderWindow->AddRenderer(renderer);


//        vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//        iren->SetRenderWindow(renderWindow);

//        renderer->SetBackground(.0, .0, .0);
//        iren->Start();
//        renderWindow->Render();
//        //when using QVTKWidget2

        vtkSmartPointer<vtkGenericOpenGLRenderWindow> glrw = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
//        glrw->AddRenderer(renderer);

//        vtkGenericOpenGLRenderWindow* glrw = vtkGenericOpenGLRenderWindow::SafeDownCast(renderWindow);
//        vtkGenericOpenGLRenderWindow* glrw = vtkGenericOpenGLRenderWindow::New();
        glrw->AddRenderer(renderer);
        glrw->SupportsOpenGL();


        QVTKWidget2* vtkwidget = new QVTKWidget2(QGLFormat(QGL::Rgba | QGL::DepthBuffer| QGL::DoubleBuffer));
        vtkwidget->SetRenderWindow(glrw);
        vtkwidget->GetRenderWindow()->SetSwapBuffers(0);
        vtkwidget->setAutoBufferSwap(true);
        vtkwidget->setParent(ui->widget);
        vtkwidget->show();

////        QVTKWidget2* vtkwidget = new QVTKWidget2(QGLFormat(QGL::Rgba | QGL::DepthBuffer| QGL::DoubleBuffer));


        //ui->widget->SetRenderWindow(glrw);

     }

void MainWindow::on_pushButtonvdW_clicked(){
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
//    vtkSmartPointer<vtkClipClosedSurface> surface = vtkSmartPointer<vtkClipClosedSurface>::New();
//        surface->SetInputConnection(molfilter->GetOutputPort());
//        surface->GenerateOutlineOn();

//    vtkSmartPointer<vtkFlyingEdges3D> flyedges = vtkSmartPointer<vtkFlyingEdges3D>::New();
//        flyedges->SetInputConnection(molfilter->GetOutputPort());
//        flyedges->ComputeNormalsOn();
//        flyedges->ComputeGradientsOn();
//        flyedges->ComputeScalarsOn();
//    vtkSmartPointer<vtkPieceScalars> piece = vtkSmartPointer<vtkPieceScalars>::New();
//        piece->SetInputConnection(molfilter->GetOutputPort());
//        piece->SetScalarModeToCellData();

//    vtkSmartPointer<vtkDataSetSurfaceFilter> surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//        surface->SetPieceInvariant(1);
//        surface->SetInputConnection(piece->GetOutputPort());

//    vtkSmartPointer<vtkCompositeSurfaceLICMapper> compmapper = vtkSmartPointer<vtkCompositeSurfaceLICMapper>::New();
//----------------------------------------------------------------------



//-----------------NACITANIE GAUSSIAN CUBE POMOCOU VTK ------------------------
    vtkGaussianCubeReader* gRead = vtkGaussianCubeReader::New();
        gRead->SetFileName("/home/piader1/bakalarka/respfit/benzoat/volume/density.cube");
        gRead->SetHBScale(1.1);
        gRead->SetBScale(10);
        gRead->Update();
//       int static range[2] = gRead.GetGridOutput().GetPointData().GetScalars().GetRange();

//        vtkImageShiftScale* readerSS = vtkImageShiftScale::New();
//        readerSS->SetInputData(gRead->GetGridOutput());
//        readerSS->SetShift(min * -1);
//        readerSS->SetScale(255 / (max - min));
//        readerSS->SetOutputScalarTypeToUnsignedChar();


//        vtkStructuredGrid* sgrid = vtkStructuredGrid::New();
//        sgrid->ShallowCopy(gRead->GetGridOutput());
//        vtkMarchingCubes* march = vtkMarchingCubes::New();
//        march->SetInputConnection(sgrid->GetOutputPort());
//        march->SetValue(0,0.5);


//        vtkPolyDataNormals* norm =  vtkPolyDataNormals::New();
//        norm->SetInputConnection(march->GetOutputPort());
//        norm->SetFeatureAngle(60.0);




    vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
        contour->SetInputConnection(gRead->GetOutputPort());
        contour->SetValue(0, 0.05);
//    vtkSmartPointer<vtkPolygonalHandleRepresentation3D> handle = vtkSmartPointer<vtkPolygonalHandleRepresentation3D>::New();
//        handle->SetHandle(molfilter->GetOutputPort());

//    vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointplacer = vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
//        pointplacer->AddProp(actor);
//        handle->SetPointPlacer(pointplacer);



//    vtkEdgePoints* points = vtkEdgePoints::New();
//        points->SetInputConnection(molfilter->GetOutputPort());


    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(contour->GetOutputPort());
        mapper->ScalarVisibilityOff();
//        mapper->SetScalarRange(0, .1);
//        mapper->GetLookupTable().SetHueRange(0.32, 0);
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(0.5,0.6,0.2);
//        actor->GetProperty()->SetRepresentationToSurface();
        actor->GetProperty()->SetOpacity(0.7);

//     vtkColorTransferFunction* ctf = vtkColorTransferFunction::New();
//        ctf->AddHSVPoint(0.0, 0.66, 1.0, 1.0);
//     vtkVolumeProperty* volprop = vtkVolumeProperty::New();
//        volprop->SetColor(ctf);
//        volprop->SetInterpolationTypeToLinear();
//    vtkFixedPointVolumeRayCastMapper* volmapper = vtkFixedPointVolumeRayCastMapper::New();
//        volmapper->SetInputConnection(gRead->GetOutputPort());
//    vtkVolume* vol = vtkVolume::New();
//        vol->SetMapper(volmapper);
//        vol->SetProperty(volprop);

//-----------------DISPLAY SCENE-----------------------------------
    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
        ren->AddActor(actor);
//        ren->AddActor(vol);
    vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
        renwin->AddRenderer(ren);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        iren->SetRenderWindow(renwin);


    ren->SetBackground(1.0, 1.0, 1.0);
    iren->Start();
    renwin->Render();

}

