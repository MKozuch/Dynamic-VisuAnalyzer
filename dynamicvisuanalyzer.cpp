#include "dynamicvisuanalyzer.h"
#include "ui_dynamicvisuanalyzer.h"

#include "Widgets/dvafourpaneview.h"
#include "Widgets/dvawidget.h"
#include "Widgets/skullwidget.h"
#include "Widgets/dvaprogrammablewidget.h"

#include <QPointer>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>


DynamicVisuAnalyzer::DynamicVisuAnalyzer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DynamicVisuAnalyzer)
{
    ui->setupUi(this);
    imageData = new dvaData(this);



    QPointer<dvaWidget> viewer = new dvaFourPaneView(this, imageData->GetReader());
    QPointer<dvaWidget> prog = new  dvaProgrammableWidget(this, imageData->GetReader());

    this->availableWidgets.append(viewer);
    this->availableWidgets.append(prog);

    dvaWidget *item;
    foreach(item, availableWidgets){
        QObject::connect(this->imageData, SIGNAL(DataUpdated()), item, SLOT(DataUpdated()));
        this->hide();
    }

    this->activeWidgetsList.append(viewer);
    ui->tabWidget->addTab(viewer, viewer->GetWidgetName());

    QObject::connect(this->imageData, SIGNAL(DataUpdated()), viewer, SLOT(DataUpdated()));
}

DynamicVisuAnalyzer::~DynamicVisuAnalyzer()
{
    delete ui;
    delete imageData;
}

void DynamicVisuAnalyzer::on_actionOpen_triggered()
{
    //TODO: zweryfikj czy folder zawiera pliki dicom
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::ShowDirsOnly);
    fileDialog.setViewMode(QFileDialog::List);
    fileDialog.setDirectory(QString("C:\\DICOM_resources\\MyHead"));
    int dialogExitCode = fileDialog.exec();
    if(dialogExitCode){
        QString path = fileDialog.selectedFiles()[0];
        this->imageData->SetStudyPath(path.toUtf8().constData());
    }
}

void DynamicVisuAnalyzer::on_actionAbout_triggered()
{
    QString aboutMsg = QString("DICOM DynamicVisuAnalyzer \n by: MKozuch");
    QMessageBox aboutDialog;
    aboutDialog.setText(aboutMsg);
    aboutDialog.setIcon(QMessageBox::Information);
    aboutDialog.exec();
}

void DynamicVisuAnalyzer::on_actionQuit_triggered()
{
    QApplication::quit();
}
