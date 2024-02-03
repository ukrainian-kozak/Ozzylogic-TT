#include "udpmodel.h"

UdpModel::UdpModel(const QString& ip)
    : _server(ip)
{
}




void UdpModel::connect(const QSettings& settings)
{
    double value = settings.value("ServerSettings/Value").toDouble();

    int i = 0;
    while (settings.contains("ServerSettings/Port" + QString::number(i))) {
        if (moreThanThree(i)) {
            break;
        }

        quint16 port = settings.value("ServerSettings/Port" + QString::number(i)).toUInt();
        std::shared_ptr<UdpClient> client = std::make_shared<UdpClient>(_server.getHostAddress(), port);

        QObject::connect(client.get(), &UdpClient::sendValue, &_server, &UdpServer::receiveValue);
        QObject::connect(&_server, &UdpServer::signalForReadyRead, client.get(), &UdpClient::callReadyRead);
        QObject::connect(&_server, &UdpServer::finish, client.get(), &UdpClient::saveData);

        _server.addPort(port);
        emit client->sendValue(value);
        _clients.append(client);

        ++i;
    }
}


void UdpModel::show()
{
    _server.show();
}



bool UdpModel::moreThanThree(int amount)
{
    if (amount >= 3) {
        QMessageBox msg;

        msg.setText("Maximum amount of receiving ports are 3!\n");
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();

        return true;
    }

    return false;
}
