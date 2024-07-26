#ifndef SERIAL_H
#define SERIAL_H
#include<QObject>
#include <QSerialPort>
#include <QTimer>
#include <QDebug>
class SerialManager : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
    explicit SerialManager(QObject *parent = nullptr);
    QString data();
    ~SerialManager();
    Q_INVOKABLE void startReading();
    void writeled(QString );
signals:
    void dataChanged();

public slots:
    void setData(QString);
    void handleSerialError(QSerialPort::SerialPortError error);
    void readData();


private:
    QString mdata;
    QSerialPort *serialPort;
    QSerialPort *serialLed;
    QTimer *reconnectTimer;
     QString portName,portLed; // Replace with the name of your serial port (e.g., "COM3" or "/dev/ttyACM0")
    const int baudRate; // Replace with the baud rate of your serial communication (e.g., 9600)
};


#endif // SERIAL_H
