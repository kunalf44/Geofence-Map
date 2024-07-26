import QtQuick
import QtLocation 5.15
import QtPositioning
import QtWebEngine
import QtWebChannel 1.0
import QtLocation 6.5
import QtQuick.Layouts
import kunal 2.0
import kunal 3.0
import QtQuick.Controls
import QtQuick.Dialogs

//import kunal 3.0

Window {
    id:window
    width: 800
    height:480
    visible:true
    title:qsTr('Geo Fencing')

   Component.onCompleted: {
       serial.startReading()
       selectfarm_popup.open()
   }

    property var clickedPoint;
    property string geoVal: "Geofence active" ;

    //plot button object
    QtObject {
        id: plotObject
        property var vis: [];
        property var pt: [gcp.lat, gcp.lon];
        property var check: 0;


        WebChannel.id: "plotPoly"
        signal someSignal();
        signal someSignal4()
        onSomeSignal: pt = [gcp.lat, gcp.lon];
        onVisChanged: addGeopoint()
    }

    //redo button object
    QtObject {
        id: redoObject

        WebChannel.id: "redoPoly"

        signal someSignal2();

    }

    QtObject {
            id: loadObject
            property var live: [gcp.lat, gcp.lon];

            WebChannel.id: "loadObj"

            signal someSignal3();
            onSomeSignal3: {live= [gcp.lat, gcp.lon]
            console.log(loadObject.live)
            }

        }



    Net{
        id: myclass
    }

    Geofence{
        id: geofence
    }


    function addGeopoint(){
        var coords = plotObject.vis
        var string = ""
        if(coords.length >0){
        for(var i=0; i<=coords.length; i++){
            geofence.add_geopoint(coords[i][0], coords[i][1])
            console.log('point added and latlng:', coords[i][0], coords[i][1])
            var split = coords[i][0] + "  " + coords[i][1] + "\n"
            string = string + split
            window.clickedPoint = string
        }
        }
    }

    function checkGeopoint(){
        console.log("Checkgeo"+gcp.lat+"::"+gcp.lon)
        var check = geofence.handleCoordinates(gcp.lat,gcp.lon)
        console.log(check
                    +"from qml")
            if(check===1){
                window.geoVal = "You are inside the geofence."
                bulb.color = "green"
                console.log("inside js true")
            }
            else if(check===2){
                window.geoVal = "You are outside the geofence!"
                bulb.color = "red"
                console.log("inside js false", plotObject.pt)
            }
            else if(check===0){
                window.geoVal = "GeoFence is active"
                bulb.color = "lightgrey"
            }

            console.log('header works')
        }

    Timer {
            id: timer2
            interval: 500;
            running: true;
            repeat: true

            onTriggered: {
                checkGeopoint()
            }
        }
    Timer {
            id: timer3
            interval: 100;
            running: false;
            repeat: true

            onTriggered: {
                plotObject.pt = [gcp.lat, gcp.lon]
                // geofence.handleCoordinates(gcp.lat,gcp.lon)
                plotObject.someSignal4()
            }
        }
    Timer {
            interval: 100;
            running: true;
            repeat: true

            onTriggered: {
                console.log("status of geofence "+geofence.handleCoordinates(gcp.lat,gcp.lon)+gcp.lat+gcp.lon);
                console.log("serial raw data "+serial.data);
                gcp.fetchData(serial.data)
                console.log("GCP raw data "+gcp.lat+" "+gcp.lon);
            }
        }

    Rectangle{
        z: 2
        color: "lightgrey"
        anchors.right: parent.right
        height: 600
        width: 250

        ColumnLayout{
                anchors.horizontalCenter:  parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 120

                spacing : 40
                RoundButton {
                    // z: 3
                    // y: 20
                    // x: 50
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 200
                    radius: 10
                    id: polygonPlot_btn
                    text: "Activate Geofence"
                    font.pixelSize: 18
                    onPressed: {
                        plotObject.someSignal()
                        polygonPlot_btn.visible=false
                        plotObject.pt = " "
                        geo_txt.opacity=1
                        dir_txt.opacity=0
                        timer3.running=true
                        checkGeopoint()
                        cancel_btn.visible=true
                        redo_btn.visible=false

                    }

                }

                RoundButton {
                    id: redo_btn
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 200
                    radius: 10
                    text: "Redo Polygon"
                    font.pixelSize: 18
                    visible: true
                    onClicked: {
                        redoObject.someSignal2()
                        geofence.clear_geopoint()
                        window.clickedPoint = " "
                        geo_txt.opacity = 0
                        dir_txt.opacity=1
                        timer3.running=false

                    }
                }
                RoundButton {
                    id: cancel_btn
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 200
                    radius: 10
                    text: "Cancel"
                    font.pixelSize: 18
                    visible: false
                    onClicked: {
                        redoObject.someSignal2()
                        // polygonPlot_btn.opacity = 1
                        geofence.clear_geopoint()
                        window.clickedPoint = " "
                        geo_txt.opacity = 0
                        dir_txt.opacity=1
                        timer3.running=false
                        cancel_btn.visible=false
                        redo_btn.visible=true
                        polygonPlot_btn.visible=true
                    }
                }
                Text{
                    text: "Latitude: "+gcp.lat
                    font.pixelSize: 18
                    font.bold: true
                }
                Text{
                    text: "Longitude: "+gcp.lon
                    font.pixelSize: 18
                    font.bold: true
                }

        }
    }

    WebChannel {
        id: channel
        registeredObjects: [plotObject, redoObject, loadObject]

    }

    WebEngineProfile {
        id:profile
        httpAcceptLanguage: "en-US,en;q=0.9,es;q=0.8,de;q=0.7";
        httpCacheType: WebEngineProfile.DiskHttpCache
    }

    WebEngineView{
        z: 1
        id:web
        // anchors.fill : parent
        height: 480
        width: 550
        anchors.left: parent.left
        profile: profile
        url:"qrc:/mapbuild.html"
        webChannel: channel

        onJavaScriptConsoleMessage: (level,message,ln,sid)=>{
                                        console.log(message);
                                    }

        onFeaturePermissionRequested: {
            console.log("Honcho");
        }

        Rectangle{
            id: geo_txt_cover
            z: 2
            color: "#1c1c1c"
            radius: 5
            anchors.left: parent.left
            height: 55
            width: 350
            opacity: 0.8
            Rectangle{
                z: 2
                id: bulb_cover
                anchors.top: parent.top
                anchors.topMargin: 7
                anchors.left: parent.left
                anchors.leftMargin: 7
                radius: 40
                height: 40
                width: 40
                opacity: 1
                color: "#555555"

                Rectangle{
                    id: bulb
                    x: 5
                    y: 5
                    radius: 30
                    height: 30
                    width: 30
                    opacity: 1
                    color: "lightgrey"
                }
            }
            Text{
                id: geo_txt
                color: "white"
                font.pointSize: 13
                x: 50
                y: 15
                text: window.geoVal
                opacity: 0
            }
            Text{
                id: dir_txt
                color: "white"
                font.pointSize: 13
                x: 50
                y: 15
                text: "Mark the plot to activate geofence."
                opacity: 1
            }
        }

        Button{
            z:2
            id: reload_btn
            height: 50
            width: 50
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            Image {
                z: 4
                x: 5
                y: 5
                id: img
                height: 40
                width: 40
                source: "qrc:/reload.png"
            }
            onClicked: {
                web.reload()
                redoObject.someSignal2()
                polygonPlot_btn.opacity = 1
                serial.clear_geopoint()
                window.clickedPoint = " "
                geo_txt.opacity = 0
                dir_txt.opacity=1
                liveloc.restart()
            }

        }
        Popup{
            id: selectfarm_popup
            width: 320
            height: 160
            modal: true
            focus: true
             anchors.centerIn:parent
            closePolicy: Popup.CloseOnEscape

            Rectangle {
                anchors.fill: parent
                color: "#ffffff"
                border.color: "#d7d2c9"
                border.width: 1
                radius: 10
            ColumnLayout{
                anchors.fill: parent
                anchors.margins: 10
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                spacing: 20
            Text {
                id: selectfarm_popup_text
                // anchors.centerIn: parent
                font.pointSize: 16
                font.family: "Helvetica"
                color: "black"
                text:"Click on the map to select \n your farm boundary."
            }
            RoundButton{
                id: selectfarm_popup_but
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: "OK"
                Layout.preferredHeight: 50
                Layout.preferredWidth: 70
               onClicked: {

                  selectfarm_popup.close()

               }
            }
            }
            }
        }
    }


    Timer{
        id: liveloc
    // interval: 3000
    running: true
    repeat: true
    onTriggered: {
        loadObject.someSignal3()
                  }

    }



}


