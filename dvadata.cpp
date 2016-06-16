#include "dvadata.h"

class dvaDataData : public QSharedData
{
public:

};

dvaData::dvaData(QObject *parent) : QObject(parent), data(new dvaDataData)
{
    reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    vtkImageData *imdt = reader->GetOutput();

    //reader->SetDirectoryName("C:/DICOM_resources/MyHead");
    //reader->Update();
    //emit(this->DataUpdated());
}

dvaData::dvaData(const dvaData &rhs) : data(rhs.data)
{
    ;
}

dvaData &dvaData::operator=(const dvaData &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

dvaData::~dvaData()
{
    ;
}


void dvaData::SetStudyPath(const char *path){
    this->reader->SetDirectoryName(path);
    this->reader->SetUpdateExtentToWholeExtent();
    this->reader->Update();
    this->reader->UpdateWholeExtent();

    int extent[6];
    this->reader->GetDataExtent(extent);

    emit this->DataUpdated();
}

vtkImageAlgorithm *dvaData::GetReader()
{
    return this->reader;
}
