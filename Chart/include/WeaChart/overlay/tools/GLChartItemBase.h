#ifndef GLCHARTITEMBASE_H
#define GLCHARTITEMBASE_H

#include <QLinearGradient>
#include <QColor>
#include <QUrl>
#include <QImage>
#include <QOpenGLFunctions>

#include "WeaChart/properties/IProperties.h"
#include "WeaChart/series/GLEnums.h"


class GLChartItemBase : public IProperties
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QLinearGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
    Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(GLStyleLine lineStyle READ lineStyle WRITE setLineStyle NOTIFY lineStyleChanged)
    Q_PROPERTY(bool useGradient READ useGradient WRITE setUseGradient NOTIFY useGradientChanged)
    Q_PROPERTY(bool useImage READ useImage WRITE setUseImage NOTIFY useImageChanged)
    Q_PROPERTY(bool fixItem READ fixItem WRITE setfixItem NOTIFY fixItemChanged)
    Q_PROPERTY(bool vertical READ vertical WRITE setVertical NOTIFY verticalChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit GLChartItemBase(QQuickItem *parent = nullptr);
    ~GLChartItemBase();

    // Properties
    QVector4D colorAsVec4D() const;
    QColor color() const;
    void setColor(const QColor &color);

    QVector4D gradientAsVec4D() const;
    QLinearGradient gradient() const;
    void setGradient(const QLinearGradient gradient);

    const QImage &imageData() const;
    QUrl image() const;
    void setImage(const QUrl &url);

    GLStyleLine lineStyle() const;
    void setLineStyle(GLStyleLine style);

    bool useGradient() const;
    void setUseGradient(bool useGrad);

    bool useImage() const;
    void setUseImage(bool useImg);

    bool fixItem() const;
    void setfixItem(bool fix);

    bool isVertical() const;
    bool vertical() const;
    void setVertical(bool enable);

    bool isVisible() const;
    bool visible() const;
    void setVisible(bool visible);

    virtual QVector<QVector2D> vertices() const noexcept = 0;
    virtual GLItemType type() const = 0;
    virtual GLenum drawType() const = 0;
signals:
    void colorChanged();
    void gradientChanged();
    void imageChanged();
    void lineStyleChanged();
    void useGradientChanged();
    void useImageChanged();
    void fixItemChanged();
    void verticalChanged();
    void coordsUpdated();

    // visibleChanged has been decalered at QQuickItem
private:
    QColor m_color;
    QLinearGradient m_gradient;
    QImage m_imageData;
    QUrl m_image;
    GLStyleLine m_lineStyle = GLStyleLine::NoStyle;
    bool m_useGradient = false;
    bool m_useImage = false;
    bool m_fixItem = true;
    bool m_vertical = false;
    bool m_visible = true;

};

#endif // GLCHARTITEMBASE_H
