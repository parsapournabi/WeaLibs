# Advanced C++/OpenGL/QML Chart

A high-performance, flexible charting librarry built with `C++`, `OpenGL`, `QML` and `Canvas`, supporting multiple series types, interactive features, and customizable visuals. The chart leverages `QQuickFramebufferObject` for rendering.
---

## File Tree
```bash
.
├── chart.frag
├── chart.vert
├── CMakeLists.txt
├── example
│   ├── CMakeLists.txt
│   ├── datasource.cpp
│   ├── datasource.h
│   ├── images
│   │   ├── targetPressed.png
│   │   └── upHovered.png
│   ├── main.cpp
│   ├── main.qml
│   └── resources.qrc
├── GLChartFrame.qml
├── GLItemLegend.qml
├── GLLegend.qml
├── glresources.qrc
├── properties
│   ├── IProperties.h
│   ├── PropertyAxisRange.h
│   ├── PropertyBackground.h
│   └── PropertySeries.h
├── README.md
├── scenes
│   ├── GLChartRenderer.cpp
│   ├── GLChartRenderer.h
│   ├── GLChartview.cpp
│   └── GLChartview.h
├── series
│   ├── GLAbstractSeries.h
│   ├── GLEnums.h
│   ├── GLSeriesHandle.h
│   ├── GLSeriesItem.h
│   └── GLSeriesStorage.h
├── tree.txt
└── utils
    ├── GLMathUtils.h
    └── GLStructures.h

```
---

## Table of Contents
1. [Requirements](#requirements)
2. [Features](#features)
3. [Series Types](#series-types)
4. [Custom Data Struct](#custom-data-struct)
5. [Color Options](#color-options)
6. [Interaction](#interaction)
7. [Grid and Background](#grid-and-background)
8. [Memory and Performance](#memory-and-performance)
9. [Configuration](#configuration)
10. [Warnings and Best Practices](#warnings-and-best-practices)
11. [Example](#example)
---

## Requirements
- C++: Minimum C++11
- OpenGL: Required version 4.6
- Qt: 5.15
- OS: Cross-platform (Linux, Windows)
- Dependencies: QtQuick, OpenGL, QCore
---

## Features
- Supported **three sereis types**:
Scatter, Line, Area.
- **Scatter series** supports marker shapes: Square, Circle, Texture(custom image).
- Marker size configurable per series (but only at scatter).
- **Pan, ZoomIn/ZoomOut, RubberBand Selection**.
- **Multi-selection** using `Ctrl` key.
- Configurable **MajorGridX/Y, MinorGrid, Background color** and other color stuff...
- Adjustable **Pan friction** (**velocityCoefficient**).
- Configurable **Mouse buttons** for Pan/Zoom/Select.
- Optimized performance with memory control.
---

## Series Types
### Scatter Series
- Shapes: `Square`, `Circle`, `Texture`.
- Marker size configurable.
- **Texture markers** require `imagePath`.
### Line Series
- Connect points in order, supports colors and thickness.
### Area Series
- Fill area under line with configurable color.
---

## Custom Data Structs

Series are implemented as **templates**:
```cpp
template <typename T>
class GLSeriesStorage {...};
```
- T must inherit from `PointXYBase`.
- `PointXYBase` contains:
  - `QVector2D` position.
  - [ChartColor](172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/Chart/utils/GLStructures.h/) color(rgba).

This design allows storing **custom data per point**, which can be emitted via signals upon **selection**.

**Note for users:**
- Make sure your custom struct/class is **Q_DECLARE_METATYPE** compatible for `QVector<T>` usage.
- Example:
```cpp
struct MyPoint : public PointXYBase {
    using PointXYBase::PointXYBase;
    int someField;
    QString name() const { return "MyPoint"; }
};
Q_DECLARE_METATYPE(MyPoint) // REQUIRED
Q_DECLARE_METATYPE(QVector<MyPoint>) // REQUIRED
```
---

## Color Options
Each series supports three color modes:
1. Mix of **point color** and **base color**.
2. Only **base color**.
3. Only **point color**.
---

>[!INFO]
> see [GLEnums.h](http://172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/Chart/series/GLEnums.h) for more details about types.

## Interaction
- Pan (optional), set `limitView`: **true** property on [GLChartFrame.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/Chart/GLChartFrame.qml).
- Zoom in/out.
- RubberBand selection.
- Multi-selection with `Ctrl`.
- Configurable **mouse buttons** for actions.
---

## Grid and Background
- MajorGridXY and MinorGrid customizable: count, color, visiblity
- Background color configurable.
---

## Memory and Performance
- Minimum chart memory: 12 * 6MB = 72MB  [see MAX_CHART_TOTAL_POINTS](http://172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/Chart/scenes/GLChartview.h)
- Configurable via internal variable.
- Performance depends on: 
        - Number of points per series.
        - Number of series.
        - Size of point struct
- Continous property updates (type, color, name) via timers **may degrade performance**.
---

## Configuration
- **Pan friction** adjustable. [velocityCoefficient](http://172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/Chart/GLChartFrame.qml)
- **Mouse buttons** for Pan/Zoom/Select configurable.
- **Marker size** adjustable per series.
- **Series color** mode adjustable per series.
---

## Warnings and Best Practices
1. **Texture markers** must have `imageIconUrl` defined.
2. Avoid **frequent timer-based property updates.**
3. Custom structs must inherit from `PointXYBase` and be **Q_DECLARE_METATYPE**.
4. Performance decreases with **large point structs**, may series, or many points.
---

## Example
Import all files and directories instead **example** directory to your project.
### Quick Start
Drawing a simple filled triangle:
1. main.cpp:
```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QDebug>
#include <QThread>

#include "GLChartview.h"
#include "GLSeriesStorage.h"

int main(int argc, char *argv[])
{

    // SurfaceFormat configuring to OpenGL 4.6 core
    QSurfaceFormat fmt;
    fmt.setMajorVersion(4);
    fmt.setMinorVersion(6);
    fmt.setDepthBufferSize(24);
    fmt.setSwapInterval(1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    QSurfaceFormat::setDefaultFormat(fmt);
    QSurfaceFormat curFmt = QSurfaceFormat::defaultFormat();

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    // Register MetaTypes of chart
    GLChartView::registerMetaTypes();

    // Creating series
    GLSeriesStorage<PointXYBase> mySeries;

    // Drawing a simple triangle with different point color
    mySeries.append(PointXYBase{50.0f, 50.0f, 255, 0, 0, 1.0});
    mySeries.append(PointXYBase{40.0f, 25.0f, 0, 255, 0, 1.0});
    mySeries.append(PointXYBase{60.0f, 25.0f, 0, 0, 255, 1.0});
    mySeries.append(PointXYBase{50.0f, 50.0f, 255, 0, 0, 1.0});

    engine.rootContext()->setContextProperty("mySeriesPtr", &mySeries);

    engine.load(url);
    return app.exec();

}

```
2- main.qml
```qml
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick 2.15
import QtQuick.Dialogs 1.3

import GLItems 1.0

Window {
    id: root
    title: "Chart Window"
    width: 500
    height: 500
    visible: true
    GLChartFrame {
        anchors.fill: parent
        anchors.margins: 10
        leftMargin: 80
        minX: 0.0
        maxX: 100.0
        minY: 0.0
        maxY: 100.0
        GLSeriesItem {
            id: mySeries
            series: mySeriesPtr
            type: GL.SeriesTypeArea

        }
    }
}
```

### Best Practice
[See example usage](http://172.16.50.13/parsa/qcustommodels/-/tree/parsa-chart/Chart/example)

## Additional Notes
- Highly **customizable** and flexible charting system.
- Supports **real-time updates**, but excessive property changes reduce performance.
- Designed for **professional applications** requiring high-performance rendering.

