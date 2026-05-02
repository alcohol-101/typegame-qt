#include "ButtonWithText.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <qDebug>

ButtonWithText::ButtonWithText(const QString& btnText, const QString& labelText, QWidget* parent)
    : QPushButton(parent)  // 注意：不传递btnText给父类
{
    // 清除按钮本身的文字
    setText("");

    // 设置鼠标样式
    setCursor(Qt::PointingHandCursor);

    setAttribute(Qt::WA_Hover, true);
    // 创建显示图片

    QLabel* imageContainer = new QLabel();
    imageContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_map = new QLabel(imageContainer);
    m_map->setAlignment(Qt::AlignCenter);
    m_map->setCursor(Qt::PointingHandCursor);
    m_map->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_map->setScaledContents(true);//启用自动拉伸

    QPixmap pixmap(btnText);
    QPixmap stretched = pixmap.scaled(
        imageContainer->size(), // 填充整个标签
        Qt::KeepAspectRatio    ,// 保持宽高比，完全显示图片
        Qt::SmoothTransformation// 平滑处理
        );

    if (!pixmap.isNull()) {
        m_map->setPixmap(stretched);
    }

    // 水平布局使图片居中
    QHBoxLayout* imageLayout = new QHBoxLayout(imageContainer);
    imageLayout->setContentsMargins(0, 0, 0, 0);
    imageLayout->setSpacing(0);
    imageLayout->addWidget(m_map,1);



    // 创建显示名称的标签
    m_label = new QLabel(labelText,this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setCursor(Qt::PointingHandCursor);
    m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_label->setAttribute(Qt::WA_Hover, true);

    // 创建布局
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(imageContainer, 1);
    m_layout->addWidget(m_label, 0, Qt::AlignHCenter);
    m_layout->setContentsMargins(1, 1, 1, 1);
    m_layout->setSpacing(1);

    //设置默认样式 - 按钮本身样式
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

    // 设置名称标签样式
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
