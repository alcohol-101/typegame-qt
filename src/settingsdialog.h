// filename: settingsdialog.h
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Apple game settings dialog (speed, targets, audio)

#ifndef _TYPEGAME_SETTINGSDIALOG_H_
#define _TYPEGAME_SETTINGSDIALOG_H_

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSoundEffect>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    void setCurrentValues(int level, int target, int maxBad, int maxApple, bool soundEnabled);

signals:
    void settingsApplied(int level, int targetCount, int maxBadCount, int maxAppleCount, bool soundEnabled);

protected:
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onDefaultClicked();
    void onLevelChanged(int value);
    void onTargetChanged(int value);
    void onBadChanged(int value);
    void onMaxAppleChanged(int value);
    void onSoundToggled(bool checked);

private:
    QSlider* m_levelSlider;
    QSlider* m_targetSlider;
    QSlider* m_badSlider;
    QSlider* m_maxAppleSlider;

    QLabel* m_levelValueLabel;
    QLabel* m_targetValueLabel;
    QLabel* m_badValueLabel;
    QLabel* m_maxAppleValueLabel;

    QCheckBox* m_soundCheckBox;

    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
    QPushButton* m_defaultBtn;

    int m_originalLevel;
    int m_originalTarget;
    int m_originalBad;
    int m_originalMaxApple;
    bool m_originalSoundEnabled;
    bool m_valuesChanged;

    QSoundEffect* m_hoverSoundEffect;
    QSoundEffect* m_clickSoundEffect;

    void setupUi();
};

#endif // _TYPEGAME_SETTINGSDIALOG_H_