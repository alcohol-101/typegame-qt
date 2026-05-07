// filename: spacewarwidget.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Space war typing game widget with LLM word generation

#ifndef _TYPEGAME_SPACEWARWIDGET_H_
#define _TYPEGAME_SPACEWARWIDGET_H_

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QSet>
#include <QRandomGenerator>
#include <QSoundEffect>
#include <QPixmap>
#include <QElapsedTimer> 
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class TriStateButton;
class SpaceSettingsDialog;
class HighScoreDialog;

enum ObjectType { Enemy, Meteorite };

struct Explosion {
    QPointF pos;
    int frame = 0;          // 0-8
    bool active = true;
};

struct GameObject {
    ObjectType type;
    QChar letter;
    QPointF pos;
    int frame = 0;
    int frameCount = 1;

    // 敌机专用
    double initialX = 0.0;
    double targetX = 0.0;
    bool phaseOneDone = false;          // 第一阶段是否完成
    double oscillationCenterX = 0.0;    // 第二阶段水平振荡中心
    double oscillationTime = 0.0;
    double oscAmplitude = 32.0;  // 振幅
    double oscOmega = 3.0;       // 角频率

    double cylinderAngle = 0.0;

    double fallSpeed = 2.0;
    double horizontalSpeed = 0.0;
    bool exploded = false;
    bool toBeRemoved = false;
    QTimer* animTimer = nullptr;
};

struct Bullet {
    QChar targetLetter;
    QPointF pos;
    QPointF targetPos;
    double currentAngle = 0;
    bool active = true;
};

struct RewardWord {
    QString text;
    int currentIndex = 0;
    QPointF pos;
    double speedX = 2.0;
    bool finished = false;
};

class SpaceWarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpaceWarWidget(QWidget* parent = nullptr);
    ~SpaceWarWidget();

	//测试相关方法
    void setTestMode(bool enabled);
    void setTestLetters(const QString& letters);
    void setGameParameters(const QJsonObject& config);
    QJsonObject getGameStats() const;
    bool isGameActive() const { return m_gameActive; }
    int getScore() const { return m_score; }
    int getLives() const { return m_lives; }
    int getMaxLives() const { return m_maxLives; }
    int getCorrectInputCount() const { return m_correctInputCount; }
    int getWrongInputCount() const { return m_wrongInputCount; }
    int getMissedCount() const { return m_missedCount; }
    void skipSounds() { m_soundEnabled = false; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onStartClicked();
    void onHighScoreClicked();
    void onOptionsClicked();
    void onExitClicked();
    void onReturnClicked() { resumeGame(); }
    void gameLoop();
    void onExplosionFrame();        // 统一更新爆炸动画帧
    void onWordSpawnTimer();        // 生成奖励单词

private:
    TriStateButton* m_startBtn, * m_highScoreBtn, * m_optionsBtn, * m_exitBtn;
    TriStateButton* m_returnBtn;    // 暂停时替换开始按钮

    int m_enemyMaxCount = 3;
    int m_meteoriteMaxCount = 1;    // = 敌机/2
    int m_speedLevel = 3;
    int m_upgradeIntervalMs = 120000;
    bool m_rewardEnabled = true;

    bool m_gameActive = false;
    bool m_gamePaused = false;
    bool m_showMainMenu = true;
    int m_score = 0;
    int m_lives = 18;
    const int m_maxLives = 18;
    int m_upgradeTimerCount = 1000;
    bool m_upgradedFlag = false;
    bool m_waitingForNameInput = false;

    QPointF m_playerPos;
    double m_playerSpeed = 6.0;
    int m_playerFrame = 0;
    bool m_playerRightDir = true;
    QTimer* m_playerAnimTimer = nullptr;

    QList<GameObject> m_objects;
    QList<Bullet> m_bullets;
    QList<Explosion> m_explosions;
    QList<RewardWord> m_rewardWords;

    QSet<QChar> m_usedLetters;

    QTimer* m_gameTimer = nullptr;           // 主循环 33fps
    QTimer* m_upgradeTimer = nullptr;
    QTimer* m_explosionCheckTimer = nullptr; // 爆炸帧更新 (0.1s)
    QTimer* m_wordSpawnTimer = nullptr;

    QElapsedTimer m_upgradeShowTimer;

    QPixmap m_playerPixmap;       // 3x4 11帧
    QPixmap m_meteoritePixmap;    // 3x4 12帧
    QPixmap m_enemyPixmap;
    QPixmap m_enemyUp;
    QPixmap m_explosionPixmap;    // 3x3 9帧
    QPixmap m_bulletPixmap;
    QPixmap m_background;
    QPixmap m_heartIcon;
    QPixmap m_lifeIcon;
    QPixmap m_scoreIcon;
    QPixmap m_timeIcon;
    QPixmap m_mainMenuBackground;
    QPixmap m_gameBackground;

    QMediaPlayer* m_bgmPlayer=nullptr;
    bool m_soundEnabled = true;

    QSoundEffect* m_shootSound = nullptr;
    QSoundEffect* m_explosionSound = nullptr;
    QSoundEffect* m_generateSound = nullptr;
    QSoundEffect* m_bonusSound = nullptr;

    SpaceSettingsDialog* m_settingsDialog = nullptr;
    HighScoreDialog* m_highScoreDialog = nullptr;
    QString m_scoreFilePath;

    void initGame();
    void stopGame();
    void updatePlayer();
    void spawnObject();
    void updateObjects();
    void updateBullets();
    void updateRewardWords();
    void checkCollisions();
    void checkEndGame();
    void handleGameOver();
    void applyUpgrade();
    void handleLetterInput(QChar letter);
    void explodeObject(GameObject* obj);    // 参数改为指针
    void spawnRewardWord();

    QString getLineWord(const QString& resourcePath);
    QString generateRewardWord();
    QChar getUnusedLetter();
    void releaseLetter(QChar ch);

    void drawBackground(QPainter& p);
    void drawMainMenu(QPainter& p);
    void drawHUD(QPainter& p);
    void drawPlayer(QPainter& p);
    void drawObjects(QPainter& p);
    void drawBullets(QPainter& p);
    void drawExplosions(QPainter& p);
    void drawRewardWords(QPainter& p);

    QRect getPlayerRect() const;            // 不再是 virtual/override
    QRect getObjectRect(const GameObject& obj) const;

    void pauseGame();
    void resumeGame();

    // ---- LLM 单词生成 ----
    void prefillLLMPool();
    void requestLLMWord();
    void onLLMWordReceived(QNetworkReply* reply);

    QNetworkAccessManager* m_networkManager = nullptr;
    QStringList m_llmWordPool;
    int m_llmPendingRequests = 0;

	//测试相关成员
    bool m_testMode = false;
    QString m_testLetters;
    int m_testLetterIndex = 0;
    int m_correctInputCount = 0;
    int m_wrongInputCount = 0;
    int m_missedCount = 0;
};

#endif // _TYPEGAME_SPACEWARWIDGET_H_