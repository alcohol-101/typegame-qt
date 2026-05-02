#ifndef BUTTONWITHTEXT_H
#define BUTTONWITHTEXT_H

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ButtonWithText : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonWithText(const QString& btnText, const QString& labelText, QWidget* parent = nullptr);

    // 设置按钮大小（正方形）
    void setButtonSize(int size);

    // 设置按钮大小（宽高分别设置）
    void setButtonSize(int width, int height);

    // 设置按钮和标签之间的间距
    void setSpacing(int spacing);

    // 设置标签样式
    void setLabelStyle(const QString& styleSheet);

    // 设置按钮内数字的样式
    //void setButtonNumberStyle(const QString& styleSheet);

    // 获取标签对象
    QLabel* getLabel() const { return m_label; }


protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    QLabel *m_map;
    QLabel* m_label;
    QVBoxLayout* m_layout;
};

#endif // BUTTONWITHTEXT_H
