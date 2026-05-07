// filename: buttonwithtext.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Custom button with image and text label, hover effects

#ifndef _TYPEGAME_BUTTONWITHTEXT_H_
#define _TYPEGAME_BUTTONWITHTEXT_H_

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ButtonWithText : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonWithText(const QString& btnText, const QString& labelText, QWidget* parent = nullptr);

    void setButtonSize(int size);
    void setButtonSize(int width, int height);
    void setSpacing(int spacing);
    void setLabelStyle(const QString& styleSheet);
    QLabel* getLabel() const { return m_label; }

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QLabel* m_map;
    QLabel* m_label;
    QVBoxLayout* m_layout;
};

#endif // _TYPEGAME_BUTTONWITHTEXT_H_
