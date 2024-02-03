#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QUdpSocket>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include "nonnegativelineedit.h"
#include <random>
#include <unordered_set>



//================================================

class UniqueDoubleGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;
    std::unordered_set<double> generatedValues;

public:
    UniqueDoubleGenerator(double min, double max) : gen(rd()), dis(min, max) {}

    double generateUniqueDouble() {
        double value;
        do {
            value = dis(gen);
        } while (generatedValues.count(value) > 0);
        generatedValues.insert(value);
        return value;
    }
};



//=================================================

class UdpServer : public QWidget {
    Q_OBJECT

    static const int maxPayLoadSize = 4472;

public:
    explicit UdpServer(const QString& hostAddress, QWidget* pwgt = nullptr);

    void addPort(quint16 port);

    QString getHostAddress();

signals:
    void signalForReadyRead();
    void finish();

public slots:
    void receiveValue(double value);
    void sendDatagrams();

private:
    QString _host;
    QVector<quint16> _ports;

    std::unique_ptr<QUdpSocket> _socket;
    std::unique_ptr<QTextEdit> _text;
    std::unique_ptr<NonNegativeLineEdit> _line;

    double _val;

private:
    QVector<double> createArray(const int size);

};

#endif // UDPSERVER_H
