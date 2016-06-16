#include "dvaprogrammablewidget.h"
#include "ui_dvaprogrammablewidget.h"

#include "dvawidget.h"

#include <vtkImageAlgorithm.h>

dvaProgrammableWidget::dvaProgrammableWidget(QWidget *parent, vtkImageAlgorithm *inputalgo) :
    dvaWidget(parent, inputalgo),
    ui(new Ui::dvaProgrammableWidget)
{
    ui->setupUi(this);
}

dvaProgrammableWidget::~dvaProgrammableWidget()
{
    delete ui;
}

void dvaProgrammableWidget::DataUpdated()
{

}
