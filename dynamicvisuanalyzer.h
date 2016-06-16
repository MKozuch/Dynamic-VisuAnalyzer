#ifndef DYNAMICVISUANALYZER_H
#define DYNAMICVISUANALYZER_H

#include <QMainWindow>
#include <QPointer>
#include <QList>

#include <vtkAlgorithmOutput.h>

#include "Widgets/dvawidget.h"
#include "dvadata.h"

namespace Ui {
class DynamicVisuAnalyzer;
}


class DynamicVisuAnalyzer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DynamicVisuAnalyzer(QWidget *parent = 0);
    ~DynamicVisuAnalyzer();

private slots:
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();

private:
    Ui::DynamicVisuAnalyzer *ui;
    QList<QPointer<dvaWidget>> activeWidgetsList;
    QList<QPointer<dvaWidget>> availableWidgets;
    dvaData *imageData;
};

#endif // DYNAMICVISUANALYZER_H
