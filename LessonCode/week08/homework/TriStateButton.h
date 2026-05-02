// TriStateButton.h
#ifndef TRISTATEBUTTON_H
#define TRISTATEBUTTON_H

#include <QPushButton>
#include <QPixmap>
#include <QSoundEffect>

class TriStateButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TriStateButton(const QString& imagePath, QWidget* parent = nullptr);
    void setFixedSize(int w, int h);

    void setHoverSound(const QString& soundPath);    // 悬浮音效
    void setPressedSound(const QString& soundPath);  // 按下音效

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    // 三帧独立存储
    QPixmap m_frames[3];   // [0]=默认, [1]=Hover, [2]=按下
    enum State { Normal, Hover, Pressed };
    State m_state;

    QSoundEffect* m_hoverSound = nullptr;    // 悬浮音效
    QSoundEffect* m_pressedSound = nullptr;  // 按下音效
};

#endif // TRISTATEBUTTON_H