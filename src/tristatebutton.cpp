// filename: tristatebutton.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Implementation of TriStateButton

#include "tristatebutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

TriStateButton::TriStateButton(const QString& imagePath, QWidget* parent)
    : QPushButton(parent)
    , m_state(Normal)
{
    QPixmap fullPixmap(imagePath);

    if (!fullPixmap.isNull()) {
        // 图片宽 = 3帧，每帧宽 = 总宽 / 3
        int frameWidth = fullPixmap.width() / 3;
        int frameHeight = fullPixmap.height();

        m_frames[0] = fullPixmap.copy(0, 0, frameWidth, frameHeight);
        m_frames[1] = fullPixmap.copy(frameWidth, 0, frameWidth, frameHeight);
        m_frames[2] = fullPixmap.copy(frameWidth * 2, 0, frameWidth, frameHeight);
    }

    m_hoverSound = new QSoundEffect(this);
    m_pressedSound = new QSoundEffect(this);

    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_Hover, true);
    setStyleSheet("border: none;");
}

void TriStateButton::setFixedSize(int w, int h)
{
    QPushButton::setFixedSize(w, h);
}

void TriStateButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    int frameIndex = static_cast<int>(m_state);
    const QPixmap& currentFrame = m_frames[frameIndex];

    if (currentFrame.isNull()) return;

    QPainter painter(this);
    painter.drawPixmap(rect(), currentFrame);
}

void TriStateButton::enterEvent(QEvent* event)
{
    if (m_state != Pressed) {
        m_state = Hover;

        if (m_hoverSound && m_hoverSound->isLoaded()) {
            m_hoverSound->play();    
        }
        update();
    }
    QPushButton::enterEvent(event);
}

void TriStateButton::leaveEvent(QEvent* event)
{
    if (m_state != Pressed) {
        m_state = Normal;
       
        update();
    }
    QPushButton::leaveEvent(event);
}

void TriStateButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_state = Pressed;

        if ( m_pressedSound && m_pressedSound->isLoaded()) {
            m_pressedSound->play();
        }
        update();
    }
    QPushButton::mousePressEvent(event);
}

void TriStateButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            m_state = Hover;
        }
        else {
            m_state = Normal;
        }
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void TriStateButton::setHoverSound(const QString& soundPath)
{
    if (m_hoverSound && !soundPath.isEmpty()) {
        m_hoverSound->setSource(QUrl(soundPath));
    }
}

void TriStateButton::setPressedSound(const QString& soundPath)
{
    if (m_pressedSound && !soundPath.isEmpty()) {
        m_pressedSound->setSource(QUrl(soundPath));
    }
}