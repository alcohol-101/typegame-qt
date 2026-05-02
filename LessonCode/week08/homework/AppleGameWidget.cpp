#include "applegamewidget.h"



#include "settingsdialog.h"
#include "exitconfirmdialog.h"


// ========== constexpr 常量 ==========
constexpr double APPLE_WIDTH_RATIO = 200.0 / 2560.0;
constexpr double APPLE_HEIGHT_RATIO = 200.0 / 1440.0;
constexpr double SMALL_APPLE_WIDTH_RATIO = 110.0 / 2560.0;
constexpr double SMALL_APPLE_HEIGHT_RATIO = 120.0 / 1440.0;
constexpr double EXIT_BTN_WIDTH_RATIO = 300.0 / 2560.0;
constexpr double EXIT_BTN_HEIGHT_RATIO = 128.0 / 1440.0;
constexpr int APPLE_SPAWN_RATE = 5;
constexpr int SPAWN_PROBABILITY = 100;
constexpr double BOTTOM_THRESHOLD_RATIO = 7.0 / 10.0;
constexpr double SPEED_FACTOR_BASE = 0.2;
constexpr double SPEED_FACTOR_RANGE = 1.8;
constexpr double ALPHABET_SIZE = 25.0;
constexpr double BASE_SPEED_MIN = 2.0;
constexpr double BASE_SPEED_MULTIPLIER = 1.5;
constexpr int BAD_APPLE_DURATION_MS = 500;
constexpr double BASKET_WIDTH_RATIO = 360.0 / 2560.0;
constexpr double BASKET_HEIGHT_RATIO = 320.0 / 1440.0;
constexpr double BASKET_MARGIN_RATIO = 80.0 / 2560.0;
constexpr int VERTICAL_OVERLAP_DIVISOR = 2;
constexpr int BASKET_INNER_MARGIN = 40;
constexpr int MIN_STEP = 1;
constexpr int MAX_SMALL_APPLES = 8;
constexpr int STEP_DIVISOR = 8;
constexpr int TARGET_OFFSET = 10;
// =====================================

AppleGameWidget::AppleGameWidget(QWidget* parent)
    : QWidget(parent)
    , m_speedLevel(1)
    , m_targetAppleCount(100)
    , m_maxBadAppleCount(10)
    , m_maxAppleCount(10)
    , m_gameActive(false)
    , m_gamePaused(false)
    , m_successCount(0)
    , m_badCount(0)
    , m_smallAppleCount(0)
    , m_updateTimer(new QTimer(this))
    , m_settingsDialog(nullptr)
    , m_showGameElements(false)
    , m_soundEnabled(true)
{
    // 设置无边框全屏
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::StrongFocus);  // 接收键盘事件

    // 加载图片资源
    m_normalApplePixmap.load(":/res/image/Apple/Images/APPLE_NORMAL.png");
    m_badApplePixmap.load(":/res/image/Apple/Images/APPLE_BAD.png");
    m_basketPixmap.load(":/res/image/Apple/Images/APPLE_BASKET.png");
    m_smallApplePixmap.load(":/res/image/Apple/Images/APPLE_SMALL.png");
    // 背景图
    m_backgroundPixmap.load(":/res/image/Apple/Images/APPLE_BACKGROUND.png");

   

    // 设置苹果绘制大小
    m_appleSize = QSize(APPLE_WIDTH_RATIO * w_primary, APPLE_HEIGHT_RATIO * h_primary);
    m_smallAppleSize = QSize(SMALL_APPLE_WIDTH_RATIO * w_primary, SMALL_APPLE_HEIGHT_RATIO * h_primary);

    // 创建底部控制按钮区域
    QWidget* controlWidget = new QWidget(this);
    controlWidget->setStyleSheet("background-color: rgba(0,0,0,0);"); // 透明
    QHBoxLayout* controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setContentsMargins(20, 10, 20, 20);
    controlLayout->setSpacing(15);

    // 退出按钮（左下角）
    m_exitBtn = new QPushButton(controlWidget);
    m_exitBtn->setFixedSize(EXIT_BTN_WIDTH_RATIO * w_primary, EXIT_BTN_HEIGHT_RATIO * h_primary);
    m_exitBtn->setStyleSheet(
        "QPushButton {"
        "   border-image: url(:/res/image/Common/Images/PUBLIC_EXIT.png)0 188 0 0;"
        "   color: white;"
        "   font-size: 16px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(:/res/image/Common/Images/PUBLIC_EXIT.png)0 94 0 94;"
        "}"
    );
    connect(m_exitBtn, &QPushButton::clicked, this, &AppleGameWidget::onExitClicked);

    m_startBtn = new QPushButton(this);
    m_pauseBtn = new QPushButton(this);
    m_stopBtn = new QPushButton(this);
    m_settingsBtn = new QPushButton(this);

    int w_window = QApplication::primaryScreen()->geometry().width();
    int h_window = QApplication::primaryScreen()->geometry().height();


    m_startBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/PUBLIC_START.png) 0 96 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/PUBLIC_START.png) 0 48 0 48;
        }
    )");
    m_pauseBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/PUBLIC_PAUSE.png) 0 78 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/PUBLIC_PAUSE.png) 0 39 0 39;
        }
    )");
    m_stopBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/PUBLIC_END.png) 0 82 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/PUBLIC_END.png) 0 41 0 41;
        }
    )");
    m_settingsBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/PUBLIC_SETUP.png) 0 78 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/PUBLIC_SETUP.png) 0 39 0 39;
        }
    )");



    controlLayout->addWidget(m_exitBtn, 0, Qt::AlignLeft | Qt::AlignBottom);

    controlLayout->addStretch();

    // 主布局：控制栏在底部
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addStretch();
    mainLayout->addWidget(controlWidget);

    // 连接按钮信号
    connect(m_startBtn, &QPushButton::clicked, this, &AppleGameWidget::onStartClicked);
    connect(m_pauseBtn, &QPushButton::clicked, this, &AppleGameWidget::onPauseClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &AppleGameWidget::onStopClicked);
    connect(m_settingsBtn, &QPushButton::clicked, this, &AppleGameWidget::onSettingsClicked);

    // 定时器设置
    m_updateTimer->setInterval(30); // 约33fps
    connect(m_updateTimer, &QTimer::timeout, this, &AppleGameWidget::onUpdateTimer);

    // 初始化按钮状态
    m_pauseBtn->setEnabled(false);
    m_stopBtn->setEnabled(false);


    // 初始化背景音乐播放器
    m_bgmPlayer = new QMediaPlayer(this);
    m_bgmPlayer->setMedia(QUrl("qrc:/res/image/Apple/Sounds/APPLE_BG.mp3"));
    m_bgmPlayer->setVolume(50); // 音量 0-100
    // 设置循环播放（QMediaPlayer 默认不循环，通过信号实现）
    connect(m_bgmPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) 
        {
            if (status == QMediaPlayer::EndOfMedia) 
            {
                if (m_soundEnabled) m_bgmPlayer->play();
            }
        });

    // ========== 悬浮音效初始化 ==========
    m_hoverSoundEffect = new QSoundEffect(this);
    m_hoverSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav"));
    m_hoverSoundEffect->setVolume(1.0f);  // 音量 0.0 ~ 1.0

    // 初始化点击音效
    m_clickSoundEffect = new QSoundEffect(this);
    m_clickSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/BTN_CLICK.wav"));  // 点击音效文件路径
    m_clickSoundEffect->setVolume(1.0f);

    // ========== 消除苹果成功音效初始化 ==========
    m_successSoundEffect = new QSoundEffect(this);
    m_successSoundEffect->setSource(QUrl("qrc:/res/image/Apple/Sounds/APPLE_IN.wav"));  // 根据实际路径修改
    m_successSoundEffect->setVolume(1.0f);

    // 安装事件过滤器到自身，这样会监听本窗口及所有子控件的事件
    m_exitBtn->installEventFilter(this);
    m_startBtn->installEventFilter(this);
    m_pauseBtn->installEventFilter(this);
    m_stopBtn->installEventFilter(this);
    m_settingsBtn->installEventFilter(this);
}

AppleGameWidget::~AppleGameWidget()
{
    if (m_bgmPlayer) {
        m_bgmPlayer->stop();
        delete m_bgmPlayer;
        m_bgmPlayer = nullptr;
    }

    if (m_settingsDialog)
        delete m_settingsDialog;
}

void AppleGameWidget::startGame()
{
    resetGameState();
    m_gameActive = true;
    m_gamePaused = false;
    m_showGameElements = true;
    m_updateTimer->start();

    m_startBtn->setEnabled(false);
    m_pauseBtn->setEnabled(true);
    m_stopBtn->setEnabled(true);
    m_settingsBtn->setEnabled(true);

    setFocus(); // 确保接收键盘事件

    // 播放背景音乐
    if (m_soundEnabled && m_bgmPlayer->state() != QMediaPlayer::PlayingState) {
        m_bgmPlayer->play();
    }
}

void AppleGameWidget::pauseGame()
{
    if (!m_gameActive || m_gamePaused)
        return;
    m_gamePaused = true;
    m_showGameElements = false;
    m_updateTimer->stop();

    // 暂停背景音乐
    if (m_soundEnabled) m_bgmPlayer->pause();

    update();
}

void AppleGameWidget::resumeGame()
{
    if (!m_gameActive || !m_gamePaused)
        return;
    m_gamePaused = false;
    m_showGameElements = true;
    m_updateTimer->start();

    // 恢复背景音乐
    if (m_soundEnabled) m_bgmPlayer->play();

    update();
}

void AppleGameWidget::stopGame()
{
    m_gameActive = false;
    m_gamePaused = false;
    m_showGameElements = false;
    m_updateTimer->stop();

    m_startBtn->setEnabled(true);
    m_pauseBtn->setEnabled(false);

    m_stopBtn->setEnabled(false);
    m_settingsBtn->setEnabled(true);

    resetGameState(); // 清除所有苹果，重置计数

    // 停止并重置背景音乐到开头
    m_bgmPlayer->stop();

    update();
}

void AppleGameWidget::showSettings()
{
    pauseGame();
    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(this);
        connect(m_settingsDialog, &SettingsDialog::settingsApplied,
            this, &AppleGameWidget::applySettings);
    }
    m_settingsDialog->setCurrentValues(m_speedLevel, m_targetAppleCount, m_maxBadAppleCount, m_maxAppleCount, m_soundEnabled);
    m_settingsDialog->exec();

    resumeGame();
}

void AppleGameWidget::onStartClicked()
{
    startGame();
}

void AppleGameWidget::onPauseClicked()
{
    if (m_gamePaused)
        resumeGame();
    else
        pauseGame();
}

void AppleGameWidget::onStopClicked()
{
    stopGame();
}

void AppleGameWidget::onSettingsClicked()
{
    showSettings();
}

void AppleGameWidget::onExitClicked()
{
    pauseGame();           // 暂停

    // 创建自定义按钮
    ExitConfirmDialog dialog(this, ":/res/image/Common/Images/MAIN_DLG_BG.png", ":/res/image/Common/Images/MAIN_DLG_EXIT.png",
        ":/res/image/Common/Images/MAIN_DLG_REPLAY.png", tr("你真的要退出吗？"));
    dialog.exec();  // 模态显示

    if (dialog.isConfirmed()) {
        close();  // 关闭游戏窗口
        m_bgmPlayer->stop();//关闭音乐
    }
    // 否则什么都不做，继续游戏
    resumeGame();

}

void AppleGameWidget::onUpdateTimer()
{
    if (!m_gameActive || m_gamePaused)
        return;

    // 随机生成新苹果（概率控制）
    if ((m_random.bounded(SPAWN_PROBABILITY) < APPLE_SPAWN_RATE || m_apples.size() < m_random.bounded(1 + m_random.bounded(m_speedLevel) / 2)) && m_apples.size() < m_maxAppleCount) {
        generateApple();
    }

    // 移动苹果

    qreal bottomThreshold = height() * BOTTOM_THRESHOLD_RATIO;

    for (int i = m_apples.size() - 1; i >= 0; --i) {
        Apple& apple = m_apples[i];
        if (apple.isBad)
            continue; // 坏苹果由定时器处理，不移动

        // 基于字母计算速度因子（0.2 ~ 2 倍速）
        int letterIndex = apple.letter.toLatin1() - 'A';  // A=0, B=1, ..., Z=25
        qreal speedFactor = SPEED_FACTOR_BASE + (letterIndex / ALPHABET_SIZE) * SPEED_FACTOR_RANGE;

        // 基础速度 + 字母速度变化
        qreal baseSpeed = BASE_SPEED_MIN + m_random.bounded(m_speedLevel) * BASE_SPEED_MULTIPLIER;
        qreal speed = baseSpeed * speedFactor;

        apple.pos.ry() += speed;

        // 检查是否到达底部区域且尚未变坏
        if (apple.pos.y() + m_appleSize.height() >= bottomThreshold && !apple.isBad) {
            convertAppleToBad(apple);
            // 注意：convertAppleToBad会设置isBad并启动计时器，不立即移除
        }
    }

    update(); // 触发重绘
}

void AppleGameWidget::onAppleBadTimeout()
{
    QTimer* timer = qobject_cast<QTimer*>(sender());
    if (!timer) return;

    // 查找对应的苹果并移除
    for (int i = 0; i < m_apples.size(); ++i) {
        if (m_apples[i].badTimer == timer) {
            m_apples.removeAt(i);
            m_badCount++;
            updateBasketSmallApples();
            checkGameOver();
            update();
            break;
        }
    }
    timer->deleteLater();
}

void AppleGameWidget::applySettings(int level, int targetCount, int maxBadCount, int maxAppleCount, bool soundEnabled)
{
    stopGame();
    m_speedLevel = level;
    m_targetAppleCount = targetCount;
    m_maxBadAppleCount = maxBadCount;
    m_maxAppleCount = maxAppleCount;
    m_soundEnabled = soundEnabled;
    // 如果游戏未开始，下次开始使用新参数；如果正在进行，不立即改变（按需求可以改变，这里简化）
    startGame();
}

void AppleGameWidget::generateApple()
{
    // 字母池 A-Z
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QList<QChar> usedLetters;
    for (const Apple& a : m_apples) {
        usedLetters.append(a.letter);
    }

    QList<QChar> available;
    for (QChar c : alphabet) {
        if (!usedLetters.contains(c))
            available.append(c);
    }
    if (available.isEmpty())
        return; // 字母已满，不再生成

    QChar letter = available.at(m_random.bounded(available.size()));

    Apple apple;
    apple.letter = letter;
    apple.isBad = false;
    apple.badTimer = nullptr;
    // 初始位置：x随机，y=0
    qreal x = m_random.bounded(width() - m_appleSize.width());
    apple.pos = QPointF(x, 0);

    m_apples.append(apple);
}

void AppleGameWidget::removeAppleByLetter(QChar letter)
{
    QChar upper = letter.toUpper();
    for (int i = 0; i < m_apples.size(); ++i) {
        if (m_apples[i].letter == upper && !m_apples[i].isBad) {
            m_apples.removeAt(i);
            m_successCount++;

            // ========== 播放成功音效 ==========
            if (m_soundEnabled && m_successSoundEffect && m_successSoundEffect->isLoaded()) {
                m_successSoundEffect->play();
            }

            updateBasketSmallApples();
            checkGameOver();
            update();
            return;
        }
    }
}

void AppleGameWidget::convertAppleToBad(Apple& apple)
{
    if (apple.isBad) return;
    apple.isBad = true;
    apple.badTimer = new QTimer(this);
    apple.badTimer->setSingleShot(true);
    apple.badTimer->start(BAD_APPLE_DURATION_MS);
    connect(apple.badTimer, &QTimer::timeout, this, &AppleGameWidget::onAppleBadTimeout);
    // 注意：不立即增加badCount，等定时器触发再增加
}

void AppleGameWidget::updateBasketSmallApples()
{
    if (m_targetAppleCount <= 0) return;
    int step = (m_targetAppleCount - TARGET_OFFSET) / STEP_DIVISOR;
    if (step <= 0) step = MIN_STEP;
    int count = m_successCount / step;
    m_smallAppleCount = qBound(0, count, MAX_SMALL_APPLES);
}

void AppleGameWidget::resetGameState()
{
    // 清除所有苹果及其计时器
    for (Apple& a : m_apples) {
        if (a.badTimer) {
            a.badTimer->stop();
            a.badTimer->deleteLater();
        }
    }
    m_apples.clear();
    m_successCount = 0;
    m_badCount = 0;
    m_smallAppleCount = 0;
    update();
}

void AppleGameWidget::checkGameOver()
{
    if (!m_gameActive) return;

    bool victory = (m_successCount >= m_targetAppleCount);
    bool defeat = (m_badCount >= m_maxBadAppleCount);

    if (victory || defeat) {
        m_gameActive = false;
        m_gamePaused = false;
        m_showGameElements = false;
        m_updateTimer->stop();

        // 计算准确率
        int total = m_successCount + m_badCount;
        double accuracy = total > 0 ? (m_successCount * 100.0 / total) : 0.0;
        QString accuracyText = tr("准确率: %1%").arg(accuracy, 0, 'f', 1);

        QString msg;
        if (victory) {
            msg = tr("恭喜，您通过了！\n%1").arg(accuracyText);
            ExitConfirmDialog success_dialog(this,
                ":/res/image/Apple/Images/APPLE_DLG_BG.png",
                ":/res/image/Apple/Images/APPLE_DLG_REPLAY.png",
                ":/res/image/Apple/Images/APPLE_DLG_NEXT.png",
                ":/res/image/Apple/Images/APPLE_DLG_END.png",
                msg);

            success_dialog.exec();
            if (success_dialog.isConfirmed_3() == 0) {
                stopGame();
                startGame();
            }
            else if (success_dialog.isConfirmed_3() == 1) {
                if (m_speedLevel < 9) {
                    m_speedLevel++;
                }
                if (m_targetAppleCount <= 800) {
                    m_targetAppleCount += 100;
                }
                else {
                    m_targetAppleCount = 900;
                }
                // 即使等级已经最大，也重新开始游戏
                stopGame();
                startGame();
            }
            else if (success_dialog.isConfirmed_3() == 2) {
                stopGame();
            }
        }
        else {
            msg = tr("您认输吧\n%1").arg(accuracyText);
            ExitConfirmDialog fail_dialog(this,
                ":/res/image/Apple/Images/APPLE_DLG_BG.png",
                ":/res/image/Apple/Images/APPLE_DLG_REPLAY.png",
                ":/res/image/Apple/Images/APPLE_DLG_END.png",
                msg);

            fail_dialog.exec();

            if (fail_dialog.isConfirmed()) {
                stopGame();
                startGame();
            }
            else {//结束
                stopGame();
            }
        }
    }
}

void AppleGameWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    // 只有在应该显示游戏元素时才绘制苹果和篮子
    if (m_showGameElements) {
        drawApples(painter);
        drawBasketAndSmallApples(painter);
    }
}

void AppleGameWidget::drawBackground(QPainter& painter)
{
    if (!m_backgroundPixmap.isNull()) {
        painter.drawPixmap(rect(), m_backgroundPixmap);
    }
    else {
        painter.fillRect(rect(), QColor(230, 240, 255)); // 淡蓝色背景
    }
}

void AppleGameWidget::drawApples(QPainter& painter)
{
    for (const Apple& apple : m_apples) {
        QPixmap pix = apple.isBad ? m_badApplePixmap : m_normalApplePixmap;
        QRectF targetRect(apple.pos, m_appleSize);
        painter.drawPixmap(targetRect.toRect(), pix);

        // 绘制字母
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(18);
        painter.setFont(font);
        painter.drawText(targetRect, Qt::AlignCenter, apple.letter);
    }
}

void AppleGameWidget::drawBasketAndSmallApples(QPainter& painter)
{
    // 篮子绘制在右下角
    int basketWidth = BASKET_WIDTH_RATIO * w_primary;
    int basketHeight = BASKET_HEIGHT_RATIO * h_primary;
    int margin = BASKET_MARGIN_RATIO * w_primary;
    m_basketPos = QPoint(width() - basketWidth - margin, height() - basketHeight - margin - 60);

    painter.drawPixmap(QRect(m_basketPos, QSize(basketWidth, basketHeight)), m_basketPixmap);

    if (m_smallAppleCount <= 0) return;

    int smallW = m_smallAppleSize.width();
    int smallH = m_smallAppleSize.height();

    // 垂直重叠量
    int verticalOverlap = smallH / VERTICAL_OVERLAP_DIVISOR;

    // 计算篮子内部区域
    int basketInnerMargin = BASKET_INNER_MARGIN;
    int basketInnerX = m_basketPos.x() + basketInnerMargin;
    int basketInnerY = m_basketPos.y() + basketInnerMargin;
    int basketInnerWidth = basketWidth - 2 * basketInnerMargin;
    int basketInnerHeight = basketHeight - 2 * basketInnerMargin;

    // 垂直方向位置
    int bottomY = basketInnerY + basketInnerHeight - smallH;
    int middleY = bottomY - smallH + verticalOverlap;
    int topY = middleY - smallH + verticalOverlap;

    // 计算水平位置
    int sectionWidth = basketInnerWidth / 3;

    // 第一层三个苹果的中心X
    int bottomX1 = basketInnerX + sectionWidth / 2;
    int bottomX2 = basketInnerX + sectionWidth + sectionWidth / 2;
    int bottomX3 = basketInnerX + 2 * sectionWidth + sectionWidth / 2;

    // 第二层两个苹果的中心X（位于下面两两苹果中心中轴线上）
    int middleX1 = (bottomX1 + bottomX2) / 2;
    int middleX2 = (bottomX2 + bottomX3) / 2;

    // 第三层三个苹果的中心X（与第一层一一对应）
    int topX1 = bottomX1;
    int topX2 = bottomX2;
    int topX3 = bottomX3;

    auto getDrawX = [smallW](int centerX) { return centerX - smallW / 2; };

    int drawn = 0;

    // 定义绘制结构
    struct ApplePosition {
        int centerX;
        int y;
    };

    // 所有苹果的位置（按绘制顺序，从下往上）
    QList<ApplePosition> positions = {
        {bottomX1, bottomY}, {bottomX2, bottomY}, {bottomX3, bottomY},  // 底层3个
        {middleX1, middleY}, {middleX2, middleY},                        // 中层2个
        {topX1, topY}, {topX2, topY}, {topX3, topY}                      // 顶层3个
    };

    // 绘制小苹果
    for (int i = 0; i < m_smallAppleCount && i < positions.size(); ++i) {
        const ApplePosition& pos = positions[m_smallAppleCount - 1 - i];
        painter.drawPixmap(QRect(getDrawX(pos.centerX), pos.y, smallW, smallH), m_smallApplePixmap);
        painter.drawPixmap(QRect(m_basketPos, QSize(basketWidth, basketHeight)), m_basketPixmap);
    }
}

void AppleGameWidget::keyPressEvent(QKeyEvent* event)
{
    if (!m_gameActive || m_gamePaused) {
        QWidget::keyPressEvent(event);
        return;
    }

    QString text = event->text();
    if (!text.isEmpty()) {
        QChar ch = text.at(0);
        if (ch.isLetter()) {
            removeAppleByLetter(ch);
        }
    }
    QWidget::keyPressEvent(event);
}


void AppleGameWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    int w = width();
    int h = height();

    int size_up = w * 0.0508;     // 或 h * 0.0903
    int size_right = w * 0.0586;  // 或 h * 0.1042
    int size_down = w * 0.0484;   // 或 h * 0.0861
    int size_left = w * 0.0492;   // 或 h * 0.0875

    int size_up1 = h * 0.065;     // 或 h * 0.0903
    int size_right1 = h * 0.0778;  // 或 h * 0.1042
    int size_down1 = h * 0.0639;   // 或 h * 0.0861
    int size_left1 = h * 0.0639;   // 或 h * 0.0875

    m_stopBtn->setFixedSize(size_up, size_up1);
    m_startBtn->setFixedSize(size_right, size_right1);
    m_settingsBtn->setFixedSize(size_down, size_down1);
    m_pauseBtn->setFixedSize(size_left, size_left1);


    m_stopBtn->move(w * 0.23 - m_stopBtn->width() / 2, h * 0.84 - m_stopBtn->height() / 2);
    m_startBtn->move(w * 0.2773 - m_startBtn->width() / 2, h * 0.9167 - m_startBtn->height() / 2);
    m_settingsBtn->move(w * 0.2121 - m_settingsBtn->width() / 2, h * 0.9521 - m_settingsBtn->height() / 2);
    m_pauseBtn->move(w * 0.1711 - m_pauseBtn->width() / 2, h * 0.8889 - m_pauseBtn->height() / 2);
}

void AppleGameWidget::closeEvent(QCloseEvent* event) {
    // 如果游戏还在运行，先停止
    if (m_gameActive) {
        stopGame();  // stopGame 中已经调用了 m_bgmPlayer->stop()
    }

    // 额外确保音乐停止
    if (m_bgmPlayer && m_bgmPlayer->state() == QMediaPlayer::PlayingState) {
        m_bgmPlayer->stop();
    }

    event->accept();
}

bool AppleGameWidget::eventFilter(QObject* obj, QEvent* event)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);
    if (!button || !button->isEnabled()) {
        return QWidget::eventFilter(obj, event);
    }

    // 处理鼠标进入事件（悬浮音效）
    if (event->type() == QEvent::Enter) {
        if (m_soundEnabled && m_hoverSoundEffect && m_hoverSoundEffect->isLoaded()) {
            m_hoverSoundEffect->play();
        }
    }

    // 处理鼠标按下事件（点击音效）
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (m_soundEnabled && m_clickSoundEffect && m_clickSoundEffect->isLoaded()) {
                m_clickSoundEffect->play();
            }
        }
    }

    // 继续传递事件给默认处理器
    return QWidget::eventFilter(obj, event);
}