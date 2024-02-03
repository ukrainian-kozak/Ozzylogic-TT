#include <QApplication>
#include <QSettings>
#include "udpmodel.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings("C:\\qt_projects\\ozzylogic\\config.ini", QSettings::IniFormat);
    QString hostAddress = settings.value("ServerSettings/HostAddress").toString();

    UdpModel model(hostAddress);

    model.connect(settings);
    model.show();

    QThread::sleep(3);

    return a.exec();
}
