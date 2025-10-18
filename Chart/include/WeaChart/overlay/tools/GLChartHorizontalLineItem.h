#ifndef GLCHARTHORIZONTALLINEITEM_H
#define GLCHARTHORIZONTALLINEITEM_H

#include "WeaChart/overlay/tools/GLChartLineItem.h"

/// @brief Same as GLChartLineItem but the different is the vertical property, is equal false by default.
class WEACHART_API GLChartHorizontalLineItem : public GLChartLineItem
{
    Q_OBJECT
public:
    explicit GLChartHorizontalLineItem(QQuickItem *parent = nullptr);
};

#endif // GLCHARTHORIZONTALLINEITEM_H
