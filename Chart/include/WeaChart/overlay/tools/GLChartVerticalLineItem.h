#ifndef GLCHARTVERTICALLINEITEM_H
#define GLCHARTVERTICALLINEITEM_H

#include "WeaChart/overlay/tools/GLChartLineItem.h"

/// @brief Same as GLChartLineItem but the different is the vertical property, is equal true by default.
class WEACHART_API GLChartVerticalLineItem : public GLChartLineItem
{
    Q_OBJECT
public:
    explicit GLChartVerticalLineItem(QQuickItem *parent = nullptr);
};

#endif // GLCHARTVERTICALLINEITEM_H
