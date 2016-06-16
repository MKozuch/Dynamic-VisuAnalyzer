#ifndef DVADATA_H
#define DVADATA_H

#include <QObject>
#include <QSharedDataPointer>

#include <vtkDICOMImageReader.h>
#include <vtkSmartPointer.h>

class dvaDataData;

class dvaData : public QObject
{
    Q_OBJECT

public:
    explicit dvaData(QObject *parent = 0);
    dvaData(const dvaData &);
    dvaData &operator=(const dvaData &);
    ~dvaData();

    vtkImageAlgorithm  *GetReader();

signals:
    void PathChanged(const char*);
    void DataUpdated();

public slots:
    void SetStudyPath(const char*);

private:
    QSharedDataPointer<dvaDataData> data;
    vtkSmartPointer <vtkDICOMImageReader> reader;
};

#endif // DVADATA_H
