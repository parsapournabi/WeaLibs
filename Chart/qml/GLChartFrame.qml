import QtQuick 2.12
import QtQuick.Controls 2.12
import com.wearily.WeaChart 1.0

Item {
    id: root
    property bool useGrid: true
    property bool movableGrid: false
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
    property int xTitlePosY: root.height - 5
    property int yTitlePosX: 0
    property font titleFont: Qt.font({ pixelSize: 13, bold: true})
    property color titleColor: "black"
    property font labelFont: Qt.font({ pixelSize: 11})
    property color labelColor: "black"
    property string xLabelSuffix: "" // For example xLabelSuffix: "$", then value from 100.0 goes to 100.0$
    property string yLabelSuffix: "" // Same as xLabelSuffix
    property int xLabelDecimalPlaces: 2
    property int yLabelDecimalPlaces: 2
    property real xLabelScaleFactor: 1.0 // Monitor Scaling for example minX = 0.0 & maxX = 1.0, by giving this property to 100.0, the UI minX & maxX will be in range of the 0.0~100.0.
    property real yLabelScaleFactor: 1.0 // Monitor Scaling for example minY = 0.0 & maxY = 1.0, by giving this property to 100.0, the UI minX & maxX will be in range of the 0.0~100.0.

    // Anchors Properties
    property int leftMargin: 40
    property int rightMargin: 20
    property int bottomMargin: 30
    property int topMargin: 20

    // Mouse Buttons
    property int selectPointsMouseButton: Qt.LeftButton // See mouseItemSelect id for more detail
    property int selectViewMouseButton: Qt.RightButton // See mouseItemView id for more detail

    // ReadOnly
    readonly property alias chart: glChartView // Also uses on GLLegend chart property

    // Chart Properties
    property real minX: 0
    property real maxX: 10.0
    property real minY: 0
    property real maxY: 10.0
    property bool limitView: false // Limiting view with MinX & MaxX and ...
    property real velocityCoefficient: 0.91
    property int panMouseButton: Qt.MiddleButton
    property int autoScalePolicy: GL.PolicyNone
    property bool fitWindow: false
    property bool debug: false


    // Crosshair ToolTip
    property bool toolTipEnable: false
    property color toolTipAxisColor: "white"
    property color toolTipCenterDotColor: "red"
    property color toolTipBoxColor: "black"
    property color toolTipBoxBorderColor: "white"
    property int toolTipBoxRadius: 4
    property color toolTipTextColor: "white"
    property string toolTipTextSplitter: "\n" // Splitter means the character of between "X:" and "Y:"
    property font toolTipFont: Qt.font({pixelSize: 12})

    // Select RubberBand & Crop Rectangle Style
    property color selectBgRectColor: Qt.rgba(0, 120/255, 215/255, 0.3)
    property color selectBorderRectColor: "#0078d7"
    property color cropBgRectColor: Qt.rgba(215/255, 10/255, 0, 0.05)
    property color cropBorderRectColor: "#d73800"

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
    function pixelToWorld(pixel, is_x = true, is_glparent = false) {
        // No need to scaling pixel to glChartView XY.
        if (is_glparent) {
            if (is_x) return glChartView.normXtoWorld(pixel)
            else return glChartView.normYtoWorld(pixel)
        }
        // Scaling to glChartView XY
        else {
            if (is_x) {
                // Normelized
                pixel = (pixel - root.leftMargin) / ((canvas.width - root.rightMargin) - root.leftMargin)
                // Scaled
                pixel = pixel * glChartView.width
                return glChartView.normXtoWorld(pixel)
            }
            else {
                // Normelized
                pixel = (pixel - root.topMargin) / ((canvas.height - root.bottomMargin) - root.topMargin)
                // Scaled
                pixel = pixel * glChartView.height
                return glChartView.normYtoWorld(pixel)
            }
        }
    }
    function clamp(value, min, max) {
        return Math.min(Math.max(value, min), max)
    }
    function niceNumber(x) {
        var exp = Math.floor(Math.log10(x))
        var f = x / Math.pow(10, exp)
        var niceF

        if (f < 1.5)
            niceF = 1
        else if (f < 3)
            niceF = 2
        else if (f < 7)
            niceF = 5
        else
            niceF = 10
        return niceF * Math.pow(10, exp)

    }

    Canvas {

        property real mouseX: -1
        property real mouseY: -1
        property bool mouseInside: false

        property real plotX: root.leftMargin
        property real plotY: root.topMargin
        property real plotW: width - root.leftMargin - root.rightMargin
        property real plotH: height - root.topMargin - root.bottomMargin

        id: canvas
        anchors.fill: parent
        focus: true

        function drawMovingGrids(ctx, viewWidth, viewHeight) {
            // X grids
            modelMajorX.clear();
            if (root.majorXCount > 0)
            {
                // Calculating spacing & first/end point
                let spacingX = niceNumber(viewWidth / root.majorXCount);
                let startX = Math.floor(glChartView.projLeft / spacingX) * spacingX;
                let endX = Math.ceil(glChartView.projRight / spacingX) * spacingX;
                // Draw X Major
                for (let x = startX; x <= endX; x += spacingX) {
                    let screenX = (x - glChartView.projLeft) / (glChartView.projRight - glChartView.projLeft) * (plotW) + plotX;
                    screenX = clamp(screenX, plotX, plotX + plotW);
                    ctx.beginPath();
                    ctx.moveTo(screenX, plotY);
                    ctx.lineTo(screenX, plotY + plotH);
                    ctx.stroke();

                    // Set Style to MinorColor
                    ctx.strokeStyle = root.minorColor;
                    // Draw X Minor
                    for (let m = 1; m < root.minorPerMajor; ++m) {
                        let xMinor = screenX + m * (plotW / root.majorXCount / root.minorPerMajor);
                        xMinor = clamp(xMinor, plotX, plotX + plotW);
                        ctx.beginPath();
                        ctx.moveTo(xMinor, plotY);
                        ctx.lineTo(xMinor, plotY + plotH);
                        ctx.stroke();
                    }
                    // Reset Style to MajorColor
                    ctx.strokeStyle = root.majorColor;

                    // Adding to the ListModel for labels
                    if (x == startX || x == endX) continue;
                    modelMajorX.append({ screenX });
                }
            }
            // Y grids
            modelMajorY.clear();
            if (root.majorYCount > 0)
            {
                // Calculating spacing & first/end point
                let spacingY = niceNumber(viewHeight / root.majorYCount);
                let startY = Math.floor(glChartView.projBottom / spacingY) * spacingY;
                let endY = Math.ceil(glChartView.projTop / spacingY) * spacingY;
                // Draw Y Major
                for (let y = startY; y <= endY; y += spacingY) {
                    let screenY = plotH - (y - glChartView.projBottom) / (glChartView.projTop - glChartView.projBottom) * (plotH) + plotY;
                    screenY = clamp(screenY, plotY, plotY + plotH);
                    ctx.beginPath();
                    ctx.moveTo(plotX, screenY);
                    ctx.lineTo(plotX + plotW, screenY);
                    ctx.stroke();

                    // Set Style to MinorColor
                    ctx.strokeStyle = root.minorColor;
                    // Draw Y Minor
                    for (let n = 1; n < root.minorPerMajor; ++n) {
                        let yMinor = screenY + n * (plotH / root.majorYCount / root.minorPerMajor);
                        yMinor = clamp(yMinor, plotY, plotY + plotH);
                        ctx.beginPath();
                        ctx.moveTo(plotX, yMinor);
                        ctx.lineTo(plotX + plotW, yMinor);
                        ctx.stroke();
                    }
                    // Reset Style to MajorColor
                    ctx.strokeStyle = root.majorColor;

                    if (y == startY || y == endY) continue;
                    modelMajorY.insert(0, { screenY });
                }
            }
        }
        function drawFixedGrids(ctx, viewWidth, viewHeight) {
            // X Grids
            for (let i = 0; i <= root.majorXCount; ++i) {
                let x_px = plotX + i * (plotW / root.majorXCount);
                ctx.beginPath();
                ctx.moveTo(x_px, plotY);
                ctx.lineTo(x_px, plotY + plotH);
                ctx.stroke();

                // Drawing X Minor
                if (i >= root.majorYCount) continue;
                // Set Style to MinorColor
                ctx.strokeStyle = root.minorColor;
                for (let m = 1; m < root.minorPerMajor; ++m) {
                    let xMinor = x_px + m * (plotW / root.majorXCount / root.minorPerMajor);
                    ctx.beginPath();
                    ctx.moveTo(xMinor, plotY);
                    ctx.lineTo(xMinor, plotY + plotH);
                    ctx.stroke();
                }
                // Reset Style to MajorColor
                ctx.strokeStyle = root.majorColor;
            }
            // Y Grids
            for (let l = 0; l <= root.majorYCount; ++l) {
                let y_px = plotY + l * (plotH / root.majorYCount);
                ctx.beginPath();
                ctx.moveTo(plotX, y_px);
                ctx.lineTo(plotX + plotW, y_px);
                ctx.stroke();

                // Drawing Y Minor
                if (l >= root.majorYCount) continue;
                // Set Style to MinorColor
                ctx.strokeStyle = root.minorColor;
                for (let n = 1; n < root.minorPerMajor; ++n) {
                    let yMinor = y_px + n * (plotH / root.majorYCount / root.minorPerMajor);
                    ctx.beginPath();
                    ctx.moveTo(plotX, yMinor);
                    ctx.lineTo(plotX + plotW, yMinor);
                    ctx.stroke();
                }
                // Reset Style to MajorColor
                ctx.strokeStyle = root.majorColor;
            }
        }

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            ctx.fillStyle = Qt.rgba(backgroundColor.r,
                                    backgroundColor.g,
                                    backgroundColor.b,
                                    backgroundOpacity);
            ctx.fillRect(plotX, plotY, plotW, plotH);

            ctx.lineWidth = 1;

            let viewWidth = glChartView.projRight - glChartView.projLeft;
            let viewHeight= glChartView.projTop - glChartView.projBottom;

            // Drawing Grids
            if (useGrid) {
                ctx.strokeStyle = root.majorColor;
                // Moving Grids
                if (movableGrid) drawMovingGrids(ctx, viewWidth, viewHeight)
                // Fixed Grids
                else drawFixedGrids(ctx, viewWidth, viewHeight)
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

            // Drawing Crosshair If ToolTip is enable & cursor pos is range of the glChartView width&height
            if (root.toolTipEnable && mouseInside)
            {
                // Drawing ToolTip Axis
                ctx.beginPath();
                ctx.strokeStyle = root.toolTipAxisColor;
                ctx.lineWidth = 1;

                // Horizontal line
                ctx.moveTo(root.leftMargin, mouseY + 0.5);
                ctx.lineTo(width - root.rightMargin, mouseY + 0.5);

                // Vertical line
                ctx.moveTo(mouseX + 0.5, root.topMargin);
                ctx.lineTo(mouseX + 0.5, height - root.bottomMargin);

                ctx.stroke();
                ctx.closePath();

                // Center dot
                ctx.beginPath();
                ctx.fillStyle = root.toolTipCenterDotColor;
                ctx.arc(mouseX, mouseY, 2, 0, Math.PI * 2);
                ctx.fill();
                ctx.closePath();
            }

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
            autoScalePolicy: root.autoScalePolicy
            fitWindow: root.fitWindow
            debug: root.debug
            onUpdateQml: {
                if (root.useGrid && root.movableGrid) canvas.requestPaint()
            }
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
                         let mxPixel = clamp(mouse.x + root.rightMargin - root.leftMargin,
                                             0.0, glChartView.width)
                         glChartView.adjustView(mxPixel, 0, mouse.angleDelta.y, // x, y, angleDelta.y
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
                         let myPixel = clamp(mouse.y + root.bottomMargin - root.topMargin,
                                             0.0, glChartView.height)
                         glChartView.adjustView(0, myPixel, mouse.angleDelta.y, // x, y, angelDelta.y
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
                           rectRubBand.color = root.cropBgRectColor
                           rectRubBand.border.color = root.cropBorderRectColor
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

        // Crosshair(toolTip) & Selection RubberBand
        MouseArea {
            property point startPos
            property bool selecting: false
            id: mouseItemSelect
            anchors.fill: canvas
            acceptedButtons: root.selectPointsMouseButton
            hoverEnabled: true
            cursorShape: canvas.mouseInside && root.toolTipEnable ? Qt.CrossCursor : Qt.ArrowCursor

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
                           rectRubBand.color = root.selectBgRectColor
                           rectRubBand.border.color = root.selectBorderRectColor
                           rectRubBand.border.width = 1

                           glChartView.preSelect(!(mouse.modifiers & Qt.ControlModifier))

                       }

            onPositionChanged: {
                if (!selecting)
                {
                    if (!root.toolTipEnable) return;
                    // Check if in range of the GLChartView
                    if (mouse.x >= root.leftMargin &&
                            mouse.x <= canvas.width - root.rightMargin &&
                            mouse.y >= root.topMargin &&
                            mouse.y <= canvas.height - root.bottomMargin)
                    {
                        canvas.mouseInside = true
                        canvas.mouseX = mouse.x
                        canvas.mouseY = mouse.y
                    }
                    else
                    {
                        canvas.mouseInside = false
                    }
                    canvas.requestPaint()
                    return;
                }
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
        // Crosshair Coordinations
        Rectangle {
            id: coordBox
            visible: canvas.mouseInside && root.toolTipEnable
            color: root.toolTipBoxColor
            radius: root.toolTipBoxRadius
            border.color: root.toolTipBoxColor
            border.width: 1
            Text {
                id: coordTxt
                text: "X: " +
                      pixelToWorld(canvas.mouseX).toFixed(xLabelDecimalPlaces) + xLabelSuffix +
                      root.toolTipTextSplitter +
                      "Y: " +
                      pixelToWorld(canvas.mouseY, false).toFixed(yLabelDecimalPlaces) + yLabelSuffix
                font: root.toolTipFont
                color: root.toolTipTextColor
                padding: 6
            }
            x:
            {
                var preferred = canvas.mouseX + 12;
                if (preferred + width > canvas.width - root.rightMargin)
                    preferred = canvas.mouseX - 12 - width;
                if (preferred < 2 + root.leftMargin) preferred = 2;
                return preferred
            }
            y:
            {
                var preferred = canvas.mouseY - height - 12;
                if (preferred < 2 + root.topMargin) preferred = canvas.mouseY - height + 12 + height;
                if (preferred + height > canvas.height - root.bottomMargin) preferred = 2
                return preferred
            }
            implicitWidth: coordTxt.paintedWidth + 12
            implicitHeight: coordTxt.paintedHeight + 8
        }
    }
    ListModel {
        id: modelMajorX
    }
    ListModel {
        id: modelMajorY
    }

    // Labels for major X
    Repeater {
        //        model: majorXCount + 1
        model: useGrid && movableGrid ? modelMajorX : majorXCount + 1
        delegate: Text {
            function getX() {
                return useGrid && movableGrid ?
                            modelData :
                            root.leftMargin + index *
                            ((root.width - root.leftMargin - root.rightMargin) /
                             root.majorXCount);
            }
            x: getX()
            y: root.height - root.bottomMargin + 4
            color: root.labelColor
            font: root.labelFont
            text: (root.xLabelScaleFactor * (glChartView.projLeft + index * ((glChartView.projRight - glChartView.projLeft)
                                                                             / root.majorXCount))).toFixed(xLabelDecimalPlaces) + root.xLabelSuffix
        }
    }
    // Labels for major Y
    Repeater {
        model: useGrid && movableGrid ? modelMajorY : majorYCount + 1
        delegate: Text {
            function getY() {
                return useGrid && movableGrid ?
                            modelData :
                            root.topMargin + index *
                            ((root.height - root.topMargin - root.bottomMargin) /
                             root.majorYCount) - 8;
            }

            x: 0 + (root.leftMargin * 0.55)
            y: getY()
            color: root.labelColor
            font: root.labelFont
            text: (root.yLabelScaleFactor * (glChartView.projTop - index * ((glChartView.projTop - glChartView.projBottom)
                                                                            / root.majorYCount))).toFixed(yLabelDecimalPlaces) + root.yLabelSuffix
        }
    }

    // X Title
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: root.xTitlePosY
        color: root.titleColor
        font: root.titleFont
        text: root.xTitle
    }
    // Y Title
    Text {
        anchors.verticalCenter: parent.verticalCenter
        x: root.yTitlePosX
        rotation: -90
        horizontalAlignment: Qt.AlignLeft
        verticalAlignment: Qt.AlignBottom
        color: root.titleColor
        font: root.titleFont
        text: root.yTitle
    }
}
