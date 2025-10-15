#include "GLChartVerticalLineItem.h"

GLChartVerticalLineItem::GLChartVerticalLineItem(QQuickItem *parent)
    : GLChartLineItem{parent}
{
    setVertical(true);
}
