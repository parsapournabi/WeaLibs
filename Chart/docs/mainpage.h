/**
 * \mainpage
 *
 * \section intro_sec Introduction
 * 	A high-performance, flexible charting librarry built with C++, OpenGL, QML and Canvas,
 *  supporting multiple series types, interactive features, and customizable visuals.
 *  The chart leverages QQuickFramebufferObject for rendering.
 *
 * \section install_sec Installation
 *	[see Installation tutorial](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/README.md#build-and-installation)
 * \section usage_sec Usage
 * [see Usage and Importing](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/README.md#usage-and-importing)
 *
 *\section quick_start_sec Quick Start
 * [see WeaChart Quick Start](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/README.md#example)
 *
 *\defgroup qml_code QML
 *@{
 *	\brief QML sources.
 *	- [GLChartFrame.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLChartFrame.qml)
 *	- [GLItemLegend.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLItemLegend.qml)
 *	- [GLLegend.qml](http://172.16.50.13/parsa/qcustommodels/-/blob/main/Chart/qml/GLLegend.qml)
 *@}
 *
 *\defgroup properties_code Properties
 *@{
 *	\brief QML/C++ Properties as class.
 *	- IProperties
 *	- PropertyAxisRange
 *	- PropertyBackground
 *	- PropertySeries
 *@}
 *
 *\defgroup scenes_code Scenes & Renderer
 *@{
 *	\brief Provides monitoring and rendering of the series data.
 *	- GLChartView
 *	- GLChartRenderer
 *@}
 *
 *\defgroup series_code Series
 *@{
 *	\brief Provides C++/QML Series classes.
 *	- GLAbstractSeries
 *	- GLSeriesStorage
 *	- GLSeriesItem
 *	- GLSeriesHandle
 *@}
 *
 *\defgroup tools_code Tools
 *@{
 *	\brief Provides C++/QML Tools and Items classes.
 *	- GLChartItemBase
 *	- GLChartLineItem
 *	- GLChartVerticalLineItem
 *	- GLChartHorizontalLineItem
 *	- GLChartGateItem
 *	- GLChartVerticalGateItem
 *	- GLChartHorizontalGateItem
 *@}
 *
 *
 *\defgroup enum_code Enumerations
 *@{
 *	\brief Provides all enumations declared in to the library.
 *	- GLSeriesType : GLEnums.h
 *	- GLColorType : GLEnums.h
 *	- GLMarkerShape : GLEnums.h
 *	- GLAutoScalePolicy: GLEnums.h
 *	- GLItemType: GLEnums.h
 *	- GLStyleLine: GLEnums.h
 *@}
 *
 *\defgroup structs_code Structs
 *@{
 *	\brief Provides all sub classes and structs.
 *	- PointXYBase
 *	- ChartColor
 *	- SeriesProps
 *	- Projection
 *@}
 *
 *\defgroup utils_code Utils
 *@{
 *	\brief Provides utilities functions inside of the namespace.
 *	- GLutils
 *@}
 *
 *\defgroup macros_code Macros & Constants
 *@{
 *	\brief Provides the all definitions and the constants of the project.
 *	\section macros_sec Macros
 *	- WEACHART_API : WeaChart_export.h
 *	- WeaChart_QML_IMPORT_PATH : config.h
 *	\section constants_sec Constants
 *	- MAX_CHART_TOTAL_POINTS : GLChartview.h
 *@}
 *
 *
 *\defgroup first_example Example
 *	@{
 *	\brief Here is the how to handle the data of each series and update it to the chart.
 *	\brief datasource.h
 *	\include example/datasource.h
 *	\brief datasource.cpp
 *	\include example/datasource.cpp
 *	\brief main.qml
 *	\include example/main.qml
 *	\brief main.cpp
 *	\include example/main.cpp
 *
 * 	@}
 *
*/
