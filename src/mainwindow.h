#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void MainWindow::mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QPoint m_dragPosition;
    QWidget *titleBar;
    bool m_bDrag=true;
};
#endif // MAINWINDOW_H
