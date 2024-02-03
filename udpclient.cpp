#include "udpclient.h"



UdpClient::UdpClient(const QString& hostAddress, quint16 port,  QObject* pobj) :
    QObject(pobj),
    _host(hostAddress),
    _port(port),
    _thread(nullptr),
    _socket(nullptr)
{
    _socket = std::make_unique<QUdpSocket>();

    _thread.reset(new QThread);
    moveToThread(_thread.get());
    _thread->start();

    if (!_socket->bind(QHostAddress(_host), _port)) {
        qDebug() << "Failed to bind!";
    }
    connect(_socket.get(), &QUdpSocket::readyRead, this, &UdpClient::processPendingDatagrams);
}




UdpClient::~UdpClient()
{
    _thread->quit();
    _thread->wait();
}



void UdpClient::callReadyRead()
{
    emit _socket->readyRead();
}




void UdpClient::processPendingDatagrams()
{
    QByteArray datagram;
     do {
        datagram.resize(_socket->pendingDatagramSize());
        int bytes = _socket->readDatagram(datagram.data(), datagram.size());
        qDebug() << bytes << "bytes were received from port:" << _port;
     } while (_socket->hasPendingDatagrams());

    QDataStream stream(datagram);
    stream.setVersion(QDataStream::Qt_6_6);
    stream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    double value;
    while (!stream.atEnd()) {
        stream >> value;
        data.append(value);
    }
}




void UdpClient::saveData()
{
    static int number = 0;
    std::sort(data.begin(), data.end());

    QFile file("C:\\qt_projects\\ozzylogic\\received_data" + QString::number(_port) + "_" + QString::number(number) + ".bin");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "Failed to open file for writing";
        return;
    }

    QDataStream outFile(&file);
    outFile.setVersion(QDataStream::Qt_6_6);

    for (auto&& num : data) {
        outFile << num;
    }

    file.close();
    data.clear();
    ++number;
}














