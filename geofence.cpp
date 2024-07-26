#include "geofence.h"

Geofence::Geofence() {}



Q_INVOKABLE void Geofence::add_geopoint(double latitude, double longitude){
    GeoPoint x;
    x.lat=latitude;
    x.lon=longitude;
    fencePoints.push_back(x);
    //        qDebug()<<fencePoints[0];
}

Q_INVOKABLE void Geofence::clear_geopoint(){
    if(!fencePoints.empty()){
    fencePoints.clear();
    }
}

bool Geofence::isInsideGeofence(double lat, double lon) {
    bool inside = false;
    for (int i = 0, j = fencePoints.size() - 1; i < fencePoints.size(); j = i++) {
        if (((fencePoints[i].lat > lat) != (fencePoints[j].lat > lat)) &&
            (lon < (fencePoints[j].lon - fencePoints[i].lon) * (lat - fencePoints[i].lat) / (fencePoints[j].lat - fencePoints[i].lat) + fencePoints[i].lon)) {
            inside = !inside;
        }
    }
    return inside;
}

Q_INVOKABLE int Geofence::handleCoordinates(double latitude, double longitude)
{
    // Do something with the latitude and longitude values
    //        QGeoCoordinate coordinate(latitude, longitude);
    qDebug()<<"Handle cordinates"<<latitude<<"::"<< longitude;
    if(fencePoints.size()<1){
        qDebug()<<"zero array";
        mans=0;

    }
    else{
        for (int i = 0/*, j = fencePoints.size() - 1*/; i < fencePoints.size(); /*j =*/ i++) {
            qDebug()<<fencePoints[i].lat<<" "<<fencePoints[i].lon<<Qt::endl;
        }
        if (isInsideGeofence(latitude, longitude)) {
            mans=1;
            qDebug() << "User is inside the geofence"<<mans;


        } else {
            mans=2;
            qDebug() << "User is outside the geofence"<<mans;


        }
    }
           return mans;
}


