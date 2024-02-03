#include "udpserver.h"

UdpServer::UdpServer(const QString& hostAddress, QWidget* pwgt) :
    QWidget(pwgt),
    _host(hostAddress),
    _socket(nullptr),
    _text(nullptr),
    _line(nullptr),
    _val(0)
{
    setWindowTitle("Udp Server");

    _socket = std::make_unique<QUdpSocket>(this);
    _text = std::make_unique<QTextEdit>();
    _line = std::make_unique<NonNegativeLineEdit>();

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    QPushButton* pcmd = new QPushButton("&Click!");
    connect(pcmd, &QPushButton::clicked, this, &UdpServer::sendDatagrams);

    pvbxLayout->addWidget(_text.get());
    pvbxLayout->setSpacing(1);
    pvbxLayout->addWidget(_line.get());
    pvbxLayout->addWidget(pcmd);
    setLayout(pvbxLayout);
}




void UdpServer::addPort(quint16 port)
{
    _ports.push_back(port);
}

QString UdpServer::getHostAddress()
{
    return _host;
}

void UdpServer::receiveValue(double value)
{
    _val = value;
    _text->append("Received value: " + QString::number(_val));
}



void UdpServer::sendDatagrams()
{
    int size = 1000000;
    if (_line->text() != "") {
        size = _line->text().toInt();
    }

    QVector<double> data = createArray(size);
    int numChunks = (size * sizeof(double) + maxPayLoadSize - 1) / maxPayLoadSize;

    for (int i = 0; i < numChunks; ++i) {
            int startIndex = i * maxPayLoadSize / sizeof(double);
            int endIndex = qMin(data.size(), static_cast<qsizetype>((i + 1) * maxPayLoadSize / sizeof(double)));

            QByteArray chunk;
            QDataStream stream(&chunk, QIODevice::WriteOnly);
            stream.setVersion(QDataStream::Qt_6_6);
            stream.setFloatingPointPrecision(QDataStream::DoublePrecision);

            for (int j = startIndex; j < endIndex; ++j) {
                stream << data[j];
            }

            for (auto&& port : _ports) {
                _socket->writeDatagram(chunk,QHostAddress(_host), port);
            }
            emit signalForReadyRead();
            QThread::msleep(1);
    }

    _text->append("An array of " + QString::number(data.size()) + " elements has just been sent");

    emit finish();
}





QVector<double> UdpServer::createArray(const int size)
{
    QVector<double> vec(size);

    double min_val = _val > 0 ? (_val *= -1) : _val;
    double max_val = _val < 0 ? (_val *= -1) : _val;

    UniqueDoubleGenerator generator(min_val, max_val);

    for (int i = 0; i < size; ++i) {
        vec[i] = generator.generateUniqueDouble();
    }

    return vec;
}
