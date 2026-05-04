#include "exitconfirmdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>

constexpr double TRIPLE_STATE_BTN_DIVISOR = 3.0;
constexpr int BORDER_IMAGE_MULTIPLIER = 2;


ExitConfirmDialog::ExitConfirmDialog(QWidget* parent, QString back_url, QString left_url, QString right_url, QString text)
    : QDialog(parent)
    , m_confirmed(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);

    m_backgroundPixmap.load(back_url);

    if (!m_backgroundPixmap.isNull()) {
        setFixedSize(m_backgroundPixmap.width() * 2, m_backgroundPixmap.height() * 2);
    }
    else {
        setFixedSize(400, 250);
    }

    QScreen* screen = QApplication::primaryScreen();
    QPoint center = screen->geometry().center();
    move(center.x() - width() / 2, center.y() - height() / 2);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 40, 30, 30);
    mainLayout->setSpacing(20);

    mainLayout->addStretch();

    QLabel* textLabel = new QLabel(text, this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);
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

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(30);
    btnLayout->addStretch();

    QPixmap* left_map = new QPixmap(left_url);
    QPixmap* right_map = new QPixmap(right_url);

    qreal left_width = left_map->width() / TRIPLE_STATE_BTN_DIVISOR;
    qreal right_width = right_map->width() / TRIPLE_STATE_BTN_DIVISOR;

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


    connect(confirmBtn, &QPushButton::clicked, [this]() {
        m_confirmed = true;
        accept();
        });

    connect(continueBtn, &QPushButton::clicked, [this]() {
        m_confirmed = false;
        reject();
        });

    m_hoverSoundEffect = new QSoundEffect(this);
    m_hoverSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav"));
    m_hoverSoundEffect->setVolume(1.0f);

    m_clickSoundEffect = new QSoundEffect(this);
    m_clickSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/BTN_CLICK.wav"));
    m_clickSoundEffect->setVolume(1.0f);

    confirmBtn->installEventFilter(this);
    continueBtn->installEventFilter(this);

}


ExitConfirmDialog::ExitConfirmDialog(QWidget* parent, QString back_url, QString left_url, QString min_url, QString right_url, QString text)
    : QDialog(parent)
    , m_confirmed(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);

    m_backgroundPixmap.load(back_url);

    if (!m_backgroundPixmap.isNull()) {
        setFixedSize(m_backgroundPixmap.width() * 2, m_backgroundPixmap.height() * 2);
    }
    else {
        setFixedSize(400, 250);
    }

    QScreen* screen = QApplication::primaryScreen();
    QPoint center = screen->geometry().center();
    move(center.x() - width() / 2, center.y() - height() / 2);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 40, 30, 30);
    mainLayout->setSpacing(20);

    mainLayout->addStretch();

    QLabel* textLabel = new QLabel(text, this);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setWordWrap(true);
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

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(20);
    btnLayout->addStretch();

    QPixmap* left_map = new QPixmap(left_url);
    QPixmap* min_map = new QPixmap(min_url);
    QPixmap* right_map = new QPixmap(right_url);

    qreal left_width = left_map->width() / TRIPLE_STATE_BTN_DIVISOR;
    qreal min_width = min_map->width() / 3;
    qreal right_width = right_map->width() / TRIPLE_STATE_BTN_DIVISOR;

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


    connect(confirmBtn, &QPushButton::clicked, [this]() {
        m_confirmed = true;
        m_confirmed_3 = 0;
        accept();
        });

    connect(minBtn, &QPushButton::clicked, [this]() {
        m_confirmed_3 = 1;
        accept();
        });

    connect(continueBtn, &QPushButton::clicked, [this]() {
        m_confirmed = false;
        m_confirmed_3 = 2;
        reject();
        });

    m_hoverSoundEffect = new QSoundEffect(this);
    m_hoverSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav"));
    m_hoverSoundEffect->setVolume(1.0f);

    m_clickSoundEffect = new QSoundEffect(this);
    m_clickSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/BTN_CLICK.wav"));
    m_clickSoundEffect->setVolume(1.0f);

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
        painter.fillRect(rect(), QColor(0, 0, 0, 180));
    }
}

bool ExitConfirmDialog::eventFilter(QObject* obj, QEvent* event)
{
    QAbstractButton* button = qobject_cast<QAbstractButton*>(obj);
    if (!button || !button->isEnabled()) {
        return QWidget::eventFilter(obj, event);
    }

    if (event->type() == QEvent::Enter) {
        if (m_hoverSoundEffect && m_hoverSoundEffect->isLoaded()) {
            m_hoverSoundEffect->play();
        }
    }

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (m_clickSoundEffect && m_clickSoundEffect->isLoaded()) {
                m_clickSoundEffect->play();
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}