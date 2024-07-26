#include "gcp.h"
#include <nmea.h>
Gcp::Gcp(QObject *parent) : QObject(parent)
    ,mlat("null")
    ,mlon("null")
{

}

QString Gcp::lat(){
    return mlat;
}
QString Gcp::lon(){
    return mlon;
}

void Gcp::setLat(QString temp){
    if(temp!=""){
        mlat=temp;
    }
    emit lonChanged();
}

void Gcp::setLon(QString temp){
    if(temp!=""){
        mlon=temp;
    }
    emit latChanged();
}

Q_INVOKABLE void Gcp::fetchData(QString data){
    if(data == ""){
        return ;
    }
    Nmea nm;
    nm.parse(data);
        if(nm.m_lat!=""){
        setLat(nm.m_lat);
        }
        if(nm.m_lon!=""){
            setLon(nm.m_lon);
        }
}
