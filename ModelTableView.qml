import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle {
    // GUI Option & Properties
    property int headerCellHeight: 45
    property int cellHeight: 30
    property int cellWidth: 150
    property int cellSpacing: 0
    property bool fixedHeader: false

    property int headerBorderWidth: 1
    property int cellsBorderWidth: 1

    property color headerCellColor: "orange"
    property var   cellColorList: ["gray", "#c7c7c7", "white"] // For example-> row1.color = gray, row2.color = LightGray, row3.color = gray and ...
    property color selectedCellsColor: "cyan"
    property color hoverCellColor: "yellow"

    property color headerBorderColor: "red"
    property var cellsBorderColorList: cellColorList // Just like the cellColorList By default is sync with cellsColor
    property color selectedCellsBorderColor: selectedCellsColor // By default is sync with cellsColor
    property color hoverCellsBorderColor: hoverCellColor // By default is sync with cellsColor

    property color headerCellTextColor: "purple"
    property var cellTextColorList: ["brown", "coral"]
    property color selectedCellsTextColor: "black"
    property color hoverCellTextColor: "white"

    property var headerCellFont: ({ family: "Arial", pointSize: 13, weight: 300, bold: true})
    property var cellFont: ({ family: "Arial", pointSize: 11,  weight: 300, bold: false})

    // Table Props
    required property var tableModel
    property int hoverRow: -1

    // Ordering Props
    required property var columns /** role: str, title: str, visible: bool, columnWidth: int || null **/
    property int sorColumn: -1
    property bool sortAscending: false

    // Signals
    signal rowClicked(int rowIndex, var rowData)

    color: "green"
    Layout.fillHeight: true
    Layout.fillWidth: true
//    anchors.fill: parent.fill
    // Headers (Columns)
    clip: true
    Row {
        id: tableHeader
        width: tableView.contentWidth; height: headerCellHeight
        x: fixedHeader ? 0 : -tableView.contentX
        z: 1
        spacing: cellSpacing

        Repeater {
            model: columns
            Rectangle {
                width: modelData.columnWidth ? modelData.columnWidth : cellWidth
                height: parent.height
                color: headerCellColor
                border.color: headerBorderColor
                border.width: headerBorderWidth
                visible: modelData.visible
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    x: 4
                    width: parent.width - 4
                    text: modelData.title
                    font.family: headerCellFont.family
                    font.pointSize: headerCellFont.pointSize
                    font.bold: headerCellFont.bold
                    font.weight: headerCellFont.weight
                    color: headerCellTextColor
                    elide: Text.ElideRight
                }
            }

        }
    }
    // Main (Rows)
    TableView {
        id: tableView
//        anchors.fill: parent
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.topMargin: tableHeader.height
        columnSpacing: cellSpacing; rowSpacing: cellSpacing
        model: tableModel
//        columnWidthProvider: function(column) { return Math.min(600, 800)}
        reuseItems: true


        ScrollBar.horizontal: ScrollBar { }
        ScrollBar.vertical: ScrollBar { }
        delegate: Row {

            function getRoleValue(role) {
                return model[role]
            }

            function getColor(row, colorList, colorSelect, colorHover) {
                if (selected)
                    return colorSelect
                if (hoverRow === row)
                    return colorHover
                return colorList[row % colorList.length]
            }

            Repeater {
                model: columns
//                model: targetModel.item.getProperties()
                delegate: Rectangle {
                    id: rowRect
                    color: getColor(row,
                                    cellColorList,
                                    selectedCellsColor,
                                    hoverCellColor)
                    border.color: getColor(row,
                                           cellsBorderColorList,
                                           selectedCellsBorderColor,
                                           hoverCellsBorderColor)
                    implicitWidth: rowValue.width
                    implicitHeight: rowValue.height
                    visible: modelData.visible

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {                            
                            tableModel.selectItem(row)
                            console.log(getRoleValue(modelData.title))
                            console.log(modelData.name, modelData.title, modelData.visible)
                        }
                        onEntered: {
                            hoverRow = row
                        }
                        onExited: {
                            if (hoverRow === row)
                                hoverRow = -1
                        }
                    }

                    Text {
                            id: rowValue
                            text: getRoleValue(modelData.name)
                            font.family: cellFont.family
                            font.pointSize: cellFont.pointSize
                            font.bold: cellFont.bold
                            font.weight: cellFont.weight
                            width: modelData.columnWidth ? modelData.columnWidth : cellWidth
                            height: cellHeight
                            color: getColor(row,
                                            cellTextColorList,
                                            selectedCellsTextColor,
                                            hoverCellTextColor)
                            elide: column === 49 ? Text.ElideLeft : Text.ElideRight
                            font.preferShaping: false
                        }

                    }

                }

            }

        }

    }
