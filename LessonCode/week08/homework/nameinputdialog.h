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
};

#endif // NAMEINPUTDIALOG_H