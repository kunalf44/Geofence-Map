#include "serial.h"
#include<QDebug>
#include <QSerialPortInfo>
// #include "nmea.h"
#include <geofence.h>
//with LED
SerialManager::SerialManager(QObject *parent) : QObject(parent),
    mdata("null"),
    portName("ttyUSB1"), // Replace with the name of your serial port
    portLed("ttyUSB0"),
    baudRate(115200) // Replace with the baud rate of your serial communication
{
    QString usbport,usbled;
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &portInfo, ports) {
        qDebug() << "Port Name:" << portInfo.portName();
        qDebug() << "Description:" << portInfo.description();
        qDebug() << "Manufacturer:" << portInfo.manufacturer();
        if(portInfo.manufacturer() == "Prolific Technology Inc."){
            usbport=portInfo.portName();
        }
        if(portInfo.manufacturer() == /*"1a86"*/"FTDI"){
            usbled=portInfo.portName();
        }
    }
    portName=usbport;
    portLed=usbled;
    serialPort = new QSerialPort(this);
    serialLed = new QSerialPort(this);
    reconnectTimer = new QTimer(this);

    connect(serialPort, &QSerialPort::errorOccurred, this, &SerialManager::handleSerialError);
    connect(serialPort, &QSerialPort::readyRead, this, &SerialManager::readData);
    connect(reconnectTimer, &QTimer::timeout, this, &SerialManager::startReading);
}

QString SerialManager::data()
{
    return mdata;
}
void SerialManager::setData(QString temp)
{
    if(temp!=""){
        mdata=temp;
    }
    emit dataChanged();
}
SerialManager::~SerialManager()
{
    if (serialPort->isOpen())
        serialPort->close();

    if (serialLed->isOpen())
        serialLed->close();

}

Q_INVOKABLE void SerialManager::startReading()
{
    if (serialPort->isOpen())
        serialPort->close();

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite))
    {
        qDebug() << "Serial port opened:" << portName;
    }
    else
    {
        QString usbport;
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        foreach (const QSerialPortInfo &portInfo, ports) {
            qDebug() << "Port Name:" << portInfo.portName();
            qDebug() << "Description:" << portInfo.description();
            qDebug() << "Manufacturer:" << portInfo.manufacturer();
            if(portInfo.manufacturer() == "Prolific Technology Inc."){
                usbport=portInfo.portName();
            }
        }
        portName=usbport;
        qDebug() << "Failed to open serial port:" << portName;
        reconnectTimer->start(5000); // Retry after 5 seconds
    }
    //For LED serial writes
    if (serialLed->isOpen())
        serialLed->close();

    serialLed->setPortName(portLed);
    serialLed->setBaudRate(baudRate);
    serialLed->setDataBits(QSerialPort::Data8);
    serialLed->setParity(QSerialPort::NoParity);
    serialLed->setStopBits(QSerialPort::OneStop);
    serialLed->setFlowControl(QSerialPort::NoFlowControl);

    if (serialLed->open(QIODevice::ReadWrite))
    {
        qDebug() << "Serial port opened:" << portLed;
    }
    else
    {
        QString usbled;
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        foreach (const QSerialPortInfo &portInfo, ports) {
            qDebug() << "Port Name:" << portInfo.portName();
            qDebug() << "Description:" << portInfo.description();
            qDebug() << "Manufacturer:" << portInfo.manufacturer();
            if(portInfo.manufacturer() == /*"1a86"*/"FTDI"){
                usbled=portInfo.portName();
            }
        }
        portLed=usbled;
        qDebug() << "Failed to open serial port:" << portLed;
        reconnectTimer->start(5000); // Retry after 5 seconds
    }
}

void SerialManager::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        qDebug() << "Serial port disconnected.";
        reconnectTimer->start(5000); // Retry after 5 seconds
    }
}

QString previous="";
void SerialManager::writeled(QString a){
    if(previous=="" || previous != a){

    QByteArray buf = a.toLocal8Bit();
    qint64 bytesWritten = serialLed->write(buf);
    previous=a;
    if (bytesWritten == -1) {
        qWarning("Failed to write to serial port: %s", qPrintable(serialLed->errorString()));
        return ;
    } else {
        qDebug("Successfully wrote %lld bytes to the serial port", bytesWritten);
    }

    }
    else{
        return;
    }
}

QByteArray ba,line;
QString templa,templo;
bool ledflag=false;
int i=0;
void SerialManager::readData()
{
    // Nmea nm;
    QByteArray newData = serialPort->readAll();
      QString str="kk";
    // Process the received data here
    ba += newData;

    // Process the received data
    int newlineIndex;
    while ((newlineIndex = ba.indexOf('\n')) != -1) {

        //
        line = ba.left(newlineIndex);
        // Handle the line of data here
        qDebug() << "Received line of data:" << line;
        str=QString(line);
        qDebug()<<"str is"<<str;
        setData(str);
        //geofence part
        // handleCoordinates(mlat.toDouble(),mlon.toDouble());
        // //writing data for LED
        // QString a="";
        // if(mans==0){//not ready
        //     a="1";//blinking
        // }
        // else if(mans==1){//Inside
        //     a="2";//stop
        // }
        // else if(mans==2){//outside geo
        //     a="0";//continous blink
        // }
        //     writeled(a);
        //geofence part
        ba.remove(0, newlineIndex + 1);
    }
}


