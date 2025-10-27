import QtQuick 2.12
import QtQuick.Controls 2.12
import com.wearily.WeaChart 1.0

Flow {
    id: legendRoot
    property var chart: parent.chart
    property int alignment: Qt.AlignTop
    property font labelFont: Qt.font({family: "Arial", bold: false, pixelSize: 14})
    property color labelColor: "black"
    spacing: 1

    Repeater {
        model: chart.legendItems
        delegate: GLItemLegend {
            type: modelData.type
            shape: modelData.markerShape
            iconUrl: modelData.markerIconUrl
            name: modelData.name
            color: modelData.color
            visible: modelData.visible
            labelColor: legendRoot.labelColor
            labelFont: legendRoot.labelFont
        }
    }

    anchors.margins: 10
    anchors {
        top: alignment & Qt.AlignTop ? parent.top: undefined
        bottom: alignment & Qt.AlignBottom ? parent.bottom: undefined
        left: alignment & Qt.AlignLeft ? parent.left: undefined
        right: alignment & Qt.AlignRight ? parent.right: undefined
        horizontalCenter: alignment & Qt.AlignHCenter ? parent.horizontalCenter : undefined
        verticalCenter: alignment & Qt.AlignVCenter ? parent.verticalCenter : undefined
    }

}
