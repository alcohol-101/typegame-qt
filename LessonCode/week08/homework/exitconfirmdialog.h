#ifndef EXITCONFIRMDIALOG_H
#define EXITCONFIRMDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QSoundEffect> 

class ExitConfirmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ExitConfirmDialog(QWidget* parent, QString back_url, QString left_url, QString right_url, QString text);
    explicit ExitConfirmDialog(QWidget* parent, QString back_url, QString left_url, QString min_url, QString right_url, QString text);
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

#endif