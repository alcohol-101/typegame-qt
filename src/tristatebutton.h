// filename: tristatebutton.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Three-state button (Normal/Hover/Pressed) using sprite sheet

#ifndef _TYPEGAME_TRISTATEBUTTON_H_
#define _TYPEGAME_TRISTATEBUTTON_H_

#include <QPushButton>
#include <QPixmap>

class LowLatencySound;

class TriStateButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TriStateButton(const QString& imagePath, QWidget* parent = nullptr);
    void setFixedSize(int w, int h);

    void setHoverSound(const QString& soundPath);
    void setPressedSound(const QString& soundPath);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QPixmap m_frames[3];   // [0]=默认, [1]=Hover, [2]=按下
    enum State { Normal, Hover, Pressed };
    State m_state;

    LowLatencySound* m_hoverSound = nullptr;
    LowLatencySound* m_pressedSound = nullptr;
};

#endif // _TYPEGAME_TRISTATEBUTTON_H_