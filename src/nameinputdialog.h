// filename: nameinputdialog.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Player name input dialog for high score entry

#ifndef _TYPEGAME_NAMEINPUTDIALOG_H_
#define _TYPEGAME_NAMEINPUTDIALOG_H_

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>

class NameInputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NameInputDialog(QWidget* parent = nullptr);
    QString name() const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QLineEdit* m_lineEdit;
    QPushButton* m_okBtn, *m_cancelBtn;
};

#endif // _TYPEGAME_NAMEINPUTDIALOG_H_