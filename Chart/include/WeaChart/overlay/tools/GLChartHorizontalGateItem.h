#ifndef GLCHARTHORIZONTALGATEITEM_H
#define GLCHARTHORIZONTALGATEITEM_H

#include "WeaChart/overlay/tools/GLChartGateItem.h"

/// @brief Same as GLChartGateItem but the different is the vertical property, is equal false by default.
class WEACHART_API GLChartHorizontalGateItem : public GLChartGateItem
{
    Q_OBJECT
public:
    explicit GLChartHorizontalGateItem(QQuickItem *parent = nullptr);
};

#endif // GLCHARTHORIZONTALGATEITEM_H
