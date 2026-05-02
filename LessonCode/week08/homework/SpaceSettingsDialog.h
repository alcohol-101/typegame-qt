// SpaceSettingsDialog.h
#ifndef SPACESETTINGSDIALOG_H
#define SPACESETTINGSDIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSoundEffect>

class SpaceSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SpaceSettingsDialog(QWidget *parent = nullptr);
    void setCurrentValues(int enemyMax, int speed, int upgradeIntervalSec, bool rewardOn);

signals:
    void settingsApplied(int enemyMax, int speed, int upgradeIntervalSec, bool rewardOn);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onDefaultClicked();
    void onEnemyMaxChanged(int value);
    void onSpeedChanged(int value);
    void onUpgradeIntervalChanged(int value);
    void onRewardToggled(bool checked);

private:
    QSlider *m_enemyMaxSlider;
    QSlider *m_speedSlider;
    QSlider *m_upgradeIntervalSlider;
    QLabel *m_enemyMaxLabel;
    QLabel *m_speedLabel;
    QLabel *m_upgradeIntervalLabel;
    QCheckBox *m_rewardCheckBox;

    QPushButton *okBtn, *cancelBtn, *defaultBtn;

    int m_originalEnemyMax, m_originalSpeed, m_originalUpgradeInterval;
    bool m_originalRewardOn;
    bool m_valuesChanged;

    QSoundEffect *m_hoverSoundEffect;
    QSoundEffect *m_clickSoundEffect;

    void setupUi();
};

#endif // SPACESETTINGSDIALOG_H