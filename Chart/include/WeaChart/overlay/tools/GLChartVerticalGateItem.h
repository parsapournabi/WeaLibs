#ifndef GLCHARTVERTICALGATEITEM_H
#define GLCHARTVERTICALGATEITEM_H

#include "WeaChart/overlay/tools/GLChartGateItem.h"

/// @brief Same as GLChartGateItem but the different is the vertical property is equal true by default.
class WEACHART_API GLChartVerticalGateItem : public GLChartGateItem
{
    Q_OBJECT
public:
    explicit GLChartVerticalGateItem(QQuickItem *parent= nullptr);
};

#endif // GLCHARTVERTICALGATEITEM_H
