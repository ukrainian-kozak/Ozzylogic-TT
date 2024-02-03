#ifndef UDPMODEL_H
#define UDPMODEL_H

#include <QSettings>
#include "udpclient.h"
#include "udpserver.h"

//===============================================

class UdpModel
{
public:
    explicit UdpModel(const QString& ip);

    void connect(const QSettings& settings);
    void show();

private:
    UdpServer _server;
    QVector<std::shared_ptr<UdpClient>> _clients;

private:
    bool moreThanThree(int amount);
};

#endif // UDPMODEL_H
