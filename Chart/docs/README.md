# Source code Description

## Subjects

The **WeaChart** component is divded into four main areas: 

- [UI/Styles](#ui/styles)
- [OpenGL and Renderer](#opengl-and-renderer)
- [Backend](#backend)
- [CMake library Configs](#cmake-library-configs)
---

## UI/Styles
All of the components UIs are written in **QML**,
except for the main chart scene, which is implemented using **shaders**.
The grids and axes are drawn using the **canvas**.

### QML
The main component is **GLChartFrame**.
Users only need to use this component to load the chart.
An optional component is **GLLegend**; NOTE that 
**GLItemLegend** should be avoided as a public component.

### GLChartFrame
- As you can see `GLChartFrame`, everything is user-defined,
so we need many properties, as listed below.
```qml

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
property int xTitlePosY: root.height - 5
property int yTitlePosX: 0
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

```

- The next step is drawing the chart's background structures,
such as grids and axes. [view canvas code](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLChartFrame.qml#L75-L149)

- Drawing labels & numerical factors is much easier with Repeater instead canvas.
Here is the code:
```qml
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
```

- Now, we need to connect the property-changed signals (used in canvas Paint)
to the **canvas.requestPaint()**.
```qml
onUseGridChanged: canvas.requestPaint()
onMajorXCountChanged: canvas.requestPaint()
onMajorYCountChanged: canvas.requestPaint()
onMinorPerMajorChanged: canvas.requestPaint()
onMajorColorChanged: canvas.requestPaint()
onMinorColorChanged: canvas.requestPaint()
onBackgroundColorChanged: canvas.requestPaint()
onBackgroundOpacityChanged: canvas.requestPaint()
```

- It is better to ensure that the properties with `mouseButton` types
do not have the same values as each other.

```qml
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
```

- Let's place the `GLChartView` component in the canvas chart scene area.
Then we assign ChartProperties to itself.
```qml
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
```

### GLItemLegend
A single legendItem which drawn using the **Canvas**,
and the shape of its marker matches with **SeriesType(scatter, line, area)**.

[view GLItemLegend for more detail](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLItemLegend.qml)

### GLLegend
The main legend component is represented with **Repeater**,
which automatically adds all series into a single legend item.
The idea behind this item is to provide a simple component
that helps implement the visual representation of the legend,
simply by adding it to the **GLChartFrame** component.

[view GLLegend for more detail](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLLegend.qml)

### QML Files
- [GLChartFrame.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLChartFrame.qml)
- [GLItemLegend.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLItemLegend.qml)
- [GLLegend.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLLegend.qml)


### Events
First of all, let's handle the zoom and pan events.
For this, we need to go back to the `GLChartView` class,
because it contains those events. Since no drawing is requried,
it is much easier to handle them in the **FBO(GLChartView** directly.

#### UI events include:
- [Zoom Event](#zoom-event)
- [Pan Event](#pan-event)
- [Select Event](#select-event)
- [Multiselect Event](#multiselect-event)
- [Crop Event](#crop-event)

#### Zoom Event
***Arguments:***
1. mxPixel: float => Mouse x position unit by pixel.
2. myPixel: float => Mouse y position unit by pixel.
3. angleDeltaY: float => Mouse angleDeltaY value unit by pixel.
4. zoomX: bool => If its false, means xAxis zoom will be ignore.
5. zoomY: bool => If its false, means yAxis zoom will be ignore.

```cpp
void GLChartView::adjustView(float mxPixel, float myPixel, float angleDeltaY,
                             bool zoomX, bool zoomY)
{
    constexpr float zoomFactor = 0.15f;
    float factor = (angleDeltaY > 0) ? (1 - zoomFactor) : (1 + zoomFactor);
    float minX = m_proj.left;
    float maxX = m_proj.right;
    float minY = m_proj.bottom;
    float maxY = m_proj.top;

    float mx = normXtoWorld(mxPixel);
    float my = normYtoWorld(myPixel);

    float newMinX = mx - (mx - minX) * factor;
    float newMaxX = mx + (maxX - mx) * factor;
    float newMinY = my - (my - minY) * factor;
    float newMaxY = my + (maxY - my) * factor;

    if (m_limitView &&
        (newMinX < m_axisRange->minX() ||
         newMaxX > m_axisRange->maxX() ||
         newMinY < m_axisRange->minY() ||
         newMaxY > m_axisRange->maxY()))
    {
        updateAxisRange();
        return;
    }

    if (zoomX) {
        m_proj.left = newMinX;
        m_proj.right = newMaxX;
    }
    if (zoomY) {
        m_proj.bottom = newMinY;
        m_proj.top = newMaxY;
    }

    emit updateQml();

    update();
}
```

>[!INFO]
> If you're wondering when `m_proj` comes from, refer [OpenGL and Renderer](#opengl-and-renderer)

#### PAN Event
***class Members:***
1. bool m_panning = false; => This field will be set true when mouse is **pressed** and its true until the mouse **released**.
2. QPointF m_panVelocity; => Current panning velocity to having smooth panning release.
3. QPointF m_lastMousePos; => Required for pan calculation.
4. QElapsedTimer m_panElaps; => To decrease `m_panVelocity` automatically, this field is
required to calculate how much value `m_panVelocity`, should minus.
5. int m_panTimerId = -1; => TimerId required, because we are using QObject `timerEvent` method.

***class Properties:***
1. bool m_limitView = false; => If its true the panning should be disabled.
2. qreal velocityCoefficient = 0.91; => Constant factor which multiple with `m_panVelocity`. (0 means no velocity)
3. Qt::MouseButton panMouseButton = Qt::MiddleButton; => Recognize which button is for panning process and which buttons are not.

**Starting Pan**

```cpp
void GLChartView::mousePressEvent(QMouseEvent *event)
{
    // PAN Process
    if (event->button() == panMouseButton && !m_limitView) {
        m_panning = true;
        m_lastMousePos = event->pos();
        m_panVelocity = QPointF(0, 0);
        // If auto pan after released wasn't finish yet, kill that process.
        if (m_panTimerId != -1) {
            killTimer(m_panTimerId);
            m_panTimerId = -1;
        }
    }
}
```

**Panning...**

```cpp
void GLChartView::mouseMoveEvent(QMouseEvent *event)
{
    // PAN Process
    if (m_panning) {
        float lastX = normXtoWorld(m_lastMousePos.x());
        float lastY = normYtoWorld(m_lastMousePos.y());
        float curX = normXtoWorld(event->pos().x());
        float curY = normYtoWorld(event->pos().y());

        qreal dt = m_panElaps.restart() / 1000.0;
        QPointF delta((curX - lastX), (curY - lastY));
        m_lastMousePos = event->pos();

        m_proj.left -= delta.x();
        m_proj.right -= delta.x();
        m_proj.bottom -= delta.y();
        m_proj.top -= delta.y();

        m_panVelocity = delta / dt;

        emit updateQml(); // updating labels number values.

        update();
    }

}
```

**Releasing Pan's MouseButton**

```cpp
void GLChartView::mouseReleaseEvent(QMouseEvent *event)
{
    // PAN Process
    if (event->button() == panMouseButton && !m_limitView) {
        m_panning = false;
        m_panElaps.restart();
        if (m_panTimerId == -1) {
            m_panTimerId = startTimer(16); // Start PAN deceleration ramp timer.
        }
    }

}
```


**Pan Ramp**

```cpp
void GLChartView::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_panTimerId)
        panAcceleration();

}

void GLChartView::panAcceleration() {

//    constexpr double friction = 0.91; // Velocity Friction.
    qreal dt = m_panElaps.restart() / 1000.0;

    m_proj.left -= m_panVelocity.x() * dt;
    m_proj.right -= m_panVelocity.x() * dt;
    m_proj.bottom -= m_panVelocity.y() * dt;
    m_proj.top -= m_panVelocity.y() * dt;

    m_panVelocity *= velocityCoefficient;

    if (qAbs(m_panVelocity.x()) < 0.03 && qAbs(m_panVelocity.y() < 0.03)) {
        killTimer(m_panTimerId);
        m_panTimerId = -1;
    }

    emit updateQml();
    update();

}
```

---
Back to the QML events.

#### Single XY Axes Zoom Event
The reason i included this in QML is that the
event must be called in the **canvas** area,
not in the **GLChartView** area.

```qml
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
```

#### Select Event
Before describing the select event, we need to
draw a rubber recantgle for visulization. 
This rectangle will also be used in the **Crop Event**.

```qml
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
```

and now here is the **Select Event**:

```qml

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
```

#### Multiselect Event
It's simple, just you need to pass the `Qt.ControlModifier` to the `glChartView.preSelect` method.


#### Crop Event
Same as the **Select Event**, just need to create a visual rubberBand and passing
the `topLeft`, `bottomRight` fields to **glChartView**.

```qml
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
```

---

## OpenGL and Renderer
To keep my explanation simple, I won't go into the
details of the FBO and backgrounds.
I'll only cover important parts and content.
Keep in mind that in **GLChartView(FBO)**,
I only store the properties that need to be synchronized
with the **GLChartRenderer**. So, there are no calculations
in the FBO except for the **Projection(m_proj)**.

### Projection
m_proj is an attribute with four variables:
`top`, `left`, `bottom`, `right`.
These fields are initialized with the default values provided by
the `axisRange` property.
Note that the projection works like a camera,
allowing us to change the view of objects instead of moving all the points.
This means that with each **PAN** and **Zoom** movement,
the `m_proj` variables must be updated.

### Renderer
A basic renderer with a single **shaderProgram**,
**VAO** and **VBO**.
The VBO accepts a `vec2` for position and a `vec4` for the color attribute.
I pass the `seletedRanges` positions as a uniform array, and in the shader,
I check each point to see if it is within the `selectedRanges`.
If it is, the point's color is set to yellow.

There is also a **SSBO** in the shaderProgram.
It is an array of struct(each struct stores series properties and the start/end index in the **VBO** [see SeriesProps struct](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/utils/GLStructures.h#L24))
By passing this **SSBO**, I can easily determine which vertex belongs to which series.
Knowing this, I can assign the style of that vertex, such as `SeriesType`, `BaseColor` and so on.

### Shaders
I recommend to read the [chart.vert & chart.frag](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/shaders/) files,
as there is nothing confusing in their content.

---

## Backend

To develop a flexible and standard chart backend, we
need to provide three main components:
1. Independent series.
2. A series read/write handler.
3. A main scene that combines series data 
and properties into a single raw dataset.

The key idea is designing a series class with a templated
Point structure.

**Why a templated point?**
Normally, we only need x, y, and color for each point,
so why use a template?

The answer is that you may have additional data for each 
point thtat is not directly related to the chart scene,
but you may want to use those fields when a point is **selected**.
That is the reason we need a templated Point.

### Series Backend

#### GLAbstractSeries & GLSeriesStorage development

You should know that it isn't possible to create a class with
both a **template** and **Q_OBJECT** at the same time (so
we can't use signals for the selection process directly in 
the templated class).

To support both, I created an abstract series class (`GLAbstractSeries`)
that contains virtual methods and a `makeSelectVariant` method.
This method should be called from the main scene's (`GLChartView`) selection process, 
where we emit `selected` signal.
[see GLAbstractSeries](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/series/GLAbstractSeries.h)

After that, I wrote a templated class that inherits from the abstract class.
In this class(`GLSeriesStorage`), we override the virtual methods and implement the point
appending process, the `makeSelectVariant` method, and so on.
[see GLSeriesStorage](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/series/GLSeriesStorage.h)

**Note** that this templated series class works like a `QVector<T>`, **T** must
inherit from `PointXYBase`.

Up to this point, I haven't explained how and where `select` signal is triggered.
I've only described the series structure and the prerequisites for the `selectProcess`.

#### GLChartView & GLSeriesHandler

As you know, there is no direct way to add a `GLSeriesStorage<T>` to the 
`GLChartView` series vector.
The solution is simple: C++ allows passing child classes as their parent class type.
For example, the argument of the `addSeries` method can be of type `GLAbstractSeries` instead of `GLSeriesStorage`.

To read/write the `GLSeriesStorage` data and emit `selected` signal from `GLChartView`,
a handler class is needed([GLSeriesHandler](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/series/GLSeriesHandler.h)).
This class provides a getter(which returns the `GLSeriesStorage`(named as view())) and `emitselected` function,
inside of which the `selected` signal of `GLSeriesStorage` is emitted.

**Note:** By using the **GLSeriesHandler** inside of the **GLChartView**,
you can probably access to all of the **GLAbstractSeries** public slots.
