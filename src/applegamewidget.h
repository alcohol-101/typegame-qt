#ifndef APPLEGAMEWIDGET_H
#define APPLEGAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QSoundEffect> 
#include <QPainter>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <cmath>
#include <QJsonObject>


class QPushButton;
class QLabel;
class SettingsDialog;

struct Apple {
    QChar letter;
    QPointF pos;
    bool isBad;
    QTimer* badTimer;           // 坏苹果停留0.5秒计时器（仅用于bad苹果）
    bool operator==(const Apple& other) const {
        return letter == other.letter;
    }
};

class AppleGameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppleGameWidget(QWidget* parent = nullptr);
    ~AppleGameWidget();

    void startGame();
    void pauseGame();
    void resumeGame();
    void stopGame();            // 结束游戏（回到未开始状态，不保留进度）
    void showSettings();

	// 测试相关方法
    void setTestMode(bool enabled);
    void setTestLetters(const QString& letters);
    void setGameParameters(const QJsonObject& config);
    QJsonObject getGameStats() const;
    bool isGameActive() const { return m_gameActive; }
    int getSuccessCount() const { return m_successCount; }
    int getBadCount() const { return m_badCount; }
    int getWrongInputCount() const { return m_wrongInputCount; }
    int getMaxHealth() const { return m_maxBadAppleCount; }
    int getCurrentHealth() const { return m_maxBadAppleCount - m_badCount; }
    int activeAppleCount() const {
        int count = 0;
        for (const Apple& a : m_apples) if (!a.isBad) ++count;
        return count;
    }

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;


private slots:
    void onStartClicked();
    void onPauseClicked();
    void onStopClicked();
    void onSettingsClicked();
    void onExitClicked();
    void onUpdateTimer();
    void onAppleBadTimeout();   // 处理坏苹果消失

    void applySettings(int level, int targetCount, int maxBadCount, int maxAppleCount, bool soundEnabled);

private:
    void generateApple();
    void removeAppleByLetter(QChar letter);
    void convertAppleToBad(Apple& apple);       // 将苹果变为坏苹果并开始0.5秒计时
    void updateBasketSmallApples();
    void resetGameState();                      // 重置为未开始状态（保留设置参数）
    void checkGameOver();

    void drawBackground(QPainter& painter);
    void drawApples(QPainter& painter);
    void drawBasketAndSmallApples(QPainter& painter);

    QPushButton* m_exitBtn;
    QPushButton* m_startBtn;
    QPushButton* m_pauseBtn;
    QPushButton* m_stopBtn;
    QPushButton* m_settingsBtn;

    int m_speedLevel;           // 0-9，决定下落速度（像素/帧）
    int m_targetAppleCount;     // 过关所需成功苹果数
    int m_maxBadAppleCount;
    int m_maxAppleCount;

    bool m_gameActive;          // 游戏是否运行中（包括暂停？实际运行中才更新）
    bool m_gamePaused;
    int m_successCount;
    int m_badCount;
    int m_smallAppleCount;      // 右下角小苹果数量 (0-8)

    bool m_showGameElements;
    bool m_soundEnabled;

    QList<Apple> m_apples;
    QTimer* m_updateTimer;      // 更新定时器（约30ms）
    QRandomGenerator m_random;

    QPixmap m_normalApplePixmap;
    QPixmap m_badApplePixmap;
    QPixmap m_basketPixmap;
    QPixmap m_smallApplePixmap;
    QPixmap m_backgroundPixmap;

    QSize m_appleSize;
    QPoint m_basketPos;
    QSize m_smallAppleSize;

    QMediaPlayer* m_bgmPlayer;
    QSoundEffect* m_hoverSoundEffect;
    QSoundEffect* m_clickSoundEffect;
    QSoundEffect* m_successSoundEffect;

    // 设置对话框指针（避免重复创建）
    SettingsDialog* m_settingsDialog;

    QScreen* screen = QApplication::primaryScreen();
    QRect fullGeometry = screen->geometry();
    double w_primary = fullGeometry.width();
    double h_primary = fullGeometry.height();

    //命令行测试相关成员
    bool m_testMode = false;
    QString m_testLetters;
    int m_testLetterIndex = 0;
    int m_wrongInputCount = 0;

};

#endif // APPLEGAMEWIDGET_H