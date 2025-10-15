# Advanced C++/OpenGL/QML Chart

A high-performance, flexible charting librarry built with `C++`, `OpenGL`, `QML` and `Canvas`, supporting multiple series types, interactive features, and customizable visuals. The chart leverages `QQuickFramebufferObject` for rendering.
---

## File Tree
```bash
.
├── cmake
│   ├── config.h.in
│   ├── WeaChartConfig.cmake.in
│   └── WeaChartUninstall.cmake.in
├── CMakeLists.txt
├── docs
│   ├── mainpage.h
│   └── README.md
├── example
│   ├── CMakeLists.txt
│   ├── datasource.cpp
│   ├── datasource.h
│   ├── images
│   │   ├── targetPressed.png
│   │   └── upHovered.png
│   ├── main.cpp
│   ├── main.qml
│   └── resources.qrc
├── include
│   └── WeaChart
│       ├── config.h
│       ├── GLChart
│       ├── properties
│       │   ├── IProperties.h
│       │   ├── PropertyAxisRange.h
│       │   ├── PropertyBackground.h
│       │   └── PropertySeries.h
│       ├── scenes
│       │   ├── GLChartRenderer.h
│       │   └── GLChartview.h
│       ├── series
│       │   ├── GLAbstractSeries.h
│       │   ├── GLEnums.h
│       │   ├── GLSeriesHandle.h
│       │   ├── GLSeriesItem.h
│       │   └── GLSeriesStorage.h
│       ├── utils
│       │   ├── GLMathUtils.h
│       │   └── GLStructures.h
│       └── WeaChart_export.h
├── install.sh
├── qml
│   ├── GLChartFrame.qml
│   ├── GLItemLegend.qml
│   ├── GLLegend.qml
│   ├── plugins.qmltypes
│   └── qmldir
├── README.md
├── resources.qrc
├── shaders
│   ├── chart.frag
│   └── chart.vert
├── src
│   └── scenes
│       ├── GLChartRenderer.cpp
│       └── GLChartview.cpp
├── tree.txt
├── uninstall.sh
└── WeaChart.qdocconf

```
---

## Table of Contents
1. [Requirements](#requirements)
2. [Build and Installation](#build-and-installation)
3. [Usage and Importing](#usage-and-importing)
4. [Features](#features)
5. [Series Types](#series-types)
6. [Custom Data Struct](#custom-data-struct)
7. [Color Options](#color-options)
8. [Interaction](#interaction)
9. [Grid and Background](#grid-and-background)
10. [Memory and Performance](#memory-and-performance)
11. [Configuration](#configuration)
12. [Warnings and Best Practices](#warnings-and-best-practices)
13. [Example](#example)
14. [Additional Notes](#additional-notes)
15. [Information](#information)
---

## Requirements
- C++: Minimum C++11 (Recommended C++17)
- OpenGL: Required version 4.6
- Qt: 5.15
- GCC: 7.5.0 or later
- CMake: 3.14 or later
- OS: Cross-platform (Linux, Windows)
- Dependencies: QtQuick, OpenGL, QCore
---
## Build and Installation
This project uses **CMake** as its build system and is cross-platform.

### 1. Clone the Repository
```bash
git clone http://172.16.50.13/parsa/qcustommodels.git
cd qcustommodels/Chart
```

### 2. Build Instruction
#### Linux
Make sure you have **cmake** and a **C++ compiler (gcc/g++)**

```bash
$ ~/qcustommodels/Chart: chmod +x install.sh
$ ~/qcustommodels/Chart: sudo ./install.sh
```

>[!INFO]
> Usually the `INSTALL_PREFIX` on linux is: `/usr/local/...`

>[!INFO]
> You can pass custom `INSTALL_PREFIX` like below.

```bash
$ ~/qcustommodels/Chart: sudo ./install.sh /my/custom/path
```

#### Windows
Make sure you have **CMake** and **(MinGW or MSVC)**  compiler installed.
Run `cmd` as Adminstrator, then do like below:

```bash
cd qcustommodels/Chart
mkdir BUILD
cd BUILD
cmake .. -DBUILD_EXAMPLE=OFF -G "MinGW Makefiles" # or another generator if needed
cmake --build . --config Release
cmake --install .
```

>[!INFO]
> Usually the `CMAKE_INSTALL_PREFIX` on windows is: `C:/Program Files/WeaChart/`

>[!INFO]
> You can pass custom `CMAKE_INSTALL_PREFIX` like below.

```bash
...
cmake .. -DBUILD_EXAMPLE=OFF -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX="C:/My/Custom/Path/WeaChart"
...
```
---

## Usage and Importing
After installing the library follow these sequence:
- Add **Quick**, **Qml**, **OpenGL** components.
- Config C++ as 17 version.
- Assign **QML_IMPORT_PATH** with [WeaChart_QML_IMPORT_PATH](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/config.h#L3) (it's optional, this will help **IDE** to shows Component properties and completetion.)
- Implement **WeaChart** lib and includes.

>[!INFO]
> If after assigning **QML_IMPORT_PATH** Components were still **Unknown**, restart Qt Creator application and then it works :)

### qmake
Here is an example of usage **WeaChart** on **.pro** application:

```qmake
QT += quick core opengl
CONFIG += c++17

...

# The path will comes from INSTALL_PREFIX at installation stage.
# You can print "WeaChart_QML_IMPORT_PATH" constant on main.cpp to see which path is on your system.
QML_IMPORT_PATH = /usr/local/share/qml/ # Change it with your installation path.
... 

unix:!macx: LIBS += -L/usr/local/lib64 -lWeaChart # Change it with your installation path.
unix:!macx: INCLUDEPATH += /usr/local/include # Change it with your installation path.

```

### CMake
Here is an example of usage **WeaChart** on **CMakeLists.txt**.

```CMake

cmake_minimum_required(VERSION 3.14)
project(myProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick OpenGL)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick OpenGL)
# Note: If you installed on specific location path make sure you specify here like: find_package(WeaChart PATHS "my/custom/path/" REQUIRED)
find_package(WeaChart REQUIRED) 

# Note: Also its same like above Note.
# Optional.
# You can print "WeaChart_QML_IMPORT_PATH" constant on main.cpp to see which path is on your system.
set(QML_IMPORT_PATH /usr/local/share/qml CACHE STRING "" FORCE)

add_executable(myProject
        main.cpp
        qml.qrc
)

target_link_libraries(myProject PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick WeaChart::WeaChart)

```
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

## Custom Data Struct

Series are implemented as **templates**:
```cpp
template <typename T>
class GLSeriesStorage {...};
```
- T must inherit from `PointXYBase`.
- `PointXYBase` contains:
  - `QVector2D` position.
  - [ChartColor](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/utils/GLStructures.h#L30-L36) color(rgba).

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
> see [GLEnums.h](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/series/GLEnums.h) for more details about types.

## Interaction
- Pan (optional), set `limitView`: **true** property on [GLChartFrame.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLChartFrame.qml#L43).
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
- Minimum chart memory: 12 * 6MB = 72MB  [see MAX_CHART_TOTAL_POINTS](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/include/WeaChart/scenes/GLChartview.h#L21)
- Configurable via internal variable.
- Performance depends on: 
        - Number of points per series.
        - Number of series.
        - Size of point struct
- Continous property updates (type, color, name) via timers **may degrade performance**.
---

## Configuration
- **Pan friction** adjustable. [velocityCoefficient](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLChartFrame.qml#L44)
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

// Importing WeaChart Lib
#include <WeaChart/GLChart>

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
    
    // Activating QDebug
    qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    /** 
        Printing WeaChart_QML_IMPORT_PATH, 
        Assigning this string to QML_IMPORT_PATH (on CMakeLists.txt or .pro)
        helps IDE to read .qml files properties and options.
    **/
    qDebug() << "WeaChart_QML_IMPORT_PATH: " << WeaChart_QML_IMPORT_PATH;

    // Register MetaTypes of chart
    GLChartView::registerMetaTypes(&engine);

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

import com.wearily.WeaChart 1.0 // Importing qml files.

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
[See example usage](http://172.16.50.13/parsa/qcustommodels/-/tree/main/Chart/example)

## Additional Notes
- Highly **customizable** and flexible charting system.
- Supports **real-time updates**, but excessive property changes reduce performance.
- Designed for **professional applications** requiring high-performance rendering.
---

## Information

If you want to learn more about the **WeaChart** library and view the full
documentation, refer to the following sections:

- [See the complete documentation](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/docs/html/index.html)
- [See the source code description](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/docs/README.md)
