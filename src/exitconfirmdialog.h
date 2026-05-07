// filename: exitconfirmdialog.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Exit confirmation dialog with custom background and buttons

#ifndef _TYPEGAME_EXITCONFIRMDIALOG_H_
#define _TYPEGAME_EXITCONFIRMDIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QSoundEffect> 

class ExitConfirmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExitConfirmDialog(QWidget* parent, const QString& back_url, const QString& left_url, const QString& right_url, const QString& text);
    explicit ExitConfirmDialog(QWidget* parent, const QString& back_url, const QString& left_url, const QString& min_url, const QString& right_url, const QString& text);
    ~ExitConfirmDialog();

    bool isConfirmed() const { return m_confirmed; }
    int  isConfirmed_3()const { return m_confirmed_3; }

protected:
    void paintEvent(QPaintEvent* event) override;  // 绘制背景图
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    bool m_confirmed = false;
    int m_confirmed_3 = 0;
    QPixmap m_backgroundPixmap;
    QSoundEffect* m_hoverSoundEffect;
    QSoundEffect* m_clickSoundEffect;
};

#endif // _TYPEGAME_EXITCONFIRMDIALOG_H_