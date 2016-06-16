#ifndef DVAFOURPANEVIEW_H
#define DVAFOURPANEVIEW_H

#include "dvawidget.h"
#include <vtkSmartPointer.h>
#include <vtkResliceImageViewer.h>

namespace Ui{
class dvaFourPaneView;
}

class dvaFourPaneView : public dvaWidget
{
    Q_OBJECT

public:
    explicit dvaFourPaneView(QWidget *parent=0, vtkImageAlgorithm* inputalgo=NULL);
    ~dvaFourPaneView();

private:
    Ui::dvaFourPaneView *ui;

protected:
    vtkSmartPointer<vtkResliceImageViewer> resliceViewerArr[3];

signals:

private slots:
    void on_renderBtn_clicked();
    void DataUpdated();
};

#endif // DVAFOURPANEVIEW_H
