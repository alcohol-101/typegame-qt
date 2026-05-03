#include "spacewarwidget.h"

#include <QPainter>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QTimer>
#include <cmath>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QElapsedTimer> 
#include <QDebug>

#include "tristatebutton.h"
#include "spacesettingsdialog.h"
#include "highscoredialog.h"
#include "nameinputdialog.h"
#include "exitconfirmdialog.h"

constexpr double PI = 3.14159265358979323846;

constexpr double bulletSpeed = 40.0;
constexpr double turnRate = 0.2;
constexpr double SPAWN_BASE_SPEED = 2.0;
constexpr double SPAWN_SPEED_MULTIPLIER = 1.0;
constexpr double METEORITE_BASE_SPEED = 2.0;
constexpr double METEORITE_SPEED_MULTIPLIER = 1.2;
constexpr double OSC_AMPLITUDE = 200.0;
constexpr double OSC_OMEGA = 1.0;
constexpr double OSC_TIME_STEP = 0.05;
constexpr int MAX_REWARD_WORDS = 1;
constexpr double DEFAULT_REWARD_WORD_SPEED = 15.0;
constexpr int GAME_OVER_DELAY_MS = 500;

// LLM API 配置（替换为你的 DeepSeek API Key）
static const QString LLM_API_URL = "https://api.deepseek.com/chat/completions";
static const QString LLM_API_KEY = "REVOKED-KEY-SET-DEEPSEEK_API_KEY-ENV-VAR";
static const QString LLM_API_MODEL = "deepseek-chat";
static const QString LLM_PROMPT = "生成一个与计算机领域相关的英文单词，只返回单词本身，不要有任何额外文字或标点符号。";
static const int LLM_MAX_TOKENS = 20;
static const int LLM_PREFILL_COUNT = 3;

SpaceWarWidget::SpaceWarWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::StrongFocus);



    m_playerPixmap.load(":/res/image/Space/Images/SPACE_SHIP.png");
    m_meteoritePixmap.load(":/res/image/Space/Images/SPACE_ENEMY_4.png");
    m_enemyPixmap.load(":/res/image/Space/Images/SPACE_ENEMY_0.png");
    m_enemyUp.load(":/res/image/Space/Images/SPACE_CAPTION_BACK.png");
    m_explosionPixmap.load(":/res/image/Space/Images/SPACE_EXPLOSION_0.png");
    m_bulletPixmap.load(":/res/image/Space/Images/SPACE_BOMB.png");
   
    m_scoreIcon.load(":/res/image/Space/Images/SPACE_LABEL_SCORE.png");
    m_heartIcon.load(":/res/image/Space/Images/SPACE_LABEL_LIFE.png");
    m_lifeIcon.load(":/res/image/Space/Images/SPACE_LIFE.png");
    m_timeIcon.load(":/res/image/Space/Images/SPACE_LABEL_TIME.png");

    m_mainMenuBackground.load(":/res/image/Space/Images/SPACE_MAINMENU_BG.png");
    m_gameBackground.load(":/res/image/Space/Images/SPACE_BACKGROUND.png");


    m_startBtn = new TriStateButton(":/res/image/Space/Images/SPACE_START.png", this);
    m_highScoreBtn = new TriStateButton(":/res/image/Space/Images/SPACE_HISCORE.png", this);
    m_optionsBtn = new TriStateButton(":/res/image/Space/Images/SPACE_OPTION.png", this);
    m_exitBtn = new TriStateButton(":/res/image/Space/Images/SPACE_EXIT.png", this);
    m_returnBtn = new TriStateButton(":/res/image/Space/Images/SPACE_RETURN.png", this);
    m_returnBtn->hide();

    QString hover_sound = QString("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav");
    QString click_sound = QString("qrc:/res/image/Common/Sounds/BTN_CLICK.wav");
    
    m_startBtn->setHoverSound(hover_sound);
    m_startBtn->setPressedSound(click_sound);
    m_highScoreBtn->setHoverSound(hover_sound);
    m_highScoreBtn->setPressedSound(click_sound);
    m_optionsBtn->setHoverSound(hover_sound);
    m_optionsBtn->setPressedSound(click_sound);
    m_exitBtn->setHoverSound(hover_sound);
    m_exitBtn->setPressedSound(click_sound);
    m_returnBtn->setHoverSound(hover_sound);
    m_returnBtn->setPressedSound(click_sound);

    connect(m_startBtn, &QPushButton::clicked, this, &SpaceWarWidget::onStartClicked);
    connect(m_highScoreBtn, &QPushButton::clicked, this, &SpaceWarWidget::onHighScoreClicked);
    connect(m_optionsBtn, &QPushButton::clicked, this, &SpaceWarWidget::onOptionsClicked);
    connect(m_exitBtn, &QPushButton::clicked, this, &SpaceWarWidget::onExitClicked);
    connect(m_returnBtn, &QPushButton::clicked, this, &SpaceWarWidget::onReturnClicked);

    // 游戏主循环 (33 fps)
    m_gameTimer = new QTimer(this);
    m_gameTimer->setInterval(30);
    connect(m_gameTimer, &QTimer::timeout, this, &SpaceWarWidget::gameLoop);


    m_upgradeTimer = new QTimer(this);
    m_upgradeTimer->setInterval(1000);
    connect(m_upgradeTimer, &QTimer::timeout, this, [this]() {
        if (!m_gameActive || m_gamePaused) return;
        m_upgradeTimerCount -= 1000;
        if (m_upgradeTimerCount <= 0) {
            applyUpgrade();
            m_upgradeTimerCount = m_upgradeIntervalMs;
        }
        });

    // 爆炸动画统一更新 (每秒10次)
    m_explosionCheckTimer = new QTimer(this);
    m_explosionCheckTimer->setInterval(100);
    connect(m_explosionCheckTimer, &QTimer::timeout, this, &SpaceWarWidget::onExplosionFrame);
    m_explosionCheckTimer->start();


    m_wordSpawnTimer = new QTimer(this);
    m_wordSpawnTimer->setInterval(10000);
    connect(m_wordSpawnTimer, &QTimer::timeout, this, &SpaceWarWidget::onWordSpawnTimer);


    m_playerAnimTimer = new QTimer(this);
    m_playerAnimTimer->setInterval(100);
    connect(m_playerAnimTimer, &QTimer::timeout, [this]() {
        if (m_gameActive && !m_gamePaused)
            m_playerFrame = (m_playerFrame + 1) % 11;
        });


    m_bgmPlayer = new QMediaPlayer(this);
    m_bgmPlayer->setMedia(QUrl("qrc:/res/image/Space/Sounds/SPACE_BG.mp3"));
    m_bgmPlayer->setVolume(50);
    connect(m_bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_soundEnabled) m_bgmPlayer->play();
        }
        });


    m_shootSound = new QSoundEffect(this);
    m_shootSound->setSource(QUrl("qrc:/res/image/Space/Sounds/SPACE_SHOOT.wav"));
    m_explosionSound = new QSoundEffect(this);
    m_explosionSound->setSource(QUrl("qrc:/res/image/Space/Sounds/SPACE_BLAST.wav"));
    m_generateSound = new QSoundEffect(this);
    m_generateSound->setSource(QUrl("qrc:/res/image/Space/Sounds/SPACE_PLANEOUT.wav"));
    m_bonusSound = new QSoundEffect(this);
    m_bonusSound->setSource(QUrl("qrc:/res/image/Space/Sounds/SPACE_WORDOUT.wav"));


    m_scoreFilePath = ":/data/data/space_highscores.txt";

    // 初始位置将在 resizeEvent 中设置
    m_playerPos = QPointF(width() / 2.0, height() * 0.85);

    m_networkManager = new QNetworkAccessManager(this);
}

SpaceWarWidget::~SpaceWarWidget()
{
    if (m_highScoreDialog) {
        m_highScoreDialog->saveScores();
    }
}

void SpaceWarWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    int w = width(), h = height();
    double btnW = w * 11.0 / 32.0, btnH = h * 5.0 / 72.0;
    double startX = w * 81 / 256;
    double y = h * 343 / 720;
    double table = h / 144.0;
    m_startBtn->setFixedSize(btnW, btnH);
    m_startBtn->move(startX, y);
    m_highScoreBtn->setFixedSize(btnW, btnH);
    m_highScoreBtn->move(startX, y + btnH + table);
    m_optionsBtn->setFixedSize(btnW, btnH);
    m_optionsBtn->move(startX, y + 2 * btnH + 2 * table);
    m_exitBtn->setFixedSize(btnW, btnH);
    m_exitBtn->move(startX, y + 3 * btnH + 3 * table);

    m_returnBtn->setFixedSize(btnW, btnH);
    m_returnBtn->move(m_startBtn->pos());
    m_returnBtn->hide();

    if (!m_gameActive && m_showMainMenu) {
        m_playerPos.setY(h * 0.85);
    }
}

void SpaceWarWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    if (m_showMainMenu) {
        drawBackground(p);
    }
    else {
        drawBackground(p);
        drawHUD(p);
        drawPlayer(p);
        drawObjects(p);
        drawBullets(p);
        drawExplosions(p);
        drawRewardWords(p);
        // 游戏结束画面
        if (!m_gameActive && !m_gamePaused) {
            
            p.fillRect(rect(), QColor(0, 0, 0, 200));
            p.setPen(Qt::white);
            QFont f = font();
            f.setPointSize(48);
            p.setFont(f);
            p.drawText(rect(), Qt::AlignCenter, tr("GAME OVER\n按ESC退出"));
        }
    }
}


void SpaceWarWidget::drawBackground(QPainter& p) {
    QPixmap* bgToDraw = nullptr;

    if (m_showMainMenu) {
        bgToDraw = &m_mainMenuBackground;
    }
    else {
        bgToDraw = &m_gameBackground;
    }

    if (bgToDraw && !bgToDraw->isNull()) {
        p.drawPixmap(rect(), *bgToDraw);
    }
    else {
        p.fillRect(rect(), Qt::black);
    }
}

void SpaceWarWidget::drawHUD(QPainter& p) {
    int w = width();
    int h = height();


    int barHeight = h * 0.11;
    QColor bgColor(0, 0, 0, 150);
    p.fillRect(0, 0, w, barHeight, bgColor);


    int iconS = barHeight * 0.55;
    int topY = (barHeight - iconS) / 2;

    QFont f("Arial", iconS * 0.45);
    p.setFont(f);
    p.setPen(Qt::white);


    int sectionW = w / 3;


    int scoreIconX = sectionW / 12;  // 图标在左边
    p.drawPixmap(scoreIconX, topY, 2*iconS, iconS, m_scoreIcon);
    p.drawText(scoreIconX + 2*iconS + 10, topY + iconS * 0.8, QString("%1").arg(m_score));


    
    int heartIconX = w/3 + iconS;
    p.drawPixmap(heartIconX, topY, 2*iconS, iconS, m_heartIcon);

    // 血条
    int barW = iconS * 6;
    int barH = iconS * 0.5;
    int barX = heartIconX + 2*iconS + 5;
    int barY = topY + (iconS - barH) / 2;

    p.drawPixmap(barX, barY, barW, barH, m_lifeIcon);
    p.drawRect(barX, barY, barW, barH);
    double ratio = (double)m_lives / m_maxLives;
    p.fillRect(barX + 1, barY + 1, (barW - 2) * ratio, barH - 2, Qt::green);


    int timeIconX = w - sectionW / 2 - iconS * 3;
    p.drawPixmap(timeIconX, topY, iconS*2, iconS, m_timeIcon);
    int secs = m_upgradeTimerCount / 1000;
    p.drawText(timeIconX + iconS*2 + 5, topY + iconS * 0.8, QString(" %1 s").arg(secs));


    if (m_upgradedFlag) {
        p.setPen(Qt::blue);
        QFont smallF("Arial", iconS * 0.38, QFont::Bold);
        p.setFont(smallF);
        p.drawText(timeIconX + iconS*2 + 5, topY + iconS *1.4, tr("游戏已升级！"));
    }
}

void SpaceWarWidget::drawPlayer(QPainter& p) {
    if (m_playerPixmap.isNull()) return;
    int w = m_playerPixmap.width() / 3;
    int h = m_playerPixmap.height() / 4;
    int frame = m_playerFrame % 11;
    int row = frame / 3;
    int col = frame % 3;
    QRect src(col * w, row * h, w, h);
    QRect target(m_playerPos.x() - width() * 0.1 / 2, m_playerPos.y() - height() * 0.15 / 2, width()*0.1, height()*0.15);
    p.drawPixmap(target, m_playerPixmap, src);
}

void SpaceWarWidget::drawObjects(QPainter& p) {
    for (const auto& obj : m_objects) {
        if (obj.exploded) continue;
        QPixmap* pm = nullptr;
        int rows = 1, cols = 1, totalFrames = 1;
        if (obj.type == Enemy) {
            pm = &m_enemyPixmap;
            rows = 4; cols = 3; totalFrames = 11;
         
        }
        else {
            pm = &m_meteoritePixmap;
            rows = 4; cols = 3; totalFrames = 11;

        }

        if (pm->isNull()) continue;
        int fw = pm->width() / cols;
        int fh = pm->height() / rows;
        int frame = obj.frame % totalFrames;
        int row = frame / cols;
        int col = frame % cols;
        QRect src(col * fw, row * fh, fw, fh);
        QRect target(obj.pos.x() - width() * 0.08 / 2, obj.pos.y() - height() * 0.12 / 2, width() * 0.08, height() * 0.12);
        p.drawPixmap(target, *pm, src);


        QRect target_up(obj.pos.x() - width() * 0.08 / 2 + 17.0 / 65.0 * 0.08 * width(), obj.pos.y() - height() * 0.12 / 2, width() * 0.08 * (32.0 / 65.0), height() * 0.12 * (15.0 / 55.0));
        p.drawPixmap(target_up, m_enemyUp);

        p.setPen(Qt::green);
        QFont f("Arial", 20, QFont::Bold);
        p.setFont(f);
        p.drawText(target_up, Qt::AlignCenter, obj.letter);
       
    }
}

void SpaceWarWidget::drawBullets(QPainter& p) {
    if (m_bulletPixmap.isNull()) return;
    for (const auto& bullet : m_bullets) {
        if (!bullet.active) continue;
        p.drawPixmap(QRect(bullet.pos.x() - 8, bullet.pos.y() - 8, 16, 16), m_bulletPixmap);
    }
}

void SpaceWarWidget::drawExplosions(QPainter& p) {
    if (m_explosionPixmap.isNull()) return;
    for (const auto& exp : m_explosions) {
        if (!exp.active) continue;
        int fw = m_explosionPixmap.width() / 3;
        int fh = m_explosionPixmap.height() / 3;
        int frame = exp.frame % 9;
        int row = frame / 3;
        int col = frame % 3;
        QRect src(col * fw, row * fh, fw, fh);
        p.drawPixmap(QRect(exp.pos.x() - width() * 0.08 / 2, exp.pos.y() - height() * 0.12 / 2, width() * 0.08, height() * 0.12), m_explosionPixmap, src);
    }
}

void SpaceWarWidget::drawRewardWords(QPainter& p) {
    QFont font = p.font();
    int w = width() * 0.03;
    font.setPixelSize(w);
    for (const auto& rw : m_rewardWords) {
        if (rw.finished) continue;
        for (int i = 0; i < rw.text.size(); ++i) {
            p.setPen(i < rw.currentIndex ? Qt::red : Qt::blue);

            p.setFont(font);

            p.drawText(rw.pos.x() + i * w * 0.8, rw.pos.y(), QString(rw.text.at(i)));
        }
    }
}


void SpaceWarWidget::initGame() {
    m_objects.clear();
    m_bullets.clear();
    m_explosions.clear();
    m_rewardWords.clear();
    m_usedLetters.clear();
    m_llmWordPool.clear();
    m_llmPendingRequests = 0;
    m_score = 0;
    m_lives = m_maxLives;
    m_playerPos = QPointF(width() / 2.0, height() * 0.85);
    m_playerFrame = 0;
    m_upgradedFlag = false;
    m_upgradeTimerCount = m_upgradeIntervalMs;
    m_gameActive = false;
    m_gamePaused = false;
}

void SpaceWarWidget::stopGame() {

    m_gameTimer->stop();
    m_upgradeTimer->stop();
    m_wordSpawnTimer->stop();
    m_playerAnimTimer->stop();

    m_bgmPlayer->stop();


    initGame();


    m_showMainMenu = true;
    m_startBtn->show();
    m_highScoreBtn->show();
    m_optionsBtn->show();
    m_exitBtn->show();
    m_returnBtn->hide();

    update();
}

void SpaceWarWidget::gameLoop() {
    if (!m_gameActive || m_gamePaused) return;
    updatePlayer();
    spawnObject();
    updateObjects();
    updateBullets();
    updateRewardWords();
    checkCollisions();
    checkEndGame();
    update();
    if (m_upgradedFlag && m_upgradeShowTimer.elapsed() > 2000) {
        m_upgradedFlag = false;
    }
}

void SpaceWarWidget::updatePlayer() {
    int step = m_playerSpeed;
    int halfW = width() * 0.05;
    if (m_playerRightDir) {
        m_playerPos.rx() += step;
        if (m_playerPos.x() + halfW >= width()) m_playerRightDir = false;
    }
    else {
        m_playerPos.rx() -= step;
        if (m_playerPos.x() - halfW <= 0) m_playerRightDir = true;
    }
}

void SpaceWarWidget::spawnObject() {
    int maxEnemy = qMin(m_enemyMaxCount, 10);
    int maxMeteor = qMin(m_meteoriteMaxCount, 5);

    int enemyCount = 0, meteorCount = 0;
    for (const auto& obj : m_objects) {
        if (obj.type == Enemy) ++enemyCount;
        else ++meteorCount;
    }

    bool spawnEnemy = (enemyCount < maxEnemy && QRandomGenerator::global()->bounded(100) < 3);
    bool spawnMeteor = (meteorCount < maxMeteor && QRandomGenerator::global()->bounded(100) < 2);

    if (!spawnEnemy && !spawnMeteor) return;

    QChar letter = getUnusedLetter();
    if (letter.isNull()) return;

    if (spawnEnemy) {
        GameObject obj;
        obj.type = Enemy;
        obj.letter = letter;

        double leftXs[] = { 0.0, width() / 6.0, width() / 3.0 };
        int idx = QRandomGenerator::global()->bounded(3);
        bool leftSide = QRandomGenerator::global()->bounded(2) == 0;
        double startX = leftSide ? leftXs[idx] : width() - leftXs[idx];

        obj.pos = QPointF(startX, -30);
        obj.initialX = startX;
        obj.targetX = leftSide ? (startX + width() / 6.0) : (startX - width() / 6.0);
        obj.phaseOneDone = false;
        obj.oscillationTime = 0.0;

        double baseSpeed = SPAWN_BASE_SPEED + m_speedLevel * SPAWN_SPEED_MULTIPLIER;
        obj.fallSpeed = baseSpeed;
        obj.horizontalSpeed = leftSide ? baseSpeed : -baseSpeed;

        obj.frame = 0;
        obj.frameCount = 1;
        obj.exploded = false;
        obj.toBeRemoved = false;

        m_objects.append(obj);
        GameObject* objPtr = &m_objects.last();

        objPtr->animTimer = new QTimer(this);
        connect(objPtr->animTimer, &QTimer::timeout, this, [this, objPtr]() {
            if (objPtr->frameCount > 0)
                objPtr->frame = (objPtr->frame + 1) % objPtr->frameCount;
            });
        objPtr->animTimer->start(100);
        m_usedLetters.insert(letter);

    }
    else if (spawnMeteor) {
        GameObject obj;
        obj.type = Meteorite;
        obj.letter = letter;
        obj.pos = QPointF(QRandomGenerator::global()->bounded(width() - 60) + 30, -30);
        obj.frame = 0;
        obj.frameCount = 12;
        obj.fallSpeed = METEORITE_BASE_SPEED + m_speedLevel * METEORITE_SPEED_MULTIPLIER;
        obj.horizontalSpeed = 0;

        m_objects.append(obj);
        GameObject* objPtr = &m_objects.last();

        objPtr->animTimer = new QTimer(this);
        connect(objPtr->animTimer, &QTimer::timeout, this, [this, objPtr]() {
            if (objPtr->frameCount > 0)
                objPtr->frame = (objPtr->frame + 1) % objPtr->frameCount;
            });
        objPtr->animTimer->start(80);
        m_usedLetters.insert(letter);
    }

    if (m_generateSound->isLoaded())m_generateSound->play();
}

QChar SpaceWarWidget::getUnusedLetter() {
    QVector<QChar> unused;
    for (char c = 'A'; c <= 'Z'; ++c) {
        QChar ch(c);
        if (!m_usedLetters.contains(ch)) unused.append(ch);
    }
    if(unused.isEmpty())return QChar();

    int randomIndex = QRandomGenerator::global()->bounded(unused.size());

    return unused[randomIndex];
}

void SpaceWarWidget::releaseLetter(QChar ch) {
    m_usedLetters.remove(ch);
}

void SpaceWarWidget::updateObjects() {
    for (auto& obj : m_objects) {
        if (obj.exploded) continue;

        if (obj.type == Enemy) {
            if (!obj.phaseOneDone) {
                obj.pos.rx() += obj.horizontalSpeed;
                obj.pos.ry() += obj.fallSpeed;

                if ((obj.horizontalSpeed > 0 && obj.pos.x() >= obj.targetX) ||
                    (obj.horizontalSpeed < 0 && obj.pos.x() <= obj.targetX)) {
                    obj.pos.setX(obj.targetX);
                    obj.phaseOneDone = true;
                    obj.oscillationCenterX = obj.targetX;
                }
            }

            else {
                obj.oscillationTime += OSC_TIME_STEP;
                double oscAmplitude = OSC_AMPLITUDE;
                double oscOmega = OSC_OMEGA;
                double offsetX = oscAmplitude * sin(oscOmega * obj.oscillationTime);
                obj.pos.setX(obj.oscillationCenterX + offsetX);
                obj.pos.ry() += obj.fallSpeed;
            }
        }
        else {
            obj.pos.ry() += obj.fallSpeed;
        }

        if (obj.pos.y() > height() + 50) {
            obj.toBeRemoved = true;

            releaseLetter(obj.letter);
        }
    }

    for (int i = m_objects.size() - 1; i >= 0; --i) {
        if (m_objects[i].toBeRemoved) {
            if (m_objects[i].animTimer) {
                m_objects[i].animTimer->stop();
                m_objects[i].animTimer->deleteLater();
            }
            m_objects.removeAt(i);
        }
    }
}

void SpaceWarWidget::updateBullets() {
    for (auto& bullet : m_bullets) {
        if (!bullet.active) continue;


        GameObject* target = nullptr;
        for (auto& obj : m_objects) {
            if (!obj.exploded && obj.letter == bullet.targetLetter) {
                target = &obj;
                break;
            }
        }
        if (target) bullet.targetPos = target->pos;


        double dx = bullet.targetPos.x() - bullet.pos.x();
        double dy = bullet.targetPos.y() - bullet.pos.y();
        double desiredAngle = atan2(dy, dx);


        double diff = desiredAngle - bullet.currentAngle;  // ← 用存储的角度
        while (diff > PI) diff -= 2 * PI;
        while (diff < -PI) diff += 2 * PI;

        // 限制转向速率
        
        bullet.currentAngle += qBound(-turnRate, diff, turnRate);  // ← 更新存储的角度


        constexpr double bulletSpeed = 40.0;
        bullet.pos.rx() += bulletSpeed * cos(bullet.currentAngle);
        bullet.pos.ry() += bulletSpeed * sin(bullet.currentAngle);


        if (!target && fabs(bullet.pos.x() - bullet.targetPos.x()) < 5 &&
            fabs(bullet.pos.y() - bullet.targetPos.y()) < 5) {
            bullet.active = false;
        }

        if (bullet.pos.x() < -20 || bullet.pos.x() > width() + 20 ||
            bullet.pos.y() > height() + 20 || bullet.pos.y() < -20)
            bullet.active = false;
    }

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
        [](const Bullet& b) { return !b.active; }),
        m_bullets.end());
}

void SpaceWarWidget::updateRewardWords() {
    for (auto& rw : m_rewardWords) {
        if (rw.finished) continue;
        rw.pos.rx() -= rw.speedX;
        if (rw.pos.x() + rw.text.size() * 15 < 0) {
            rw.finished = true;
        }
    }
    m_rewardWords.erase(std::remove_if(m_rewardWords.begin(), m_rewardWords.end(),
        [](const RewardWord& rw) { return rw.finished; }),
        m_rewardWords.end());
}

void SpaceWarWidget::checkCollisions() {
    QRect playerRect = getPlayerRect();
    for (auto& obj : m_objects) {
        if (obj.exploded) continue;
        QRect objRect = getObjectRect(obj);
        for (auto& bullet : m_bullets) {
            if (!bullet.active || bullet.targetLetter != obj.letter) continue;
            if (objRect.contains(QRect(bullet.pos.x() - 8, bullet.pos.y() - 8, 16, 16))) {
                explodeObject(&obj);
                bullet.active = false;
                m_score += 1500;
                if (m_explosionSound->isLoaded()) m_explosionSound->play();
                break;
            }
        }
        if (playerRect.intersects(objRect)) {
            explodeObject(&obj);
            m_lives--;

            
            if (m_lives <= 0) {
                checkEndGame();
            }
        }
    }
}

void SpaceWarWidget::explodeObject(GameObject* obj) {
    if (!obj || obj->exploded) return;
    obj->exploded = true;
    obj->toBeRemoved = true;
    releaseLetter(obj->letter);
    if (obj->animTimer) obj->animTimer->stop();

    Explosion exp;
    exp.pos = obj->pos;
    exp.frame = 0;
    exp.active = true;
    m_explosions.append(exp);
}

void SpaceWarWidget::onExplosionFrame() {
    for (int i = m_explosions.size() - 1; i >= 0; --i) {
        auto& exp = m_explosions[i];
        if (!exp.active) continue;
        exp.frame++;
        if (exp.frame >= 9) {
            exp.active = false;
            m_explosions.removeAt(i);
        }
    }
}

void SpaceWarWidget::checkEndGame() {
    if (m_lives <= 0 && m_gameActive) {
        m_gameActive = false;
        m_gameTimer->stop();
        m_upgradeTimer->stop();
        m_wordSpawnTimer->stop();
        m_playerAnimTimer->stop();
        
    }
}

void SpaceWarWidget::handleGameOver() {
    QTimer::singleShot(GAME_OVER_DELAY_MS, this, [this]() {
       
        if (m_score > 0) {
            NameInputDialog nameDlg(this);
            if (nameDlg.exec() == QDialog::Accepted) {
                QString name = nameDlg.name();
                if (!name.isEmpty()) {
                    if (!m_highScoreDialog) {
                        m_highScoreDialog = new HighScoreDialog(m_scoreFilePath, this);
                        m_highScoreDialog->loadScores();
                    }
                    m_highScoreDialog->addScore(name, m_score);
                }
            }
        }

        m_showMainMenu = true;
        m_startBtn->show(); m_highScoreBtn->show(); m_optionsBtn->show(); m_exitBtn->show();
        m_returnBtn->hide();
        update();
        });
}

void SpaceWarWidget::applyUpgrade() {
    if (m_speedLevel < 10) {
        m_speedLevel++;
        m_enemyMaxCount = qMin(m_enemyMaxCount + 1, 10);
        m_meteoriteMaxCount = m_enemyMaxCount / 2;
        m_upgradedFlag = true;
        m_upgradeShowTimer.start();
    }
}

void SpaceWarWidget::handleLetterInput(QChar letter) {
    
    for (auto& rw : m_rewardWords) {
        if (rw.finished) continue;
        if (rw.currentIndex < rw.text.size() && rw.text.at(rw.currentIndex).toUpper() == letter) {
            rw.currentIndex++;
            if (rw.currentIndex == rw.text.size()) {
                rw.finished = true;
                m_lives = m_maxLives;
               
            } 
            return;
        }
    }
    for (auto& obj : m_objects) {
        if (!obj.exploded && obj.letter == letter) {
            Bullet bullet;
            bullet.targetLetter = letter;
            bullet.pos = QPointF(m_playerPos.x(), m_playerPos.y() - 30);
            bullet.targetPos = obj.pos;
            bullet.active = true;

            double dx = obj.pos.x() - bullet.pos.x();
            double dy = obj.pos.y() - bullet.pos.y();
            bullet.currentAngle = atan2(dy, dx);

            m_bullets.append(bullet);
            if (m_shootSound->isLoaded()) m_shootSound->play();
            
            return;
        }
    }
    
        m_score -= 400;
    
}

void SpaceWarWidget::onWordSpawnTimer() {
    if (!m_gameActive || m_gamePaused || !m_rewardEnabled) return;
    spawnRewardWord();
}

void SpaceWarWidget::spawnRewardWord() {
    if ( !m_rewardEnabled) return;
    if (m_rewardWords.size() >= MAX_REWARD_WORDS) return;

    
    QString word = generateRewardWord();

    RewardWord rw;
    rw.text = word;
    rw.currentIndex = 0;
    rw.pos = QPointF(width() , height() * 0.3);
    rw.speedX = DEFAULT_REWARD_WORD_SPEED;
    rw.finished = false;
    m_rewardWords.append(rw);
    if (m_bonusSound->isLoaded()) m_bonusSound->play();
}

QString SpaceWarWidget::getLineWord(const QString& resourcePath) {
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    QString result;
    int lineCount = 0;

    while (!in.atEnd()) {
        QString currentLine = in.readLine();

        if (currentLine.trimmed().isEmpty())continue;

        lineCount++;

        if (QRandomGenerator::global()->bounded(100) == 0) {
            result = currentLine.trimmed();
            break;
        }

    }
    file.close();
    return result;
}

QString SpaceWarWidget::generateRewardWord() {
    // 优先从 LLM 单词池取，消耗后立即异步补充
    if (!m_llmWordPool.isEmpty()) {
        QString word = m_llmWordPool.takeFirst();
        requestLLMWord();
		qDebug() << "从 LLM 单词池获取单词：" << word;
        return word.toUpper();
    }

    QString word = getLineWord(":/data/data/word.txt");
    if (word.isEmpty()) {
        qDebug() << "使用备用单词列表";
        static QStringList backupWords = {
            "ALGORITHM", "DEBUG", "COMPILE", "RUNTIME", "VARIABLE",
            "FUNCTION", "OBJECT", "CLASS", "INHERIT", "POLYMORPHISM"
        };
        int randomIndex = QRandomGenerator::global()->bounded(backupWords.size());
        word = backupWords[randomIndex];
    }
    return word.toUpper();
}

void SpaceWarWidget::prefillLLMPool() {
    for (int i = 0; i < LLM_PREFILL_COUNT; ++i)
        requestLLMWord();
}

void SpaceWarWidget::requestLLMWord() {
    if (m_llmPendingRequests >= LLM_PREFILL_COUNT)
        return;

    QJsonObject msg;
    msg["role"] = "user";
    msg["content"] = LLM_PROMPT;
    QJsonArray messages;
    messages.append(msg);
    QJsonObject body;
    body["model"] = LLM_API_MODEL;
    body["messages"] = messages;
    body["max_tokens"] = LLM_MAX_TOKENS;

    QUrl url(LLM_API_URL);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + LLM_API_KEY).toUtf8());

    QByteArray postData = QJsonDocument(body).toJson();
    QNetworkReply* reply = m_networkManager->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onLLMWordReceived(reply);
    });
    m_llmPendingRequests++;
}

void SpaceWarWidget::onLLMWordReceived(QNetworkReply* reply) {
    reply->deleteLater();
    m_llmPendingRequests--;

    if (reply->error() != QNetworkReply::NoError)
        return;

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray choices = doc.object()["choices"].toArray();
    if (choices.isEmpty())
        return;

    QString word = choices[0].toObject()["message"].toObject()["content"].toString()
                       .trimmed().toUpper();

    // 校验：仅包含英文字母且长度合理
    if (word.isEmpty() || word.length() < 2)
        return;
    for (const QChar& ch : word) {
        if (!ch.isLetter() || ch.toLatin1() < 'A' || ch.toLatin1() > 'Z')
            return;
    }

    m_llmWordPool.append(word);
}


void SpaceWarWidget::pauseGame() {
    if (!m_gameActive || m_gamePaused) return;
    m_gamePaused = true;
    m_gameTimer->stop();
    m_upgradeTimer->stop();
    m_wordSpawnTimer->stop();
    m_playerAnimTimer->stop();

    if (m_soundEnabled) m_bgmPlayer->pause();

    m_showMainMenu = true;
    m_startBtn->hide();
    m_highScoreBtn->show();
    m_optionsBtn->show();
    m_exitBtn->show();
    m_returnBtn->show();

    update();
}

void SpaceWarWidget::resumeGame() {
    if (!m_gameActive || !m_gamePaused) return;
    m_gamePaused = false;
    m_gameTimer->start();
    m_upgradeTimer->start();
    if (m_rewardEnabled) m_wordSpawnTimer->start();
    m_playerAnimTimer->start();

    if (m_soundEnabled) m_bgmPlayer->play();

    m_showMainMenu = false;
    m_returnBtn->hide();
    m_startBtn->hide();
    m_highScoreBtn->hide();
    m_optionsBtn->hide();
    m_exitBtn->hide();

    setFocus();
    update();
}


void SpaceWarWidget::onStartClicked() {
    if (m_gamePaused) return;
    initGame();
    prefillLLMPool();
    m_showMainMenu = false;
    m_startBtn->hide(); m_highScoreBtn->hide(); m_optionsBtn->hide(); m_exitBtn->hide();
    m_returnBtn->hide();
    m_gameActive = true;
    m_gamePaused = false;
    m_upgradeTimerCount = m_upgradeIntervalMs;
    m_gameTimer->start();
    m_upgradeTimer->start();
    if (m_rewardEnabled) m_wordSpawnTimer->start();
    m_playerAnimTimer->start();


    if (m_soundEnabled && m_bgmPlayer->state() != QMediaPlayer::PlayingState) {
        m_bgmPlayer->play();
    }
    setFocus();
}

void SpaceWarWidget::onHighScoreClicked() {
    if (!m_highScoreDialog) {
        m_highScoreDialog = new HighScoreDialog(m_scoreFilePath, this);
        m_highScoreDialog->loadScores();
    }

   
    m_highScoreDialog->exec();
}

void SpaceWarWidget::onOptionsClicked() {
    if (!m_settingsDialog) {
        m_settingsDialog = new SpaceSettingsDialog(this);
        connect(m_settingsDialog, &SpaceSettingsDialog::settingsApplied, this,
            [this](int enemyMax, int speed, int intervalSec, bool rewardOn) {
                m_enemyMaxCount = enemyMax;
                m_meteoriteMaxCount = enemyMax / 2;
                m_speedLevel = speed;
                m_upgradeIntervalMs = intervalSec * 1000;
                m_rewardEnabled = rewardOn;
                if (m_gameActive) {
                    stopGame();
                    
                    m_gameActive = true;
                    m_gameTimer->start();
                    m_upgradeTimer->start();
                    if (m_rewardEnabled) m_wordSpawnTimer->start();
                    m_playerAnimTimer->start();
                }
            });
    }
    m_settingsDialog->setCurrentValues(m_enemyMaxCount, m_speedLevel, m_upgradeIntervalMs / 1000, m_rewardEnabled);
    m_settingsDialog->exec();
}

void SpaceWarWidget::onExitClicked() {
    if (m_gameActive) pauseGame();
    ExitConfirmDialog dlg(this,
        ":/res/image/Common/Images/MAIN_DLG_BG.png",
        ":/res/image/Common/Images/MAIN_DLG_EXIT.png",
        ":/res/image/Common/Images/MAIN_DLG_REPLAY.png",
        tr("你真的要退出吗？"));
    dlg.exec();
    if (dlg.isConfirmed()) {
        close();
    }
    else {
        if (m_gamePaused) resumeGame();
    }
}


void SpaceWarWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        if (m_gameActive && !m_gamePaused) {
            pauseGame();
        }
        else if (m_gamePaused) {
            resumeGame();
        }
        else if (!m_gameActive && !m_showMainMenu) {
            handleGameOver();
            
            update();
        }
        else if (m_showMainMenu) {
            onExitClicked();
        }
        return;
    }
    if (m_gameActive && !m_gamePaused) {
        QString text = event->text();
        if (!text.isEmpty()) {
            QChar ch = text.at(0);
            if (ch.isLetter()) handleLetterInput(ch.toUpper());
        }
    }
}

void SpaceWarWidget::closeEvent(QCloseEvent* event) {

    if (m_bgmPlayer && m_bgmPlayer->state() == QMediaPlayer::PlayingState) {
        m_bgmPlayer->stop();
    }

    if (m_highScoreDialog) m_highScoreDialog->saveScores();
    event->accept();
}

QRect SpaceWarWidget::getPlayerRect() const {
    int w = width()*0.1, h = height()*0.15;
    return QRect(m_playerPos.x() - w / 2, m_playerPos.y() - h / 2, w, h);
}

QRect SpaceWarWidget::getObjectRect(const GameObject& obj) const {
    int w = width() * 0.06, h = height() * 0.09;
    return QRect(obj.pos.x() - w / 2, obj.pos.y() - h / 2, w, h);
}