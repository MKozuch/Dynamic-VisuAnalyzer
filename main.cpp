#include "dynamicvisuanalyzer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DynamicVisuAnalyzer w;
    w.show();

    return a.exec();
}
