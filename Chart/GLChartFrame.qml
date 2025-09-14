import QtQuick 2.15
import QtQuick.Controls 2.15
import GLItems 1.0

Item {
    id: root
    property bool useGrid: true
    property int decimalPlaces: 2
    property int majorXCount: 7 // Number of X major Grid
    property int majorYCount: 7 // Number of Y major Grid
    property int minorPerMajor: 4 // Number of sub grids per each majorGrid.
    property color majorColor: "#666"
    property color minorColor: "#aaa"
    property color axisColor: "black"
    property color backgroundColor: "blue"
    property real backgroundOpacity: 0.3
    property string xTitle: "X Axis"
    property string yTitle: "Y Axis"
    property font titleFont: Qt.font({ pixelSize: 13, bold: true})
    property color titleColor: "black"
    property font labelFont: Qt.font({ pixelSize: 11})
    property color labelColor: "black"
    property string xLabelSuffix: "" // For example xLabelSuffix: "$", then value from 100.0 goes to 100.0$
    property string yLabelSuffix: "" // Same as xLabelSuffix

    property int leftMargin: 40
    property int rightMargin: 20
    property int bottomMargin: 30
    property int topMargin: 20

    // Mouse Buttons
    property int selectPointsMouseButton: Qt.LeftButton // See mouseItemSelect id for more detail
    property int selectViewMouseButton: Qt.RightButton // See mouseItemView id for more detail

    // ReadOnly
    property var chart: glChartView

    // Chart Properties
    property real minX: 0
    property real maxX: 10.0
    property real minY: 0
    property real maxY: 10.0
    property bool limitView: false // Limiting view with MinX & MaxX and ...
    property real velocityCoefficient: 0.91
    property int panMouseButton: Qt.MiddleButton

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

    // CLOSE program if any of these properties values has the same value.
    Component.onCompleted: {
        if (root.panMouseButton === root.selectPointsMouseButton ||
                root.selectPointsMouseButton === root.selectViewMouseButton ||
                root.panMouseButton === root.selectViewMouseButton)
        {
            console.log("[ERROR]: At GLChartFrame QML, panMouseButton, selectPointsMouseButton, selectViewMouseButton, can't be same button!.")
            Qt.exit(1)
        }
    }

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
            axisRange.minX: root.minX // Custom Prop
            axisRange.maxX: root.maxX // Custom Prop
            axisRange.minY: root.minY // Custom Prop
            axisRange.maxY: root.maxY // Custom Prop
            limitView: root.limitView // Custom Prop
            velocityCoefficient: root.velocityCoefficient
            panMouseButton: root.panMouseButton
        }
        // Rubber Band rectangle
        Rectangle {
            id: rectRubBand
            visible: mouseItemSelect.selecting | mouseItemView.selecting
            // Drawing Rectangle with scaled positions
            function rubberBand(mouse, startPos) {
                let x = Math.max(Math.min(mouse.x, startPos.x), leftMargin)
                let y = Math.max(Math.min(mouse.y, startPos.y), topMargin)
                let w = Math.abs(Math.max(x, mouse.x) - startPos.x)
                let h = Math.abs(Math.max(y, mouse.y) - startPos.y)
                rectRubBand.x = x
                rectRubBand.y = y
                rectRubBand.width = x + w >= canvas.width - rightMargin ? canvas.width - rightMargin - x : w
                rectRubBand.height = y + h >= canvas.height - bottomMargin ? canvas.height - bottomMargin - y : h
            }

        }

        // Single X Axis Zoom
        MouseArea {
            id: mouseWheelXAxis
            anchors.fill: canvas
            anchors.topMargin: glChartView.height + root.topMargin
            onWheel: (mouse) =>
                     {
                         glChartView.adjustView(glChartView.width / 2, 0, mouse.angleDelta.y, // x, y, angleDelta.y
                                                         true, false) // zoomX, zoomY
                     }
        }
        // Single Y Axis Zoom
        MouseArea {
            id: mouseWheelYAxis
            anchors.fill: canvas
            anchors.rightMargin: glChartView.width + root.rightMargin
            anchors.bottomMargin: root.bottomMargin
            onWheel: (mouse) =>
                     {
                         glChartView.adjustView(0, glChartView.height / 2, mouse.angleDelta.y, // x, y, angelDelta.y
                                                         false, true) // zoomX, zoomY
                     }
        }

        // View Selection RubberBand
        MouseArea {
            property point startPos
            property bool selecting: false
            id: mouseItemView
            anchors.fill: canvas
            acceptedButtons: root.selectViewMouseButton
            onPressed: (mouse) =>
                       {
                           // Disable Mouse Selecting
                           mouseItemSelect.enabled = false

                           startPos = Qt.point(mouse.x, mouse.y)
                           selecting = true
                           rectRubBand.x = startPos.x
                           rectRubBand.y = startPos.y
                           rectRubBand.width = 0
                           rectRubBand.height = 0

                           // Rect Style
                           rectRubBand.color = Qt.rgba(215/255, 10/255, 0, 0.05)
                           rectRubBand.border.color = "#d73800"
                           rectRubBand.border.width = 1
                       }

            onPositionChanged: {
                if (!selecting)
                    return
                rectRubBand.rubberBand(mouse, startPos)
            }
            onReleased: {
                selecting = false
                glChartView.updateRectView(Qt.rect(rectRubBand.x - leftMargin,
                                                   rectRubBand.y - topMargin,
                                                   rectRubBand.width,
                                                   rectRubBand.height))

                // Enable Mouse Selecting
                mouseItemSelect.enabled = true
            }

        }

        // Selection RubberBand
        MouseArea {
            property point startPos
            property bool selecting: false
            id: mouseItemSelect
            anchors.fill: canvas
            acceptedButtons: root.selectPointsMouseButton


            onPressed: (mouse) =>
                       {
                           // Disable Mouse View
                           mouseItemView.enabled = false

                           startPos = Qt.point(mouse.x, mouse.y)
                           selecting = true
                           rectRubBand.x = startPos.x
                           rectRubBand.y = startPos.y
                           rectRubBand.width = 0
                           rectRubBand.height = 0

                           // Rect Style
                           rectRubBand.color = Qt.rgba(0, 120/255, 215/255, 0.3)
                           rectRubBand.border.color = "#0078d7"
                           rectRubBand.border.width = 1

                           glChartView.preSelect(!(mouse.modifiers & Qt.ControlModifier))

                       }

            onPositionChanged: {
                if (!selecting)
                    return
                rectRubBand.rubberBand(mouse, startPos)
                glChartView.rangeSelecting(Qt.rect(rectRubBand.x - leftMargin,
                                                   rectRubBand.y - topMargin,
                                                   rectRubBand.width,
                                                   rectRubBand.height))

            }
            onReleased: {
                selecting = false

                if (!rectRubBand.width && !rectRubBand.height) {
                    rectRubBand.rubberBand(mouse, startPos)
                    glChartView.rangeSelecting(Qt.rect(rectRubBand.x - leftMargin - 2,
                                                       rectRubBand.y - topMargin - 2,
                                                       5, // Width
                                                       5)) // Height

                }


                glChartView.selectFinished()

                // Enable Mouse View
                mouseItemView.enabled = true
            }
        }
    }

    // Labels for major X
    Repeater {
        model: majorXCount + 1
        delegate: Text {
            x: root.leftMargin + index * ((root.width - root.leftMargin - root.rightMargin) / root.majorXCount)
            y: root.height - root.bottomMargin + 4
            color: root.labelColor
            font: root.labelFont
            text: (glChartView.projLeft + index * ((glChartView.projRight - glChartView.projLeft)
                                                   / root.majorXCount)).toFixed(decimalPlaces) + root.xLabelSuffix
        }
    }
    // Labels for major Y
    Repeater {
        model: majorYCount + 1
        delegate: Text {
            x: 0 + (root.leftMargin * 0.55)
            y: root.topMargin + index * ((root.height - root.topMargin - root.bottomMargin) / root.majorYCount) - 8
            color: root.labelColor
            font: root.labelFont
            text: (glChartView.projTop - index * ((glChartView.projTop - glChartView.projBottom)
                                                  / root.majorYCount)).toFixed(decimalPlaces) + root.yLabelSuffix
        }
    }

    // X Title
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height - 5
        color: root.titleColor
        font: root.titleFont
        text: root.xTitle
    }
    // Y Title
    Text {
        anchors.verticalCenter: parent.verticalCenter
        x: 0
        rotation: -90
        color: root.titleColor
        font: root.titleFont
        text: root.yTitle
    }

}
