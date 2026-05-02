#include "exitconfirmdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>

// ========== 常量 ==========
constexpr double TRIPLE_STATE_BTN_DIVISOR = 3.0;
constexpr int BORDER_IMAGE_MULTIPLIER = 2;
// ==========================


ExitConfirmDialog::ExitConfirmDialog(QWidget* parent, QString back_url, QString left_url, QString right_url, QString text)
    : QDialog(parent)
    , m_confirmed(false)
{
    // ===== 窗口设置 =====
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);  // 允许透明背景

    // 加载背景图
    m_backgroundPixmap.load(back_url);

    // 设置窗口大小为背景图尺寸（你也可以手动指定）
    if (!m_backgroundPixmap.isNull()) {
        setFixedSize(m_backgroundPixmap.width() * 2, m_backgroundPixmap.height() * 2);
    }
    else {
        setFixedSize(400, 250);  // 备用尺寸
    }

    // 居中显示
    QScreen* screen = QApplication::primaryScreen();
    QPoint center = screen->geometry().center();
    move(center.x() - width() / 2, center.y() - height() / 2);

    // ===== 创建布局 =====
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 40, 30, 30);  // 左、上、右、下边距
    mainLayout->setSpacing(20);

    // 顶部弹簧（把内容往下推）
    mainLayout->addStretch();

    // 提示文字
    QLabel* textLabel = new QLabel(text, this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);  // 支持换行
    textLabel->setStyleSheet(R"(
        QLabel {
            background: transparent;
            color: black;
            font-size: 16px;
            font-weight: bold;
        }
    )");
    mainLayout->addWidget(textLabel);

    mainLayout->addStretch();

    // ===== 按钮区域 =====
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(30);
    btnLayout->addStretch();

    QPixmap* left_map = new QPixmap(left_url);
    QPixmap* right_map = new QPixmap(right_url);

    qreal left_width = left_map->width() / TRIPLE_STATE_BTN_DIVISOR;
    qreal right_width = right_map->width() / TRIPLE_STATE_BTN_DIVISOR;

    // 退出按钮
    QPushButton* confirmBtn = new QPushButton(this);
    confirmBtn->setFixedSize(92, 46);
    QString confirmStyle = QString(R"(
        QPushButton{
            border-image: url("%1") 0 %3 0 0;
             border: none;
            }
         QPushButton:hover {
            border-image: url("%1") 0 %2 0 %2;
        }
    )").arg(left_url).arg(left_width).arg(BORDER_IMAGE_MULTIPLIER * left_width);
    confirmBtn->setStyleSheet(confirmStyle);
    btnLayout->addWidget(confirmBtn);
    btnLayout->addStretch();

    // 继续按钮
    QPushButton* continueBtn = new QPushButton(this);
    continueBtn->setFixedSize(92, 46);
    confirmStyle = QString(R"(
        QPushButton {
            border-image: url("%1") 0 %3 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url("%1") 0 %2 0 %2;
        }
    )").arg(right_url).arg(right_width).arg(BORDER_IMAGE_MULTIPLIER * right_width);
    continueBtn->setStyleSheet(confirmStyle);

    btnLayout->addWidget(continueBtn);

    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);


    // ===== 连接信号 =====
    connect(confirmBtn, &QPushButton::clicked, [this]() {
        m_confirmed = true;
        accept();  // 关闭对话框，返回 QDialog::Accepted
        });

    connect(continueBtn, &QPushButton::clicked, [this]() {
        m_confirmed = false;
        reject();  // 关闭对话框，返回 QDialog::Rejected
        });

    // ========== 悬浮音效初始化 ==========
    m_hoverSoundEffect = new QSoundEffect(this);
    m_hoverSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav"));
    m_hoverSoundEffect->setVolume(1.0f);  // 音量 0.0 ~ 1.0

    // 初始化点击音效
    m_clickSoundEffect = new QSoundEffect(this);
    m_clickSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/BTN_CLICK.wav"));  // 你的点击音效文件路径
    m_clickSoundEffect->setVolume(1.0f);

    // ===== 为按钮安装事件过滤器 =====
    confirmBtn->installEventFilter(this);
    continueBtn->installEventFilter(this);

}


ExitConfirmDialog::ExitConfirmDialog(QWidget* parent, QString back_url, QString left_url, QString min_url, QString right_url, QString text)
    : QDialog(parent)
    , m_confirmed(false)
{
    // ===== 窗口设置 =====
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);  // 允许透明背景

    // 加载背景图
    m_backgroundPixmap.load(back_url);

    // 设置窗口大小为背景图尺寸（你也可以手动指定）
    if (!m_backgroundPixmap.isNull()) {
        setFixedSize(m_backgroundPixmap.width() * 2, m_backgroundPixmap.height() * 2);
    }
    else {
        setFixedSize(400, 250);  // 备用尺寸
    }

    // 居中显示
    QScreen* screen = QApplication::primaryScreen();
    QPoint center = screen->geometry().center();
    move(center.x() - width() / 2, center.y() - height() / 2);

    // ===== 创建布局 =====
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 40, 30, 30);  // 左、上、右、下边距
    mainLayout->setSpacing(20);

    // 顶部弹簧（把内容往下推）
    mainLayout->addStretch();

    // 提示文字
    QLabel* textLabel = new QLabel(text, this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);  // 支持换行
    textLabel->setStyleSheet(R"(
        QLabel {
            background: transparent;
            color: black;
            font-size: 16px;
            font-weight: bold;
        }
    )");
    mainLayout->addWidget(textLabel);

    mainLayout->addStretch();

    // ===== 按钮区域 =====
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    btnLayout->addStretch();

    QPixmap* left_map = new QPixmap(left_url);
    QPixmap* min_map = new QPixmap(min_url);
    QPixmap* right_map = new QPixmap(right_url);

    qreal left_width = left_map->width() / TRIPLE_STATE_BTN_DIVISOR;
    qreal min_width = min_map->width() / 3;
    qreal right_width = right_map->width() / TRIPLE_STATE_BTN_DIVISOR;

    // 左按钮
    QPushButton* confirmBtn = new QPushButton(this);
    confirmBtn->setFixedSize(92, 46);
    QString confirmStyle = QString(R"(
        QPushButton{
            border-image: url("%1") 0 %3 0 0;
             border: none;
            }
         QPushButton:hover {
            border-image: url("%1") 0 %2 0 %2;
        }
    )").arg(left_url).arg(left_width).arg(BORDER_IMAGE_MULTIPLIER * left_width);
    confirmBtn->setStyleSheet(confirmStyle);
    btnLayout->addWidget(confirmBtn);
    btnLayout->addStretch();

    //中间按钮
    QPushButton* minBtn = new QPushButton(this);
    minBtn->setFixedSize(92, 46);
    confirmStyle = QString(R"(
        QPushButton{
            border-image: url("%1") 0 %3 0 0;
             border: none;
            }
         QPushButton:hover {
            border-image: url("%1") 0 %2 0 %2;
        }
    )").arg(min_url).arg(min_width).arg(2 * min_width);
    minBtn->setStyleSheet(confirmStyle);
    btnLayout->addWidget(minBtn);
    btnLayout->addStretch();

    // 右按钮
    QPushButton* continueBtn = new QPushButton(this);
    continueBtn->setFixedSize(92, 46);
    confirmStyle = QString(R"(
        QPushButton {
            border-image: url("%1") 0 %3 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url("%1") 0 %2 0 %2;
        }
    )").arg(right_url).arg(right_width).arg(BORDER_IMAGE_MULTIPLIER * right_width);
    continueBtn->setStyleSheet(confirmStyle);
    btnLayout->addWidget(continueBtn);
    btnLayout->addStretch();

    mainLayout->addLayout(btnLayout);


    // ===== 连接信号 =====
    connect(confirmBtn, &QPushButton::clicked, [this]() {
        m_confirmed = true;
        m_confirmed_3 = 0;
        accept();  // 关闭对话框，返回 QDialog::Accepted
        });

    connect(minBtn, &QPushButton::clicked, [this]() {
        m_confirmed_3 = 1;
        accept();  // 关闭对话框，返回 QDialog::Accepted
        });

    connect(continueBtn, &QPushButton::clicked, [this]() {
        m_confirmed = false;
        m_confirmed_3 = 2;
        reject();  // 关闭对话框，返回 QDialog::Rejected
        });

    // ========== 悬浮音效初始化 ==========
    m_hoverSoundEffect = new QSoundEffect(this);
    m_hoverSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav"));
    m_hoverSoundEffect->setVolume(1.0f);  // 音量 0.0 ~ 1.0

    // 初始化点击音效
    m_clickSoundEffect = new QSoundEffect(this);
    m_clickSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/BTN_CLICK.wav"));  // 你的点击音效文件路径
    m_clickSoundEffect->setVolume(1.0f);

    // ===== 为按钮安装事件过滤器 =====
    confirmBtn->installEventFilter(this);
    minBtn->installEventFilter(this);
    continueBtn->installEventFilter(this);
}

ExitConfirmDialog::~ExitConfirmDialog()
{
}

void ExitConfirmDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if (!m_backgroundPixmap.isNull()) {
        painter.drawPixmap(rect(), m_backgroundPixmap);
    }
    else {
        // 备用：绘制半透明黑色背景
        painter.fillRect(rect(), QColor(0, 0, 0, 180));
    }
}

bool ExitConfirmDialog::eventFilter(QObject* obj, QEvent* event)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);
    if (!button || !button->isEnabled()) {
        return QWidget::eventFilter(obj, event);
    }

    // 处理鼠标进入事件（悬浮音效）
    if (event->type() == QEvent::Enter) {
        if (m_hoverSoundEffect && m_hoverSoundEffect->isLoaded()) {
            m_hoverSoundEffect->play();
        }
    }

    // 处理鼠标按下事件（点击音效）
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (m_clickSoundEffect && m_clickSoundEffect->isLoaded()) {
                m_clickSoundEffect->play();
            }
        }
    }

    // 继续传递事件给默认处理器
    return QWidget::eventFilter(obj, event);
}