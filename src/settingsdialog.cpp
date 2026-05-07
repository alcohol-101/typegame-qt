// filename: settingsdialog.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Implementation of SettingsDialog

#include "settingsdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPalette>
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>

#include "exitconfirmdialog.h"

constexpr double BUTTON_WIDTH_RATIO = 0.17778;
constexpr double BUTTON_HEIGHT_RATIO = 0.09333;
constexpr double OK_BTN_X_RATIO = 0.38889;
constexpr double CANCEL_BTN_X_RATIO = 0.58889;
constexpr double DEFAULT_BTN_X_RATIO = 0.78889;
constexpr double BTN_Y_RATIO = 0.85333;

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
    , m_levelSlider(nullptr)
    , m_targetSlider(nullptr)
    , m_badSlider(nullptr)
    , m_maxAppleSlider(nullptr)
    , m_levelValueLabel(nullptr)
    , m_targetValueLabel(nullptr)
    , m_badValueLabel(nullptr)
    , m_maxAppleValueLabel(nullptr)
    , m_soundCheckBox(nullptr)
    , m_valuesChanged(false)
{
    setFixedSize(900, 600);
    setWindowFlags(Qt::FramelessWindowHint);
    setModal(true);

    this->setStyleSheet(R"(
    SettingsDialog {
        border-image: url(:/res/image/Space/Images/APPLE_SETUP.png);
        background-repeat: no-repeat;
        background-position: center;

        }
    )");

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

    this->update();
}

void SettingsDialog::setupUi()
{

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 等级 0-9
    QHBoxLayout* levelLayout = new QHBoxLayout();
    QLabel* levelLabel = new QLabel(tr("游戏等级 :"), this);
    m_levelSlider = new QSlider(Qt::Horizontal, this);
    m_levelSlider->setRange(0, 9);

    m_levelValueLabel = new QLabel("1", this);
    m_levelValueLabel->setAlignment(Qt::AlignCenter);

    levelLayout->addStretch(16);
    levelLayout->addWidget(levelLabel);
    levelLayout->addWidget(m_levelSlider, 50);
    levelLayout->addWidget(m_levelValueLabel);
    levelLayout->addStretch(2);

    connect(m_levelSlider, &QSlider::valueChanged, this, &SettingsDialog::onLevelChanged);

    // 过关苹果数量 10-900
    QHBoxLayout* targetLayout = new QHBoxLayout();
    QLabel* targetLabel = new QLabel(tr("过关苹果数 :"), this);
    m_targetSlider = new QSlider(Qt::Horizontal, this);
    m_targetSlider->setRange(10, 900);

    m_targetValueLabel = new QLabel("10", this);
    m_targetValueLabel->setAlignment(Qt::AlignCenter);

    targetLayout->addStretch(16);
    targetLayout->addWidget(targetLabel);
    targetLayout->addWidget(m_targetSlider, 50);
    targetLayout->addWidget(m_targetValueLabel);
    targetLayout->addStretch(2);

    connect(m_targetSlider, &QSlider::valueChanged, this, &SettingsDialog::onTargetChanged);

    // 失败苹果数量 5-50
    QHBoxLayout* badLayout = new QHBoxLayout();
    QLabel* badLabel = new QLabel(tr("失败苹果数 :"), this);
    m_badSlider = new QSlider(Qt::Horizontal, this);
    m_badSlider->setRange(5, 50);

    m_badValueLabel = new QLabel("10", this);

    m_badValueLabel->setAlignment(Qt::AlignCenter);

    badLayout->addStretch(16);
    badLayout->addWidget(badLabel);
    badLayout->addWidget(m_badSlider, 50);
    badLayout->addWidget(m_badValueLabel);
    badLayout->addStretch(2);

    connect(m_badSlider, &QSlider::valueChanged, this, &SettingsDialog::onBadChanged);

    // 同屏苹果数量 1-20
    QHBoxLayout* maxAppleLayout = new QHBoxLayout();
    QLabel* maxAppleLabel = new QLabel(tr("最多同屏苹果数 :"), this);
    m_maxAppleSlider = new QSlider(Qt::Horizontal, this);
    m_maxAppleSlider->setRange(1, 20);
    m_maxAppleValueLabel = new QLabel("10", this);
    m_maxAppleValueLabel->setAlignment(Qt::AlignCenter);

    maxAppleLayout->addStretch(16);
    maxAppleLayout->addWidget(maxAppleLabel);
    maxAppleLayout->addWidget(m_maxAppleSlider, 50);
    maxAppleLayout->addWidget(m_maxAppleValueLabel);
    maxAppleLayout->addStretch(2);

    connect(m_maxAppleSlider, &QSlider::valueChanged, this, &SettingsDialog::onMaxAppleChanged);

    // 音效开关
    QHBoxLayout* soundLayout = new QHBoxLayout();
    QLabel* soundLabel = new QLabel(tr("音效开关 :"), this);
    m_soundCheckBox = new QCheckBox(this);
    m_soundCheckBox->setChecked(true);
    soundLayout->addStretch(16);
    soundLayout->addWidget(soundLabel);
    soundLayout->addWidget(m_soundCheckBox);
    soundLayout->addStretch(50);
    soundLayout->addStretch(2);

    connect(m_soundCheckBox, &QCheckBox::toggled, this, &SettingsDialog::onSoundToggled);

    // 按钮行
    m_okBtn = new QPushButton(this);
    m_cancelBtn = new QPushButton(this);
    m_defaultBtn = new QPushButton(this);

    qreal nowW = this->width();
    qreal nowH = this->height();

    qreal w = nowW * BUTTON_WIDTH_RATIO;
    qreal h = nowH * BUTTON_HEIGHT_RATIO;

    m_okBtn->setFixedSize(w, h);
    m_okBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/OK.png) 0 144 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/OK.png) 0 72 0 72;
        }
    )");
    m_cancelBtn->setFixedSize(w, h);
    m_cancelBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/CANCEL.png) 0 144 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/CANCEL.png) 0 72 0 72;
        }
    )");
    m_defaultBtn->setFixedSize(w, h);
    m_defaultBtn->setStyleSheet(R"(
        QPushButton {
            border-image: url(:/res/image/Common/Images/DEFAULT.png) 0 144 0 0;
            border: none;
        }
        QPushButton:hover {
            border-image: url(:/res/image/Common/Images/DEFAULT.png) 0 72 0 72;
        }
    )");

    mainLayout->addLayout(levelLayout);
    mainLayout->addLayout(targetLayout);
    mainLayout->addLayout(badLayout);
    mainLayout->addLayout(maxAppleLayout);
    mainLayout->addLayout(soundLayout);
    mainLayout->addStretch();

    connect(m_okBtn, &QPushButton::clicked, this, &SettingsDialog::onOkClicked);
    connect(m_cancelBtn, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
    connect(m_defaultBtn, &QPushButton::clicked, this, &SettingsDialog::onDefaultClicked);

    // 初始化默认值
    m_levelSlider->setValue(1);
    m_targetSlider->setValue(10);
    m_badSlider->setValue(10);
    m_maxAppleSlider->setValue(10);
    m_soundCheckBox->setChecked(true);
    m_valuesChanged = false;

    this->update();
}

void SettingsDialog::setCurrentValues(int level, int target, int maxBad, int maxApple, bool soundEnabled)
{
    // 临时断开信号连接，避免触发 changed 标志
    m_levelSlider->blockSignals(true);
    m_targetSlider->blockSignals(true);
    m_badSlider->blockSignals(true);
    m_maxAppleSlider->blockSignals(true);
    m_soundCheckBox->blockSignals(true);

    m_levelSlider->setValue(level);
    m_targetSlider->setValue(target);
    m_badSlider->setValue(maxBad);
    m_maxAppleSlider->setValue(maxApple);
    m_soundCheckBox->setChecked(soundEnabled);

    m_levelValueLabel->setText(QString::number(level));
    m_targetValueLabel->setText(QString::number(target));
    m_badValueLabel->setText(QString::number(maxBad));
    m_maxAppleValueLabel->setText(QString::number(maxApple));

    m_levelSlider->blockSignals(false);
    m_targetSlider->blockSignals(false);
    m_badSlider->blockSignals(false);
    m_maxAppleSlider->blockSignals(false);
    m_soundCheckBox->blockSignals(false);

    m_originalLevel = level;
    m_originalTarget = target;
    m_originalBad = maxBad;
    m_originalMaxApple = maxApple;
    m_originalSoundEnabled = soundEnabled;
    m_valuesChanged = false;
}

void SettingsDialog::onLevelChanged(int value)
{
    m_levelValueLabel->setText(QString::number(value));
    m_valuesChanged = true;
}

void SettingsDialog::onTargetChanged(int value)
{
    m_targetValueLabel->setText(QString::number(value));
    m_valuesChanged = true;
}

void SettingsDialog::onBadChanged(int value)
{
    m_badValueLabel->setText(QString::number(value));
    m_valuesChanged = true;
}

void SettingsDialog::onMaxAppleChanged(int value)
{
    m_maxAppleValueLabel->setText(QString::number(value));
    m_valuesChanged = true;
}

void SettingsDialog::onSoundToggled(bool checked)
{
    Q_UNUSED(checked);
    m_valuesChanged = true;
}

void SettingsDialog::onOkClicked()
{
    ExitConfirmDialog dialog(this, ":/res/image/Common/Images/MAIN_DLG_BG.png", ":/res/image/Common/Images/YES.png",
        ":/res/image/Common/Images/NO.png", tr("设置改变是否立即生效？"));
    dialog.exec();

    if (!dialog.isConfirmed()) {
        return;
    }
    emit settingsApplied(m_levelSlider->value(), m_targetSlider->value(), m_badSlider->value(), m_maxAppleSlider->value(), m_soundCheckBox->isChecked());
    accept();
}

void SettingsDialog::onCancelClicked()
{
    // 恢复到原始值
    m_levelSlider->blockSignals(true);
    m_targetSlider->blockSignals(true);
    m_badSlider->blockSignals(true);
    m_maxAppleSlider->blockSignals(true);
    m_soundCheckBox->blockSignals(true);

    m_levelSlider->setValue(m_originalLevel);
    m_targetSlider->setValue(m_originalTarget);
    m_badSlider->setValue(m_originalBad);
    m_maxAppleSlider->setValue(m_originalMaxApple);
    m_soundCheckBox->setChecked(m_originalSoundEnabled);

    m_levelValueLabel->setText(QString::number(m_originalLevel));
    m_targetValueLabel->setText(QString::number(m_originalTarget));
    m_badValueLabel->setText(QString::number(m_originalBad));
    m_maxAppleValueLabel->setText(QString::number(m_originalMaxApple));

    m_levelSlider->blockSignals(false);
    m_targetSlider->blockSignals(false);
    m_badSlider->blockSignals(false);
    m_maxAppleSlider->blockSignals(false);
    m_soundCheckBox->blockSignals(false);

    m_valuesChanged = false;
    reject();
}

void SettingsDialog::onDefaultClicked()
{
    m_levelSlider->setValue(1);
    m_targetSlider->setValue(100);
    m_badSlider->setValue(10);
    m_maxAppleSlider->setValue(10);
    m_soundCheckBox->setChecked(true);
    m_valuesChanged = true; // 视为修改
}

void SettingsDialog::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    int w = width();
    int h = height();

    m_okBtn->move(w * OK_BTN_X_RATIO, h * BTN_Y_RATIO);
    m_cancelBtn->move(w * CANCEL_BTN_X_RATIO, h * BTN_Y_RATIO);
    m_defaultBtn->move(w * DEFAULT_BTN_X_RATIO, h * BTN_Y_RATIO);
}

bool SettingsDialog::eventFilter(QObject* obj, QEvent* event)
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