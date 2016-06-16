#include "skullwidget.h"
#include "dvawidget.h"
#include "utils.h"

#include <iostream>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkRenderWindow.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRendererCollection.h>
#include <vtkCamera.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkInteractorStyleUser.h>
#include <vtkImageData.h>
#include <vtkVolumeCollection.h>

#include "ui_skullwidget.h"

class TimerCallback;
class SkullWidget;


class TimerCallback : public vtkCommand{
public:
    static TimerCallback *New(){
        TimerCallback *callback = new TimerCallback;
        callback->TimerCount = 0;
        return callback;
    }


    virtual void Execute(vtkObject *caller, unsigned long eventId, void *vtkNotUsed(callData)){
        if(vtkCommand::TimerEvent == eventId) this->TimerCount++;

        vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
        vtkCamera* camera = iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();

        camera->Azimuth(5);
        camera->SetViewUp(0, 1, 0);

        iren->GetRenderWindow()->Render();
    }


private:
    int TimerCount;

public:

};



SkullWidget::SkullWidget(QWidget *parent, vtkImageAlgorithm *inputalgo) : dvaWidget(parent)
{
    this->inputAlgo = inputalgo;
    //inputAlgo = vtkSmartPointer <vtkImageAlgorithm>::New();
    this -> ui = new Ui::SkullWidget();
    ui->setupUi(this);

    // skalar jest typu unsigned short (5)
    //cout << "Scalartype: " << reader->GetDataScalarType() << endl;

    // calculate center point
    int dataExtent[6];
    double dataSpacing[3];
    double centerPoint[3];
    inputAlgo->GetOutput()->GetExtent(dataExtent);
    inputAlgo->GetOutput()->GetSpacing(dataSpacing);
    for(int i=0; i<3; ++i) centerPoint[i] = (dataExtent[2*i+1] - dataExtent[2*i]) *dataSpacing[i]/2 ;


    // define volume properties
    vtkSmartPointer<vtkVolumeProperty> volProperty =
            vtkSmartPointer<vtkVolumeProperty>::New();
    volProperty->SetInterpolationTypeToNearest();
    volProperty->ShadeOn();
    volProperty->SetAmbient(0.4);
    volProperty->SetDiffuse(0.6);
    volProperty->SetSpecular(0.2);

    vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
    volumeGradientOpacity->AddPoint(0, 0.0);
    volumeGradientOpacity->AddPoint(128,  0.5);
    volumeGradientOpacity->AddPoint(255, 1.0);
    volProperty->SetGradientOpacity(volumeGradientOpacity);

    vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
//    volumeScalarOpacity->AddPoint(0, 0.0);
//    volumeScalarOpacity->AddPoint(25000, 1.0, 0.1, 1);
    volumeScalarOpacity->SetClamping(1);
    volumeScalarOpacity->AddPoint(0, 0.0);
    volumeScalarOpacity->AddPoint(511, 0.05);
    volProperty->SetScalarOpacity(volumeScalarOpacity);

    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volMapper =
            vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volMapper->SetInputConnection(inputAlgo->GetOutputPort());
    volMapper->SetBlendModeToAdditive();
    //volMapper->SetBlendModeToComposite();
    //volMapper->SetBlendModeToMaximumIntensity();
    //volMapper->SetBlendModeToMinimumIntensity();

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volMapper);
    volume->SetProperty(volProperty);
    volume->SetVisibility(1);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddVolume(volume);

    vtkRenderWindow *renWin = this->ui->vtkWidget->GetRenderWindow();
    renWin->AddRenderer(renderer);

    // setup camera
    vtkCamera *camera = renderer->GetActiveCamera();
    camera->SetParallelProjection(1);
    camera->SetFocalPoint(centerPoint);
    camera->Zoom(0.005);
    camera->SetViewUp(0, 1, 0);
    camera->Elevation(20);
    camera->OrthogonalizeViewUp();


    // setup axes
    vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
    vtkSmartPointer<vtkTransform> axesTransform = vtkSmartPointer<vtkTransform>::New();
    axesTransform->Translate(centerPoint);
    axesActor->SetUserTransform(axesTransform);
    axesActor->SetTotalLength(200, 200, 200);
    renderer->AddActor(axesActor);


    // attach timer callback to interactor
    vtkRenderWindowInteractor *iren = this->ui->vtkWidget->GetInteractor();
    vtkSmartPointer<TimerCallback> timerCallback = vtkSmartPointer<TimerCallback>::New();
    iren->CreateRepeatingTimer(20);
    iren->AddObserver(vtkCommand::TimerEvent, timerCallback);


    // setup interactor
    vtkSmartPointer<vtkInteractorStyleUser> interactorStyle = vtkSmartPointer<vtkInteractorStyleUser>::New();
    iren->SetInteractorStyle(interactorStyle);

    // render will be called at paintevent of QVTKWidget

    //Utils::histo(reader.GetPointer());

}


SkullWidget::~SkullWidget(){
    std::cout << "skullWidget destructor" << std::endl;
    delete ui;
}

void SkullWidget::InitializeWidget(){
    // zrobic na foreach
    vtkSmartPointer <vtkRendererCollection> renderers = this->ui->vtkWidget->GetRenderWindow()->GetRenderers();
    vtkSmartPointer <vtkRenderer> renderer = renderers->GetFirstRenderer();
    vtkSmartPointer <vtkVolumeCollection> volumes = renderer->GetVolumes();
    vtkSmartPointer <vtkVolume> volume = volumes->GetNextVolume();
    volume->SetVisibility(1);

    this->widgetInitialized = true;
}

void SkullWidget::DataUpdated()
{
    if (!this->IsInitialized()){
        this->InitializeWidget();
    }
}
