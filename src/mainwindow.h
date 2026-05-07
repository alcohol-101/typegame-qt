// filename: mainwindow.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Frameless main window with custom title bar and game selection

#ifndef _TYPEGAME_MAINWINDOW_H_
#define _TYPEGAME_MAINWINDOW_H_

#include <QMainWindow>
#include <QMouseEvent>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void setupContent();
    Ui::MainWindow* ui;
    QPoint m_dragPosition;
    QWidget* m_titleBar;
    bool m_bDrag=true;
};
#endif // _TYPEGAME_MAINWINDOW_H_
