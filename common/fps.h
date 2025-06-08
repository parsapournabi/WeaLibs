#ifndef FPS_H
#define FPS_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QDateTime>
#include <QBrush>
#include <QPainter>
#include <QTimer>

class Fps : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
public:
    explicit Fps(QQuickItem *parent = nullptr) {
        m_times.clear();
        setFlag(QQuickItem::ItemHasContents);
    }
    ~Fps() {

    }

    void paint(QPainter *painter) {
        recalculateFPS();

        QBrush brush(Qt::yellow);

        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRoundedRect(0, 0, boundingRect().width(), boundingRect().height(), 0, 0);

        update();


    }

    Q_INVOKABLE int fps() const { return m_currentFPS; }

signals:
    void fpsChanged(int fps);

private:
    void recalculateFPS() {

        qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        m_times.push_back(currentTime);

        while( m_times[0] < currentTime - 1000) {
            m_times.pop_front();
        }

        int currentCount = m_times.length();
        m_currentFPS = (currentCount + m_cacheCount) / 2;
//        qDebug() << m_currentFPS;

        if (currentCount != m_cacheCount) fpsChanged(m_currentFPS);

        m_cacheCount = currentCount;


    }
private:
    int m_currentFPS = 0;
    int m_cacheCount = 0;
    QVector<qint64> m_times;



};

#endif // FPS_H
