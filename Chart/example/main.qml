import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

import CustomItems 1.0
import com.wearily.WeaChart 1.0
import "qrc:/qml" // Importing WeaChart resources.qrc/qml prefix .

Window {
    id: root
    title: "Chart Window"
    width: 1600
    height: 850
    visible: true

    RowLayout {
        anchors.fill: parent
        spacing: 10
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 5
            RowLayout {
                anchors.fill: parent
                spacing: 10
                GroupBox {
                    Layout.fillHeight: true
                    //                    Layout.fillWidth: true
                    //                    Layout.alignment: Qt.AlignTop
                    id: gbControls
                    title: "Controls"
                    ColumnLayout {
                        anchors.fill: parent
                        Layout.margins: 7
                        spacing: 15
                        GroupBox {
                            title: "Chart Style"
                            Layout.fillWidth: true
                            ColumnLayout {
                                anchors.fill: parent
                                spacing: 20
                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 3
                                    Button {
                                        id: btnColorDialog
                                        text: "Background Color"
                                        font.pointSize: 9
                                        onClicked: {
                                            colorDialog.open()
                                        }

                                        ColorDialog {
                                            id: colorDialog
                                            modality: Qt.WindowModal
                                            showAlphaChannel: true
                                            currentColor: "#00171f"
                                        }
                                    }
                                    CheckBox {
                                        id: cbUseGrid
                                        text: "Use Grid: " + qsTr(state)
                                        font.pointSize: 9
                                        checked: true

                                    }
                                    Button {
                                        id: btnColorMajorGrid
                                        text: "Major Grid Color"
                                        font.pointSize: 9
                                        onClicked: {
                                            colorDialogMajor.open()
                                        }

                                        ColorDialog {
                                            id: colorDialogMajor
                                            modality: Qt.WindowModal
                                            showAlphaChannel: true
                                            currentColor: "#179998"
        //                                    currentAlpha: 3
                                        }
                                    }
                                    Button {
                                        id: btnColorMinorGrid
                                        text: "Minor Grid Color"
                                        font.pointSize: 9
                                        onClicked: {
                                            colorDialogMinor.open()
                                        }

                                        ColorDialog {
                                            id: colorDialogMinor
                                            modality: Qt.WindowModal
                                            showAlphaChannel: true
                                            currentColor: "#023c32"
        //                                    currentAlpha: 0.3
                                        }
                                    }

                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 3
                                    Label {
                                        id: lblMajorHGridCnt
                                        text: "Major H Grids: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinGridHMajor
                                        from: -100 * 100
                                        value: 10
                                        to: 100 * 100
                                        stepSize: 1
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinGridHMajor.from, spinGridHMajor.to)
                                            top:  Math.max(spinGridHMajor.from, spinGridHMajor.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinGridHMajor.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }
                                    Item {
                                        Layout.fillWidth: true
                                    }

                                    Label {
                                        id: lblMinorHGridCnt
                                        text: "Minor per Major"
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinGridHMinor
                                        from: -100 * 100
                                        value: 5
                                        to: 100 * 100
                                        stepSize: 1
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinGridHMinor.from, spinGridHMinor.to)
                                            top:  Math.max(spinGridHMinor.from, spinGridHMinor.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinGridHMinor.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }

                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 3
                                    Label {
                                        id: lblMajorVGridCnt
                                        text: "Major V Grids: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinGridVMajor
                                        from: -100 * 100
                                        value: 10
                                        to: 100 * 100
                                        stepSize: 1
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinGridVMajor.from, spinGridVMajor.to)
                                            top:  Math.max(spinGridVMajor.from, spinGridVMajor.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinGridVMajor.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100

                                        }
                                    }
                                    Item {
                                        Layout.fillWidth: true
                                    }

                                    Label {
                                        id: lblMinorVGridCnt
                                        text: "Minor V Grids: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinGridVMinor
                                        from: -100 * 100
                                        value: 100
                                        to: 100 * 100
                                        stepSize: 100
                                        editable: true
                                        enabled: false

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinGridVMinor.from, spinGridVMinor.to)
                                            top:  Math.max(spinGridVMinor.from, spinGridVMinor.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinGridVMinor.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }

                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 3
                                    Label {
                                        id: lblMinX
                                        text: "MinX: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinMinX
                                        from: -360 * 100
                                        value: 0
                                        to: 360 * 100
                                        stepSize: 5
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinMinX.from, spinMinX.to)
                                            top:  Math.max(spinMinX.from, spinMinX.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinMinX.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }
                                    Item {
                                        Layout.fillWidth: true
                                    }

                                    Label {
                                        id: lblMaxX
                                        text: "MaxX: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinMaxX
                                        from: -360 * 100
                                        value: 100
                                        to: 1000 * 3000
                                        stepSize: 5
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinMaxX.from, spinMaxX.to)
                                            top:  Math.max(spinMaxX.from, spinMaxX.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinMaxX.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }

                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 3
                                    Label {
                                        id: lblMinY
                                        text: "MinY: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinMinY
                                        from: -360 * 100
                                        value: 0
                                        to: 1000 * 3000
                                        stepSize: 5
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinMinY.from, spinMinY.to)
                                            top:  Math.max(spinMinY.from, spinMinY.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinMinY.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }
                                    Item {
                                        Layout.fillWidth: true
                                    }
                                    Label {
                                        id: lblMaxY
                                        text: "MaxY: "
                                    }

                                    SpinBox {
                                        Layout.maximumHeight: 30
                                        id: spinMaxY
                                        from: -360 * 100
                                        value: 100
                                        to: 360 * 100
                                        stepSize: 5
                                        editable: true

                                        property int decimals: 2
                                        property real realValue: value / 100

                                        validator: DoubleValidator {
                                            bottom: Math.min(spinMaxY.from, spinMaxY.to)
                                            top:  Math.max(spinMaxY.from, spinMaxY.to)
                                        }

                                        textFromValue: function(value, locale) {
                                            return Number(value / 100).toLocaleString(locale, 'f', spinMaxY.decimals)
                                        }

                                        valueFromText: function(text, locale) {
                                            return Number.fromLocaleString(locale, text) * 100
                                        }
                                    }
                                }

                              }
                          }

                        GroupBox {
                            id: gbSeries
                            title: "Series Configuration"
                            font.pointSize: 10
                            ColumnLayout {
                                Layout.topMargin: 8
                                Layout.bottomMargin: 8
                                RowLayout {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                    spacing: 10
                                    ComboBox {
                                        id: cboxSeries
                                        model: ["Series 0", "Series 1", "Series 2"]
                                    }
                                    CheckBox {
                                        id: cbSeriesVisible
                                        checked: true
                                        text: "Series visible: "
                                    }
                                    ComboBox {
                                        id: cboxSeriesType
                                        model: ["Scatter", "Line", "Area"]
                                    }
                                    ComboBox {
                                        id: cboxSeriesColorType
                                        model: ["MixColor", "Only Base", "Only Point"]
                                    }
                                }
                                RowLayout {
                                    spacing: 10
                                    Rectangle {
                                        color: "#cccccc"
                                        Layout.fillWidth: true
                                        height: 50
                                        TextEdit {
                                            id: txtSeriesName
                                            anchors.fill: parent
                                            horizontalAlignment: TextEdit.AlignHCenter
                                            verticalAlignment: TextEdit.AlignVCenter
                                            text: "Series Name example"
                                            font.bold: true
                                        }
                                    }
                                    Button {
                                        text: "Series Color"
                                        font.pointSize: 9
                                        onClicked: {
                                            colorDialogSeries.open()
                                        }

                                        ColorDialog {
                                            id: colorDialogSeries
                                            modality: Qt.WindowModal
                                            showAlphaChannel: true
                                            currentColor: "#023c32"
                                        }
                                    }
                                    SpinBox {
                                        id: spSeriesMarkerSize
                                        value: 10
                                        from: 0
                                        to: 100
                                        stepSize: 1
                                        editable: true
                                    }


                                }
                                RowLayout {
                                    spacing: 10
                                    ComboBox {
                                        Layout.fillWidth: true
                                        id: cboxSeriesMarkerShape
                                        model: ["ShapeSquare", "ShapeCircle", "ShapeTexture"]
                                    }
                                    ComboBox {
                                        Layout.fillWidth: true
                                        id: cboxSeriesIcon
                                        model: ["IconTargetPressed", "IconUpHovered"]
                                    }
                                    Button {
                                        id: btnSeriesConfig
                                        text: "Update Series"
                                        font.pointSize: 10
                                        highlighted: true
                                        onClicked: gbSeries.updateSeries()
                                    }
                                }

                            }
                            function updateSeries() {
                                let series = null
                                if (cboxSeries.currentValue === "Series 0") {
                                    series = series0
                                }
                                else if (cboxSeries.currentValue === "Series 1") {
                                    series = series1
                                }
                                else if (cboxSeries.currentValue === "Series 2") {
                                    series = series2
                                }
                                series.visible = cbSeriesVisible.checked
                                series.type = cboxSeriesType.currentIndex
                                series.colorType = cboxSeriesColorType.currentIndex
                                series.name = txtSeriesName.text
                                series.color = colorDialogSeries.currentColor
                                series.markerSize = spSeriesMarkerSize.value
                                series.markerShape = cboxSeriesMarkerShape.currentIndex
                                if (cboxSeriesIcon.currentValue === "IconTargetPressed")
                                    series.markerIconUrl = ":/images/targetPressed.png"
                                else if (cboxSeriesIcon.currentValue === "IconUpHovered")
                                    series.markerIconUrl = "qrc:/images/upHovered.png"


                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }

                GroupBox {
                    Layout.fillHeight: true
                    id: gbMonitor
                    title: "Monitor And Status"
                    ColumnLayout {
                        anchors.fill: parent
                        Label {
                            Layout.alignment: Qt.AlignTop
                            id: lblFps
                            width: 75
                            height: 75
                            color: "black"
                            font.pointSize: 11
                            font.bold: true
                            wrapMode: "WordWrap"
                            text: "FPS: " + fpsItem.fps
                            Fps {
                                id: fpsItem
                                width: 1
                                height: 1
                            }
                        }

                    }

                }


            }


        }
        Rectangle {
            id: rect
            Layout.margins: 5
            Layout.fillHeight: true
            width: 800
//            border.color: "gray"
//            color: "cyan"
            GLChartFrame {
                // All properties bellow are optional, see GLChartFrame all properties & default values.
                id: glChartFrame
                anchors.fill: parent
                anchors.margins: 30
                anchors.topMargin: 0
                leftMargin: 100
                topMargin: 80

                useGrid: cbUseGrid.checked
                yLabelDecimalPlaces: 0
                xLabelDecimalPlaces: 0
                majorXCount: spinGridHMajor.value
                majorYCount: spinGridVMajor.value
                minorPerMajor: spinGridHMinor.value
                majorColor: colorDialogMajor.currentColor
                minorColor: colorDialogMinor.currentColor
                backgroundColor: colorDialog.currentColor
                backgroundOpacity: colorDialog.currentAlpha
                xTitle: "This is X Axis"
                yTitle: "Frequency Channel"
//                xTitlePosY: glChartFrame.height - 30
//                yTitlePosX: -10

                selectViewMouseButton: Qt.RightButton
                selectPointsMouseButton: Qt.LeftButton

                minX: spinMinX.value
                minY: spinMinY.value
                maxX: spinMaxX.value
                maxY: spinMaxY.value
                velocityCoefficient: 0.91
                limitView: false
                panMouseButton: Qt.MiddleButton

                GLSeriesItem {
                    id: series0
                    series: series0Ptr // Required Property, See main.cpp for more details.
                    // Optional Properties & Online, see 'PropertySeries.h' for more details & default values.
                    visible: true
                    type: GL.SeriesTypeScatter
                    colorType: GL.MixBasePointColor
                    name: "Series 0"
                    color: "red"
                    markerSize: 50
                    markerShape: GL.ShapeSquare
                    markerIconUrl: ":/images/targetPressed.png"
                }
                GLSeriesItem {
                    id: series1
                    series: series1Ptr // Required Property, See main.cpp for more details.
                    // Optional Properties & Online, see 'PropertySeries.h' for more details & default values.
                    visible: true
                    type: GL.SeriesTypeLine
                    colorType: GL.OnlyBaseColor
                    name: "Series 1"
                    color: "green"
                    markerSize: 50
                    markerShape: GL.ShapeCircle
                    markerIconUrl: ""

                }
                GLSeriesItem {
                    id: series2
                    series: series2Ptr // Required Property, See main.cpp for more details.
                    // Optional Properties & Online, see 'PropertySeries.h' for more details & default values.
                    visible: true
                    type: GL.SeriesTypeArea
                    colorType: GL.OnlyPointColor
                    name: "Series 2"
                    color: "blue"
                    markerSize: 50
                    markerShape: GL.ShapeSquare
                    markerIconUrl: ""
                }
                GLLegend {
                    id: legend
                    chart: glChartFrame.chart
                    visible: true
                    alignment: Qt.AlignHCenter | Qt.AlignTop
                }

            }
        }

    }



}
