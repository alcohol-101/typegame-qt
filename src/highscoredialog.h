// filename: highscoredialog.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: High score display dialog with file persistence

#ifndef _TYPEGAME_HIGHSCOREDIALOG_H_
#define _TYPEGAME_HIGHSCOREDIALOG_H_

#include <QDialog>
#include <QVector>
#include <QPair>
#include <QPixmap>

class TriStateButton;

class HighScoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighScoreDialog(const QString& scoreFilePath, QWidget* parent = nullptr);

    void loadScores();
    void saveScores();
    void addScore(const QString& name, int score);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onReturnClicked();

private:
    QString m_filePath;
    QPixmap m_background;
    QVector<QPair<QString, int>> m_entries;
    TriStateButton* m_returnBtn;
};

#endif // _TYPEGAME_HIGHSCOREDIALOG_H_