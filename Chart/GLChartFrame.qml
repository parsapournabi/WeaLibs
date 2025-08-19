import QtQuick 2.15
import GLItems 1.0

Item {
    id: root
    property bool useGrid: true
    property int decimalPlaces: 2
    property int majorXCount: 7
    property int majorYCount: 7
    property int minorPerMajor: 4
    property color majorColor: "#666"
    property color minorColor: "#aaa"
    property color axisColor: "black"
    property color backgroundColor: "blue"
    property real backgroundOpacity: 0.3
    property string xTitle: "X Axis"
    property string yTitle: "Y Axis"
    property font labelFont: Qt.font({ pixelSize: 12})
    property real minX: 0
    property real maxX: 10.0
    property real minY: 0
    property real maxY: 10.0

    property int leftMargin: 40
    property int rightMargin: 20
    property int bottomMargin: 30
    property int topMargin: 20

    width: 300
    height: 400

    onUseGridChanged: canvas.requestPaint()
    onMajorXCountChanged: canvas.requestPaint()
    onMajorYCountChanged: canvas.requestPaint()
    onMinorPerMajorChanged: canvas.requestPaint()
    onMajorColorChanged: canvas.requestPaint()
    onMinorColorChanged: canvas.requestPaint()
    onBackgroundColorChanged: canvas.requestPaint()
    onBackgroundOpacityChanged: canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            let plotX = root.leftMargin;
            let plotY = root.topMargin;
            let plotW = width - root.leftMargin - root.rightMargin;
            let plotH = height - root.topMargin - root.bottomMargin;

            ctx.fillStyle = Qt.rgba(backgroundColor.r,
                                    backgroundColor.g,
                                    backgroundColor.b,
                                    backgroundOpacity);
            ctx.fillRect(plotX, plotY, plotW, plotH);

            ctx.strokeStyle = root.minorColor;
            ctx.lineWidth = 1;

            // Minor Grid
            if (useGrid) {
                for (let i = 0; i <= root.majorXCount; ++i) {
                    if (i >= root.majorXCount)
                        continue;
                    let xMajor = plotX + i * (plotW / root.majorXCount);
                    for (let m = 1; m < root.minorPerMajor; ++m) {
                        let xMinor = xMajor + m * (plotW / root.majorXCount / root.minorPerMajor);
                        ctx.beginPath();
                        ctx.moveTo(xMinor, plotY);
                        ctx.lineTo(xMinor, plotY + plotH);
                        ctx.stroke();
                    }
                }

                for (let j = 0; j <= root.majorYCount; ++j) {
                    if (j >= root.majorYCount)
                        continue;
                    let yMajor = plotY + j * (plotH / root.majorYCount);
                    for (let n = 1; n < root.minorPerMajor; ++n) {
                        let yMinor = yMajor + n * (plotH / root.majorYCount / root.minorPerMajor);
                        ctx.beginPath();
                        ctx.moveTo(plotX, yMinor);
                        ctx.lineTo(plotX + plotW, yMinor);
                        ctx.stroke();
                    }
                }
            }

            // Major Grid
            if (useGrid) {
                ctx.strokeStyle = root.majorColor;
                for (let k = 0; k <= root.majorXCount; ++k) {
                    let x = plotX + k * (plotW / root.majorXCount);
                    ctx.beginPath();
                    ctx.moveTo(x, plotY);
                    ctx.lineTo(x, plotY + plotH);
                    ctx.stroke();
                }
                for (let l = 0; l <= root.majorYCount; ++l) {
                    let y = plotY + l * (plotH / root.majorYCount);
                    ctx.beginPath();
                    ctx.moveTo(plotX, y);
                    ctx.lineTo(plotX + plotW, y);
                    ctx.stroke();
                }
            }

            // Axes
            ctx.strokeStyle = root.axisColor;
            ctx.lineWidth = 2;
            ctx.beginPath();
            ctx.moveTo(plotX, plotY);
            ctx.lineTo(plotX, plotY + plotH);
            ctx.moveTo(plotX, plotY + plotH);
            ctx.lineTo(plotX + plotW, plotY + plotH);
            ctx.stroke();
        }
        GLChartView {
            id: glChartView
            anchors.fill: canvas
            anchors.topMargin: root.topMargin
            anchors.bottomMargin: root.bottomMargin
            anchors.rightMargin: root.rightMargin
            anchors.leftMargin: root.leftMargin

            background.opacity: 0.0 // Custom Prop
            background.color: Qt.rgba(0, 0, 0, 0) // Custom Prop
            axisRange.minX: minX // Custom Prop
            axisRange.maxX: maxX // Custom Prop
            axisRange.minY: minY // Custom Prop
            axisRange.maxY: maxY // Custom Prop
        }
        // Rubber Band rectangle
        Rectangle {
            id: rectRubBand
            color: Qt.rgba(0, 120/255, 215/255, 0.3)
            border.color: "#0078d7"
            border.width: 1
            visible: mouseItem.selecting

        }
        MouseArea {
            property point startPos
            property bool selecting: false
            id: mouseItem
            anchors.fill: canvas
            acceptedButtons: Qt.LeftButton
            onPressed: (mouse) =>
                       {
                           startPos = Qt.point(mouse.x, mouse.y)
                           selecting = true
                           rectRubBand.x = startPos.x
                           rectRubBand.y = startPos.y
                           rectRubBand.width = 0
                           rectRubBand.height = 0
                           glChartView.preSelect(!(mouse.modifiers & Qt.ControlModifier))

                       }
            onPositionChanged: {
                if (!selecting)
                    return
                let x = Math.max(Math.min(mouse.x, startPos.x), leftMargin)
                let y = Math.max(Math.min(mouse.y, startPos.y), topMargin)
                let w = Math.abs(Math.max(x, mouse.x) - startPos.x)
                let h = Math.abs(Math.max(y, mouse.y) - startPos.y)
                rectRubBand.x = x
                rectRubBand.y = y
                rectRubBand.width = x + w >= width - rightMargin ? width - rightMargin - x : w
                rectRubBand.height = y + h >= height - bottomMargin ? height - bottomMargin - y : h

                glChartView.rangeSelecting(Qt.rect(rectRubBand.x - leftMargin, rectRubBand.y - topMargin, rectRubBand.width, rectRubBand.height))
                //                console.log("Rectangle Size: ", rectRubBand.x, rectRubBand.y, rectRubBand.width, rectRubBand.height, mouse.x, mouse.y, glChartView.width, glChartView.height)
            }
            onReleased: {
                selecting = false
                glChartView.selectFinished()
                //                glChartView.rangeSelecting(Qt.rect(0.0, 0.0, 0.0, 0.0))
            }

        }
    }

    // Labels for major X
    Repeater {
        model: majorXCount + 1
        delegate: Text {
            x: root.leftMargin + index * ((root.width - root.leftMargin - root.rightMargin) / root.majorXCount)
            y: root.height - root.bottomMargin + 4
            font: root.labelFont
            //            text: (root.minX + index * ((root.maxX - root.minX) / root.majorXCount)).toFixed(decimalPlaces)
            text: (glChartView.projLeft + index * ((glChartView.projRight - glChartView.projLeft) / root.majorXCount)).toFixed(decimalPlaces)
        }
    }
    // Labels for major Y
    Repeater {
        model: majorYCount + 1
        delegate: Text {
            x: 0
            y: root.topMargin + index * ((root.height - root.topMargin - root.bottomMargin) / root.majorYCount) - 8
            font: root.labelFont
            //            text: (root.maxY - index * ((root.maxY - root.minY) / root.majorYCount)).toFixed(decimalPlaces)
            text: (glChartView.projTop - index * ((glChartView.projTop - glChartView.projBottom) / root.majorYCount)).toFixed(decimalPlaces)
        }
    }

    // X Title
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height - 5
        font.pixelSize: 14
        text: root.xTitle
    }
    // Y Title
    Text {
        anchors.verticalCenter: parent.verticalCenter
        x: 0
        rotation: -90
        font.pixelSize: 14
        text: root.yTitle
    }

}
