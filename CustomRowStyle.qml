pragma Singleton
import QtQuick 2.15

QtObject {
//    required property bool selected

    property color defaultColor: "black"
    property color selectedColor: "#e0f7ff"
    property color hoverColor: "cyan"

    property int selectAnimDuration: 300
    property int hoverAnimDuration: 300

    property int rowFont: Qt.font({ family: "Arial", pointSize: 12, bold: true, weight: 300})

    ColorAnimation {
        id: colorAnim
        from: "white"
        to: "black"
        property: "color"
        duration: 2000
    }
    function colorAnimation() {
        return colorAnim
    }

    property Component animationHover: SequentialAnimation {
        PropertyAnimation {
            target: targetParent
            property: "color"
            from: defaultColor
            to: hoverColor
            duration: hoverAnimDuration
        }
    }
}
