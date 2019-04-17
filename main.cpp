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

    QObject::connect(myBackend, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, myBackend, SLOT(run()));

    QTimer::singleShot(1000, myBackend, SLOT(run2()));


    //QTimer::singleShot(3000, myBackend, SLOT(run3()));
  //QTimer::singleShot(15000, myBackend, SLOT(close()));


    return a.exec();


/*QString s = "F:\\Imagenes\\auntie.jpg";
    std::string mystr = "F:\\Imagenes\\auntie.jpg";
        //int return_value =  SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)mystr.c_str(), SPIF_UPDATEINIFILE);

          bool ret = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)s.utf16(), SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);

    std::cout<<"ending" << std::endl;
    return 0;*/
}
