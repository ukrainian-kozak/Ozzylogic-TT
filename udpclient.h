#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QUdpSocket>
#include <QThread>
#include <QFile>
#include <random>

//================================================


class UdpClient : public QObject {
    Q_OBJECT  

public:
    explicit UdpClient(const QString& hostAddress, quint16 port, QObject* pobj = nullptr);

    ~UdpClient();

signals:
    void sendValue(double value);

public slots:
    void callReadyRead();
    void saveData();

private slots:
    void processPendingDatagrams();

private:
    QString _host;
    quint16 _port;

    QVector<double> data;
    std::unique_ptr<QThread> _thread;
    std::unique_ptr<QUdpSocket> _socket;
};

#endif // UDPCLIENT_H
