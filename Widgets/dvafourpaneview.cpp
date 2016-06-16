#include "dvafourpaneview.h"
#include "ui_dvafourpaneview.h"

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageAlgorithm.h>
#include <vtkImageData.h>

#include <QDebug>

#include "skullwidget.h"

dvaFourPaneView::dvaFourPaneView(QWidget *parent, vtkImageAlgorithm  *inputalgo):
    dvaWidget(parent, inputalgo)
{
    this->widgetName = "View";
    this->inputAlgo = inputAlgo;

    this->ui = new Ui::dvaFourPaneView;
    this->ui->setupUi(this);

    for(int i=0; i<3; i++) this->resliceViewerArr[i] = vtkSmartPointer<vtkResliceImageViewer>::New();
    this->ui->view1->SetRenderWindow(resliceViewerArr[0]->GetRenderWindow());
    this->ui->view2->SetRenderWindow(resliceViewerArr[1]->GetRenderWindow());
    this->ui->view3->SetRenderWindow(resliceViewerArr[2]->GetRenderWindow());
    resliceViewerArr[0]->SetupInteractor(this->ui->view1->GetInteractor());
    resliceViewerArr[1]->SetupInteractor(this->ui->view2->GetInteractor());
    resliceViewerArr[2]->SetupInteractor(this->ui->view3->GetInteractor());

    for(int i=0; i<3; i++){
        this->resliceViewerArr[i]->SetInputData(this->inputAlgo->GetOutput());
        this->resliceViewerArr[i]->SetSliceOrientation(i);
        this->resliceViewerArr[i]->SetResliceModeToAxisAligned();
        this->resliceViewerArr[i]->Render();
    }

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    this->ui->view4->GetRenderWindow()->AddRenderer(renderer);

    /* SkullWidget *skullWidget = new SkullWidget(this, inputAlgo);
    this->ui->viewsLayout->removeWidget(this->ui->view4);
    this->ui->view4->hide();
    this->ui->viewsLayout->addWidget(skullWidget, 1, 1, 1, 1);
    this->ui->viewsLayout->update();
    */
}

dvaFourPaneView::~dvaFourPaneView(){
    delete this->ui;
}

void dvaFourPaneView::on_renderBtn_clicked()
{
    ;
}

void dvaFourPaneView::DataUpdated()
{
    int imageDims[3];
    this->inputAlgo->GetOutput()->GetDimensions(imageDims);
    for(int i=0; i<3; ++i) this->resliceViewerArr[i]->SetSlice(imageDims[i]/2);
    for(int i=0; i<3; ++i) this->resliceViewerArr[i]->GetRenderer()->ResetCamera();
    double range[2];
    this->inputAlgo->GetOutput()->GetScalarRange(range);
    for(int i=0; i<3; ++i) this->resliceViewerArr[i]->SetColorWindow(range[1]-range[0]);
    for(int i=0; i<3; ++i) this->resliceViewerArr[i]->SetColorLevel((range[0]+range[1])/2.0);
    for(int i=0; i<3; ++i) this->resliceViewerArr[i]->Render();
}

