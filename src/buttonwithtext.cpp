#include "buttonwithtext.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <qDebug>

ButtonWithText::ButtonWithText(const QString& btnText, const QString& labelText, QWidget* parent)
    : QPushButton(parent)  // 注意：不传递btnText给父类
{
    setText("");

    setCursor(Qt::PointingHandCursor);

    setAttribute(Qt::WA_Hover, true);
    QLabel* imageContainer = new QLabel();
    imageContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_map = new QLabel(imageContainer);
    m_map->setAlignment(Qt::AlignCenter);
    m_map->setCursor(Qt::PointingHandCursor);
    m_map->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_map->setScaledContents(true);

    QPixmap pixmap(btnText);
    if (!pixmap.isNull()) {
        m_map->setPixmap(pixmap);
    }

    QHBoxLayout* imageLayout = new QHBoxLayout(imageContainer);
    imageLayout->setContentsMargins(0, 0, 0, 0);
    imageLayout->setSpacing(0);
    imageLayout->addWidget(m_map,1);

    m_label = new QLabel(labelText,this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setCursor(Qt::PointingHandCursor);
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_label->setAttribute(Qt::WA_Hover, true);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(imageContainer, 1);
    m_layout->addWidget(m_label, 0, Qt::AlignHCenter);
    m_layout->setContentsMargins(1, 1, 1, 1);
    m_layout->setSpacing(1);

    setStyleSheet(R"(
        QPushButton {
            border: none;
            background-color: #edf7ff;
        }
    )");

    m_map->setStyleSheet(R"(
        QLabel {
            border: 1px solid #ccc;
            border-radius: 5px;

        }
    )");

    m_label->setStyleSheet(R"(
        QLabel {

            font-size: 14px;
            color: #333333;
            padding: 4px;
            font-weight: bold;
        }

    )");

    // 为标签安装事件过滤器，捕获标签的点击事件
    m_map->installEventFilter(this);
    m_label->installEventFilter(this);

}

void ButtonWithText::setButtonSize(int size)
{
    setFixedSize(size, size);
}

void ButtonWithText::setButtonSize(int width, int height)
{
    setFixedSize(width, height);
}

void ButtonWithText::setSpacing(int spacing)
{
    if (m_layout) {
        m_layout->setSpacing(spacing);
    }
}

void ButtonWithText::setLabelStyle(const QString& styleSheet)
{
    if (m_label) {
        m_label->setStyleSheet(styleSheet);
    }
}


bool ButtonWithText::eventFilter(QObject* obj, QEvent* event)
{
    if ((obj == m_label || obj == m_map) &&
        event->type() == QEvent::MouseButtonPress) {
        click();
        return true;
    }


    return QPushButton::eventFilter(obj, event);
}

void ButtonWithText::enterEvent(QEvent* event)
{
    QPushButton::enterEvent(event);

    if (m_label) {
        m_label->setStyleSheet(R"(
            QLabel {
                font-size: 14px;
                color: #4f95ff;
                padding: 4px;
                font-weight: bold;
                text-decoration: underline;
            }
        )");
    }
}

void ButtonWithText::leaveEvent(QEvent* event)
{
    QPushButton::leaveEvent(event);

    if (m_label) {
        m_label->setStyleSheet(R"(
            QLabel {
                font-size: 14px;
                color: #333333;
                padding: 4px;
                font-weight: bold;
                text-decoration: none;
            }
        )");
    }
}
