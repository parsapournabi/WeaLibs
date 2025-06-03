import QtQuick 2.15
import QtQuick.Controls 2.15
//import Qt.labs.qmlmodels 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import QtQuick.Controls.Material 2.12
//import QtQuick.Controls.Styles 1.4
import QtQuick.Shapes 1.12
import QtCharts 2.3
import QtLocation 5.6
//import QtPositioning 5.6
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.12
import QtGraphicalEffects 1.12
import CustomItems 1.0

// import "."


ApplicationWindow {
    visible: true
    width: 1250
    height: 800
    title: "Radar with TableView"
    color: "#c7c7c7"

    Rectangle {
        anchors.fill: parent
//        z: 1
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 100


            RadarView {
                Layout.preferredHeight: 500
                Layout.preferredWidth: 500
                Layout.leftMargin: 30
            }
            ModelTableView {
                tableModel: targetModel
                id: table

//                anchors.fill: parent
//                columns: [{"name": "id", "title": "Id", "visible": true, "columnWidth": null},
//                    {"name": "row", "title": "Row", "visible": true, "columnWidth": null},
//                    {"name": "azimuth", "title": "Az", "visible": true, "columnWidth": null},
//                {"name": "elevation", "title": "Elv", "visible": true, "columnWidth": null},
//                {"name": "rangeCell", "title": "Range", "visible": true, "columnWidth": 400},
//                {"name": "power", "title": "Power", "visible": true, "columnWidth": null}]
                columns: targetModel.getProperties(true)

//                cellColorList: ["purple"]
                cellsBorderColorList: ["brown", "dark-blue", "black"]
                selectedCellsBorderColor: "gray"
                hoverCellsBorderColor: "red"

            }

        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 20

            Fps {
                id: fpsItem
                width: 1
                height: 1
                visible: true
            }
            Label {
                id: lblFps
                width: 150
                height: 75
                font.pointSize: 11
                font.bold: true
                text: qsTr("FPS: ") + fpsItem.fps
//                text: qsTr("FPS: ") + fpsC.

            }

            Button {
                text: "Add Target"
                onClicked: targetModel.addTargetFromQml(Math.random()*360, Math.random()*200, Math.random()*200, Math.random()*200)
            }

            Button {
                text: "Remove Last"
                onClicked: {

                    console.log(sbTargetId.value)
                    console.log(Object.keys(table))
                    console.log("And targetModel", targetModel)
                    targetModel.removeItem(sbTargetId.value)
                }
            }
            CheckBox {
                text: "azimuth visible"
                onClicked: {
                    console.log("CheckState", checked)
//                    table.update()
                    console.log("Table Result: ", table.columns[3].visible)
                    targetModel.setProperty("azimuth", "visible", checked)
                    console.log("Table Result: ", table.columns[3].visible)
                    console.log("All data: \n", targetModel.getProperties(true))
                }
            }
            SpinBox {
                id: sbTargetId

            }
        }
    }
}

}

