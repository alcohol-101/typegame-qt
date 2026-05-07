// filename: spacesettingsdialog.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Implementation of SpaceSettingsDialog

#include "spacesettingsdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>

#include "exitconfirmdialog.h"

constexpr double BUTTON_WIDTH_RATIO = 0.17778;
constexpr double BUTTON_HEIGHT_RATIO = 0.09333;
constexpr double OK_BTN_X_RATIO = 0.38889;
constexpr double CANCEL_BTN_X_RATIO = 0.58889;
constexpr double DEFAULT_BTN_X_RATIO = 0.78889;
constexpr double BTN_Y_RATIO = 0.85333;

SpaceSettingsDialog::SpaceSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , m_enemyMaxSlider(nullptr)
    , m_speedSlider(nullptr)
    , m_upgradeIntervalSlider(nullptr)
    , m_enemyMaxLabel(nullptr)
    , m_speedLabel(nullptr)
    , m_upgradeIntervalLabel(nullptr)
    , m_rewardCheckBox(nullptr)
    , m_valuesChanged(false)
{
    setFixedSize(900, 600);
    setWindowFlags(Qt::FramelessWindowHint);
    setModal(true);
    setStyleSheet("SpaceSettingsDialog { border-image: url(:/res/image/Space/Images/APPLE_SETUP.png); }");
    setupUi();

    m_hoverSoundEffect = new QSoundEffect(this);
    m_hoverSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav"));
    m_hoverSoundEffect->setVolume(1.0f);
    m_clickSoundEffect = new QSoundEffect(this);
    m_clickSoundEffect->setSource(QUrl("qrc:/res/image/Common/Sounds/BTN_CLICK.wav"));
    m_clickSoundEffect->setVolume(1.0f);

    m_okBtn->installEventFilter(this);
    m_cancelBtn->installEventFilter(this);
    m_defaultBtn->installEventFilter(this);
}

void SpaceSettingsDialog::setupUi()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* enemyLayout = new QHBoxLayout;
    QLabel* enemyLabel = new QLabel(tr("敌机最大同屏数量 :"), this);
    m_enemyMaxSlider = new QSlider(Qt::Horizontal, this);
    m_enemyMaxSlider->setRange(1, 10);
    m_enemyMaxLabel = new QLabel("3", this);
    m_enemyMaxLabel->setAlignment(Qt::AlignCenter);
    enemyLayout->addStretch(16);
    enemyLayout->addWidget(enemyLabel);
    enemyLayout->addWidget(m_enemyMaxSlider, 50);
    enemyLayout->addWidget(m_enemyMaxLabel);
    enemyLayout->addStretch(2);
    connect(m_enemyMaxSlider, &QSlider::valueChanged, this, &SpaceSettingsDialog::onEnemyMaxChanged);

    QHBoxLayout* speedLayout = new QHBoxLayout;
    QLabel* speedLabel = new QLabel(tr("速度 :"), this);
    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(1, 10);
    m_speedLabel = new QLabel("3", this);
    m_speedLabel->setAlignment(Qt::AlignCenter);
    speedLayout->addStretch(16);
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(m_speedSlider, 50);
    speedLayout->addWidget(m_speedLabel);
    speedLayout->addStretch(2);
    connect(m_speedSlider, &QSlider::valueChanged, this, &SpaceSettingsDialog::onSpeedChanged);

    QHBoxLayout* intervalLayout = new QHBoxLayout;
    QLabel* intervalLabel = new QLabel(tr("难度升级间隔(秒) :"), this);
    m_upgradeIntervalSlider = new QSlider(Qt::Horizontal, this);
    m_upgradeIntervalSlider->setRange(30, 600);
    m_upgradeIntervalLabel = new QLabel("120", this);
    m_upgradeIntervalLabel->setAlignment(Qt::AlignCenter);
    intervalLayout->addStretch(16);
    intervalLayout->addWidget(intervalLabel);
    intervalLayout->addWidget(m_upgradeIntervalSlider, 50);
    intervalLayout->addWidget(m_upgradeIntervalLabel);
    intervalLayout->addStretch(2);
    connect(m_upgradeIntervalSlider, &QSlider::valueChanged, this, &SpaceSettingsDialog::onUpgradeIntervalChanged);

    QHBoxLayout* rewardLayout = new QHBoxLayout;
    QLabel* rewardLabel = new QLabel(tr("奖励模式 :"), this);
    m_rewardCheckBox = new QCheckBox(this);
    m_rewardCheckBox->setChecked(false);
    rewardLayout->addStretch(16);
    rewardLayout->addWidget(rewardLabel);
    rewardLayout->addWidget(m_rewardCheckBox);
    rewardLayout->addStretch(50);
    rewardLayout->addStretch(2);
    connect(m_rewardCheckBox, &QCheckBox::toggled, this, &SpaceSettingsDialog::onRewardToggled);

    qreal w = width() * BUTTON_WIDTH_RATIO;
    qreal h = height() * BUTTON_HEIGHT_RATIO;
    m_okBtn = new QPushButton(this);
    m_okBtn->setFixedSize(w, h);
    m_okBtn->setStyleSheet("QPushButton{ border-image: url(:/res/image/Common/Images/OK.png) 0 144 0 0; border:none; }"
                         "QPushButton:hover{ border-image: url(:/res/image/Common/Images/OK.png) 0 72 0 72; }");
    m_cancelBtn = new QPushButton(this);
    m_cancelBtn->setFixedSize(w, h);
    m_cancelBtn->setStyleSheet("QPushButton{ border-image: url(:/res/image/Common/Images/CANCEL.png) 0 144 0 0; border:none; }"
                             "QPushButton:hover{ border-image: url(:/res/image/Common/Images/CANCEL.png) 0 72 0 72; }");
    m_defaultBtn = new QPushButton(this);
    m_defaultBtn->setFixedSize(w, h);
    m_defaultBtn->setStyleSheet("QPushButton{ border-image: url(:/res/image/Common/Images/DEFAULT.png) 0 144 0 0; border:none; }"
                              "QPushButton:hover{ border-image: url(:/res/image/Common/Images/DEFAULT.png) 0 72 0 72; }");

    mainLayout->addLayout(enemyLayout);
    mainLayout->addLayout(speedLayout);
    mainLayout->addLayout(intervalLayout);
    mainLayout->addLayout(rewardLayout);
    mainLayout->addStretch();

    connect(m_okBtn, &QPushButton::clicked, this, &SpaceSettingsDialog::onOkClicked);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SpaceSettingsDialog::onCancelClicked);
    connect(m_defaultBtn, &QPushButton::clicked, this, &SpaceSettingsDialog::onDefaultClicked);

    m_enemyMaxSlider->setValue(3);
    m_speedSlider->setValue(3);
    m_upgradeIntervalSlider->setValue(120);
    m_rewardCheckBox->setChecked(true);
    m_valuesChanged = false;
}

void SpaceSettingsDialog::setCurrentValues(int enemyMax, int speed, int upgradeIntervalSec, bool rewardOn)
{
    m_enemyMaxSlider->blockSignals(true);
    m_speedSlider->blockSignals(true);
    m_upgradeIntervalSlider->blockSignals(true);
    m_rewardCheckBox->blockSignals(true);

    m_enemyMaxSlider->setValue(enemyMax);
    m_speedSlider->setValue(speed);
    m_upgradeIntervalSlider->setValue(upgradeIntervalSec);
    m_rewardCheckBox->setChecked(rewardOn);
    m_enemyMaxLabel->setText(QString::number(enemyMax));
    m_speedLabel->setText(QString::number(speed));
    m_upgradeIntervalLabel->setText(QString::number(upgradeIntervalSec));

    m_enemyMaxSlider->blockSignals(false);
    m_speedSlider->blockSignals(false);
    m_upgradeIntervalSlider->blockSignals(false);
    m_rewardCheckBox->blockSignals(false);

    m_originalEnemyMax = enemyMax;
    m_originalSpeed = speed;
    m_originalUpgradeInterval = upgradeIntervalSec;
    m_originalRewardOn = rewardOn;
    m_valuesChanged = false;
}

void SpaceSettingsDialog::onEnemyMaxChanged(int value) {
    m_enemyMaxLabel->setText(QString::number(value));
    m_valuesChanged = true;
}
void SpaceSettingsDialog::onSpeedChanged(int value) {
    m_speedLabel->setText(QString::number(value));
    m_valuesChanged = true;
}
void SpaceSettingsDialog::onUpgradeIntervalChanged(int value) {
    m_upgradeIntervalLabel->setText(QString::number(value));
    m_valuesChanged = true;
}
void SpaceSettingsDialog::onRewardToggled(bool) { m_valuesChanged = true; }

void SpaceSettingsDialog::onOkClicked()
{
    ExitConfirmDialog dlg(this, ":/res/image/Common/Images/MAIN_DLG_BG.png",
                          ":/res/image/Common/Images/YES.png",
                          ":/res/image/Common/Images/NO.png",
                          tr("设置改变是否立即生效？"));
    dlg.exec();
    if (!dlg.isConfirmed()) return;
    emit settingsApplied(m_enemyMaxSlider->value(), m_speedSlider->value(),
                         m_upgradeIntervalSlider->value(), m_rewardCheckBox->isChecked());
    accept();
}

void SpaceSettingsDialog::onCancelClicked()
{
    m_enemyMaxSlider->blockSignals(true);
    m_speedSlider->blockSignals(true);
    m_upgradeIntervalSlider->blockSignals(true);
    m_rewardCheckBox->blockSignals(true);
    m_enemyMaxSlider->setValue(m_originalEnemyMax);
    m_speedSlider->setValue(m_originalSpeed);
    m_upgradeIntervalSlider->setValue(m_originalUpgradeInterval);
    m_rewardCheckBox->setChecked(m_originalRewardOn);
    m_enemyMaxLabel->setText(QString::number(m_originalEnemyMax));
    m_speedLabel->setText(QString::number(m_originalSpeed));
    m_upgradeIntervalLabel->setText(QString::number(m_originalUpgradeInterval));
    m_enemyMaxSlider->blockSignals(false);
    m_speedSlider->blockSignals(false);
    m_upgradeIntervalSlider->blockSignals(false);
    m_rewardCheckBox->blockSignals(false);
    m_valuesChanged = false;
    reject();
}

void SpaceSettingsDialog::onDefaultClicked()
{
    m_enemyMaxSlider->setValue(3);
    m_speedSlider->setValue(3);
    m_upgradeIntervalSlider->setValue(120);
    m_rewardCheckBox->setChecked(true);
    m_valuesChanged = true;
}

void SpaceSettingsDialog::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_okBtn->move(width() * OK_BTN_X_RATIO, height() * BTN_Y_RATIO);
    m_cancelBtn->move(width() * CANCEL_BTN_X_RATIO, height() * BTN_Y_RATIO);
    m_defaultBtn->move(width() * DEFAULT_BTN_X_RATIO, height() * BTN_Y_RATIO);
}

bool SpaceSettingsDialog::eventFilter(QObject* obj, QEvent* event)
{
    QAbstractButton* btn = qobject_cast<QAbstractButton *>(obj);
    if (!btn || !btn->isEnabled()) return QWidget::eventFilter(obj, event);
    if (event->type() == QEvent::Enter) {
        if (m_hoverSoundEffect && m_hoverSoundEffect->isLoaded()) m_hoverSoundEffect->play();
    }
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::LeftButton && m_clickSoundEffect && m_clickSoundEffect->isLoaded())
            m_clickSoundEffect->play();
    }
    return QWidget::eventFilter(obj, event);
}