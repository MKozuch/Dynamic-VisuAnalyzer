#ifndef DVAPROGRAMMABLEWIDGET_H
#define DVAPROGRAMMABLEWIDGET_H

#include <QWidget>

#include "dvawidget.h"

namespace Ui {
class dvaProgrammableWidget;
}

class dvaProgrammableWidget : public dvaWidget
{
    Q_OBJECT

public:
    explicit dvaProgrammableWidget(QWidget *parent = 0, vtkImageAlgorithm *inputalgo=0);
    ~dvaProgrammableWidget();

private:
    Ui::dvaProgrammableWidget *ui;

public slots:
    void DataUpdated();

};

#endif // DVAPROGRAMMABLEWIDGET_H
