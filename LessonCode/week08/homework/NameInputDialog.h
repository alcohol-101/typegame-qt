// NameInputDialog.h
#ifndef NAMEINPUTDIALOG_H
#define NAMEINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>

class NameInputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NameInputDialog(QWidget *parent = nullptr);
    QString name() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLineEdit *m_lineEdit;
    QPushButton *m_okBtn, *m_cancelBtn;
    //QPixmap m_background;
};

#endif // NAMEINPUTDIALOG_H