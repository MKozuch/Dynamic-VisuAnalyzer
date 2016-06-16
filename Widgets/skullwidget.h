#ifndef SKULLWIDGET_H
#define SKULLWIDGET_H

#include <QWidget>
#include <vtkImageAlgorithm.h>
#include "dvawidget.h"

namespace Ui {
class SkullWidget;
}

class SkullWidget : public dvaWidget
{
    Q_OBJECT

public:
    explicit SkullWidget(QWidget *parent = 0, vtkImageAlgorithm *inputalgo=NULL);
    ~SkullWidget();

    void InitializeWidget();

private:
    Ui::SkullWidget *ui;

public slots:
    void DataUpdated();
};

#endif // SKULLWIDGET_H
