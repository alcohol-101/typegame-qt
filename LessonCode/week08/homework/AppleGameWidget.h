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


class QPushButton;
class QLabel;
class SettingsDialog;

// 苹果结构体
struct Apple {
    QChar letter;               // 显示的字母 A-Z
    QPointF pos;                // 当前位置（中心点）
    bool isBad;                 // 是否为坏苹果状态
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

    void startGame();           // 开始新游戏（重置状态）
    void pauseGame();           // 暂停
    void resumeGame();          // 继续
    void stopGame();            // 结束游戏（回到未开始状态，不保留进度）
    void showSettings();        // 弹出设置对话框

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
    void generateApple();                       // 随机生成一个苹果
    void removeAppleByLetter(QChar letter);     // 根据字母消除苹果（成功）
    void convertAppleToBad(Apple& apple);       // 将苹果变为坏苹果并开始0.5秒计时
    void updateBasketSmallApples();             // 更新右下角小苹果数量
    void resetGameState();                      // 重置为未开始状态（保留设置参数）
    void checkGameOver();                       // 检查是否通关或失败

    void drawBackground(QPainter& painter);
    void drawApples(QPainter& painter);
    void drawBasketAndSmallApples(QPainter& painter);

    // UI控件
    QPushButton* m_exitBtn;
    QPushButton* m_startBtn;
    QPushButton* m_pauseBtn;
    QPushButton* m_stopBtn;
    QPushButton* m_settingsBtn;

    // 游戏参数
    int m_speedLevel;           // 0-9，决定下落速度（像素/帧）
    int m_targetAppleCount;     // 过关所需成功苹果数
    int m_maxBadAppleCount;     // 允许的最大失败苹果数
    int m_maxAppleCount;        // 同屏最大苹果数量

    // 游戏状态
    bool m_gameActive;          // 游戏是否运行中（包括暂停？实际运行中才更新）
    bool m_gamePaused;
    int m_successCount;         // 已成功消除的苹果数
    int m_badCount;             // 已失败的苹果数
    int m_smallAppleCount;      // 右下角小苹果数量 (0-8)

    bool m_showGameElements;    // 是否显示游戏元素（苹果、篮子等）
    bool m_soundEnabled;        // 音效开关

    QList<Apple> m_apples;      // 当前屏幕上的苹果列表
    QTimer* m_updateTimer;      // 更新定时器（约30ms）
    QRandomGenerator m_random;

    // 图片资源
    QPixmap m_normalApplePixmap;
    QPixmap m_badApplePixmap;
    QPixmap m_basketPixmap;
    QPixmap m_smallApplePixmap;
    QPixmap m_backgroundPixmap; // 背景图（如果没有可纯色）

    // 尺寸缓存
    QSize m_appleSize;          // 苹果图片绘制大小
    QPoint m_basketPos;         // 篮子绘制位置（右下角）
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

};

#endif // APPLEGAMEWIDGET_H