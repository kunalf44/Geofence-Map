#ifndef GCP_H
#define GCP_H
#include <QObject>
#include <QDebug>

class Gcp : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString lat READ lat WRITE setLat NOTIFY latChanged)
    Q_PROPERTY(QString lon READ lon WRITE setLon NOTIFY lonChanged)
    explicit Gcp(QObject *parent = nullptr);
    QString lat();
    QString lon();
    Q_INVOKABLE void fetchData(QString);
signals:
    void latChanged();
    void lonChanged();

public slots:
    void setLat(QString);
    void setLon(QString);
private:
    QString mlat,mlon;
};

#endif // GCP_H
