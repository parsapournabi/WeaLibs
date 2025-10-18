#ifndef GLCHARTITEMBASE_H
#define GLCHARTITEMBASE_H

#include <QLinearGradient>
#include <QColor>
#include <QUrl>
#include <QImage>
#include <QOpenGLFunctions>

#include "WeaChart/properties/IProperties.h"
#include "WeaChart/series/GLEnums.h"


/// @brief AbstractChartItem based on QQuickItem contains base properties and methods.
/// @details The class has 3 virtual method that should be implemented on the children classes.
class WEACHART_API GLChartItemBase : public IProperties
{
    Q_OBJECT
    /// @brief color of the ChartItem
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    /// @brief Gradient of the ChartItem.
    Q_PROPERTY(QLinearGradient gradient READ gradient WRITE setGradient NOTIFY gradientChanged)
    /// @brief ImageUrl of the ChartItem, the format must be ":/path/to/img.png" or "qrc:/path/to/img.png"
    Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY imageChanged)
    /// @brief Referer GLStyleLine or Qt.PenStyle
    Q_PROPERTY(GLStyleLine lineStyle READ lineStyle WRITE setLineStyle NOTIFY lineStyleChanged)
    /// @brief A boolean to activate item gradient.
    Q_PROPERTY(bool useGradient READ useGradient WRITE setUseGradient NOTIFY useGradientChanged)
    /// @brief A boolean to activate item image.
    Q_PROPERTY(bool useImage READ useImage WRITE setUseImage NOTIFY useImageChanged)
    /// @brief By enabling this property, the GLChartView projection will deactivate.
    /// @note If fixItem true, the from/to or position property value must between 0.0 and 1.0. for example 0.0 is left and bottom and 1.0 is right and top.
    Q_PROPERTY(bool fixItem READ fixItem WRITE setfixItem NOTIFY fixItemChanged)
    /// @brief An abstract property to define the Item as Vertically or Horizontally.
    Q_PROPERTY(bool vertical READ vertical WRITE setVertical NOTIFY verticalChanged)
    /// @brief ChartItem visibility
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit GLChartItemBase(QQuickItem *parent = nullptr);
    ~GLChartItemBase();

    // Properties
    /// @brief getter of color property but as Vector4D
    /// @return QVector4D: the rgba value r = x, g = y, b = z, a = w.
    QVector4D colorAsVec4D() const;
    /// @brief getter of color property.
    /// @return QColor.
    QColor color() const;
    /// @brief setter of color property.
    /// @param QColor: value of the color.
    void setColor(const QColor &color);

    /// @brief getter of gradient property but as Vector4D
    /// @return QVector4D: the rgba value start().x() = x, start().y() = y, stop().x() = z, stop().y() = w.
    QVector4D gradientAsVec4D() const;
    /// @brief getter of gradient property.
    /// @return QLinearGradient.
    QLinearGradient gradient() const;
    /// @brief setter of gradient property.
    /// @param QLinearGradient: value of the gradient.
    void setGradient(const QLinearGradient gradient);

    /// @brief getter of image property but as QImage.
    /// @return QImage(image()).
    const QImage &imageData() const;
    /// @brief getter of image property.
    /// @return QUrl: url of the image.
    QUrl image() const;
    /// @brief setter of image property.
    /// @param QUrl: value of the imageUrl.
    void setImage(const QUrl &url);

    /// @brief getter of lineStyle property.
    /// @return GLStyleLine.
    GLStyleLine lineStyle() const;
    /// @brief setter of lineStyle property.
    /// @param GLStyleLine.
    void setLineStyle(GLStyleLine style);

    /// @brief getter of useGradient property.
    /// @return bool.
    bool useGradient() const;
    /// @brief setter of useGradient property.
    /// @param use.
    void setUseGradient(bool useGrad);

    /// @brief getter of useImage property.
    /// @return bool.
    bool useImage() const;
    /// @brief setter of useImage property.
    /// @param bool.
    void setUseImage(bool useImg);

    /// @brief getter of fixItem property.
    /// @return bool.
    bool fixItem() const;
    /// @brief setter of fixItem property.
    /// @param bool.
    void setfixItem(bool fix);

    /// @brief sync with vertical property.
    /// @return bool.
    bool isVertical() const;
    /// @brief getter of vertical property.
    /// @return bool.
    bool vertical() const;
    /// @brief setter of vertical property.
    /// @param bool.
    void setVertical(bool enable);

    /// @brief sync with visible property.
    bool isVisible() const;
    /// @brief getter of visible property.
    /// @return bool.
    bool visible() const;
    /// @brief setter of visible property.
    /// @param bool.
    void setVisible(bool visible);

    /// @brief Vertices coordinate which used at QOpenGLBuffer::write.
    /// @note the method must be override at child class.
    /// @return QVector<QVector2D> decalered vertices position depends on fixItem property and item coordinte.
    virtual QVector<QVector2D> vertices() const noexcept = 0;
    /// @brief The type of the ChartItem (it can be Line or Gate).
    /// @note the method must be override at child class.
    /// @return GLItemType
    virtual GLItemType type() const = 0;
    /// @brief the drawType means the ChartItem should draw as "GL_LINE" or "GL_TRIANGLE_STRIP" or etc.
    /// @note the method must be override at child class.
    /// @return GLenum.
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

    // visibleChanged has been decalered at QQuickItem
private:
    QColor m_color;
    QLinearGradient m_gradient;
    QImage m_imageData;
    QUrl m_image;
    GLStyleLine m_lineStyle = GLStyleLine::NoStyle;
    bool m_useGradient = false;
    bool m_useImage = false;
    bool m_fixItem = false;
    bool m_vertical = false;
    bool m_visible = true;

};

#endif // GLCHARTITEMBASE_H
