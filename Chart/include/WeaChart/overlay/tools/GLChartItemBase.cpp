#include "GLChartItemBase.h"

#include <QVector4D>

GLChartItemBase::GLChartItemBase(QQuickItem *parent)
    : IProperties{parent}
{
    //    connect(this, &GLChartItemBase::changed, this, &GLChartItemBase::update);
}

GLChartItemBase::~GLChartItemBase()
{
}

QVector4D GLChartItemBase::colorAsVec4D() const
{
    return QVector4D(m_color.redF(),
                     m_color.greenF(),
                     m_color.blueF(),
                     m_color.alphaF());
}

QColor GLChartItemBase::color() const
{
    return m_color;
}

void GLChartItemBase::setColor(const QColor &color)
{
    if (color == m_color) return;
    m_color = color;
    emit colorChanged();
    emit changed();
}

QVector4D GLChartItemBase::gradientAsVec4D() const
{
    return QVector4D(m_gradient.start().x(),
                     m_gradient.start().y(),
                     m_gradient.finalStop().x(),
                     m_gradient.finalStop().y());
}

QLinearGradient GLChartItemBase::gradient() const
{
    return m_gradient;
}

void GLChartItemBase::setGradient(const QLinearGradient gradient)
{
    if (gradient == m_gradient) return;
    m_gradient = gradient;
    emit gradientChanged();
    emit changed();
}

const QImage &GLChartItemBase::imageData() const
{
    return m_imageData;
}

QUrl GLChartItemBase::image() const
{
    return m_image;
}

void GLChartItemBase::setImage(const QUrl &url)
{
    if (url == m_image) return;
    m_image = url;
    QString path = m_image.toString();
    if (path.startsWith("qrc:")) path.replace(0, 3, "");
    m_imageData = QImage(path);
    emit imageChanged();
    emit changed();
    update();
}

GLStyleLine GLChartItemBase::lineStyle() const
{
    return m_lineStyle;
}

void GLChartItemBase::setLineStyle(GLStyleLine style)
{
    if (style == m_lineStyle) return;
    m_lineStyle = style;
    emit lineStyleChanged();
    emit changed();
}

bool GLChartItemBase::useGradient() const
{
    return m_useGradient;
}

void GLChartItemBase::setUseGradient(bool useGrad)
{
    if (useGrad == m_useGradient) return;
    m_useGradient = useGrad;
    emit useGradientChanged();
    emit changed();
}

bool GLChartItemBase::useImage() const
{
    return m_useImage;
}

void GLChartItemBase::setUseImage(bool useImg)
{
    if (useImg == m_useImage) return;
    m_useImage = useImg;
    emit useImageChanged();
    emit changed();
}

bool GLChartItemBase::fixItem() const
{
    return m_fixItem;
}

void GLChartItemBase::setfixItem(bool fix)
{
    if (fix == m_fixItem) return;
    m_fixItem = fix;
    emit fixItemChanged();
    emit changed();
}

bool GLChartItemBase::isVertical() const
{
    return m_vertical;
}

bool GLChartItemBase::vertical() const
{
    return m_vertical;
}

void GLChartItemBase::setVertical(bool enable)
{
    if (enable == m_vertical) return;
    m_vertical = enable;
    emit verticalChanged();
    emit changed();
}

bool GLChartItemBase::isVisible() const
{
    return m_visible;
}

bool GLChartItemBase::visible() const
{
    return m_visible;
}

void GLChartItemBase::setVisible(bool visible)
{
    if (visible == m_visible) return;
    m_visible = visible;
    emit visibleChanged();
    emit changed();
}
