#ifndef GEOFENCE_H
#define GEOFENCE_H
#include <QObject>
#include <QDebug>
class Geofence : public QObject
{
    Q_OBJECT
public:
    struct GeoPoint {
        double lat;
        double lon;

    };
    Geofence();
    std::vector<GeoPoint> fencePoints = {};
    Q_INVOKABLE void add_geopoint(double latitude, double longitude);
    Q_INVOKABLE void clear_geopoint();
    bool isInsideGeofence(double lat, double lon);
    Q_INVOKABLE int handleCoordinates(double latitude, double longitude);

private:
    int mans;
};

#endif // GEOFENCE_H
