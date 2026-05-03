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

    QSoundEffect* m_hoverSound = nullptr;
    QSoundEffect* m_pressedSound = nullptr;
};

#endif // TRISTATEBUTTON_H