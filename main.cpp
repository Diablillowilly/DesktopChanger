
#include <QCoreApplication>
#include <QtCore>
#include "backend.hpp"

#include <iostream> //cout/cin etc
#include <string.h> //pra strings

#include <windows.h>
int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

    Backend *myBackend = new Backend(&a);

    QObject::connect(myBackend, SIGNAL(close()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, myBackend, SLOT(start()));



    return a.exec();
}
