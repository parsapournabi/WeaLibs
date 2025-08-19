import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

import CustomItems 1.0

Window {
    id: root
    title: "Chart Window"
    width: 1500
    height: 850
    visible: true
    //    GLChartFrame {
    //        anchors.fill: parent
    //        anchors.margins: 30
    //    }

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
                        spacing: 6
                        RowLayout {
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
                                }
                            }
                            CheckBox {
                                id: cbUseGrid
                                text: "Use Grid: " + qsTr(state)
                                font.pointSize: 9

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
                                }
                            }

                        }

                        RowLayout {
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
                                onValueChanged: mainClass.doStuff()
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
                                to: 360 * 100
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
                                to: 360 * 100
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

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }

                GroupBox {
                    Layout.fillHeight: true
                    //                    Layout.fillWidth: true
                    //                    Layout.alignment: Qt.AlignTop
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
            border.color: "gray"
            GLChartFrame {
                id: glChartFrame
                anchors.fill: parent
                anchors.margins: 30

                useGrid: cbUseGrid.checked
                decimalPlaces: 2
                leftMargin: 80
                majorXCount: spinGridHMajor.value
                majorYCount: spinGridVMajor.value
                minorPerMajor: spinGridHMinor.value
                majorColor: colorDialogMajor.currentColor
                minorColor: colorDialogMinor.currentColor
                backgroundColor: colorDialog.currentColor
                backgroundOpacity: colorDialog.currentAlpha
                minX: spinMinX.value
                minY: spinMinY.value
                maxX: spinMaxX.value
                maxY: spinMaxY.value
            }
        }

    }



}
