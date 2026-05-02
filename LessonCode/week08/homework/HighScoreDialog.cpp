// HighScoreDialog.cpp
#include "highscoredialog.h"
#include "tristatebutton.h"
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QDebug>

// ========== 常量 ==========
constexpr int MAX_DISPLAY_SCORES = 9;
constexpr double SCORE_X_RATIO = 250.0 / 800.0;
constexpr double SCORE_Y_BASE_RATIO = 154.0 / 600.0;
constexpr double SCORE_Y_STEP_RATIO = 36.0 / 600.0;
constexpr double SCORE_ITEM_WIDTH_RATIO = 345.0 / 800.0;
constexpr double SCORE_ITEM_HEIGHT_RATIO = 33.0 / 600.0;
// =========================


HighScoreDialog::HighScoreDialog(const QString& scoreFilePath, QWidget* parent)
    : QDialog(parent), m_filePath(scoreFilePath)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
    m_background.load(":/res/image/Space/Images/SPACE_HISCORE_BG.png");

    QScreen* screen = QApplication::primaryScreen();
    setFixedSize(screen->size());

    // 右下角返回按钮（手动定位）
    m_returnBtn = new TriStateButton(":/res/image/Space/Images/SPACE_RETURN.png", this);
    m_returnBtn->setFixedSize(height() * 0.36, height() * 0.06);
    m_returnBtn->move(width() - m_returnBtn->width() - 50, height() - m_returnBtn->height() - 30);

    QString hover_sound = QString("qrc:/res/image/Common/Sounds/ANIBTN_ENTER.wav");
    QString click_sound = QString("qrc:/res/image/Common/Sounds/BTN_CLICK.wav");

    m_returnBtn->setHoverSound(hover_sound);
    m_returnBtn->setPressedSound(click_sound);


    connect(m_returnBtn, &QPushButton::clicked, this, &HighScoreDialog::onReturnClicked);

    loadScores();
}

void HighScoreDialog::loadScores()
{
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_entries.clear();
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList parts = line.split(":");
            if (parts.size() == 2) {
                bool ok;
                int score = parts[1].toInt(&ok);
                if (ok)
                    m_entries.append({ parts[0], score });
            }
        }
        file.close();

        
    }

    // 排序降序
    std::sort(m_entries.begin(), m_entries.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });
    if (m_entries.size() > MAX_DISPLAY_SCORES)
        m_entries = m_entries.mid(0, MAX_DISPLAY_SCORES);

    update(); // 触发重绘
}

void HighScoreDialog::saveScores()
{
    QFile file(m_filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& entry : m_entries) {
            out << entry.first << ":" << entry.second << "\n";
        }
        file.close();
    }
}

void HighScoreDialog::addScore(const QString& name, int score)
{
    m_entries.append({ name, score });
    std::sort(m_entries.begin(), m_entries.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });
    if (m_entries.size() > MAX_DISPLAY_SCORES)
        m_entries = m_entries.mid(0, MAX_DISPLAY_SCORES);
    saveScores();
    update(); // 触发重绘
}

void HighScoreDialog::onReturnClicked()
{
    accept();
}

void HighScoreDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景
    if (!m_background.isNull())
        painter.drawPixmap(rect(), m_background);
    else
        painter.fillRect(rect(), QColor(20, 20, 80, 200));

    // 设置文字样式
    QFont font("Arial",32,  QFont::Bold);
   
    painter.setFont(font);
    painter.setPen(QColor(0, 255, 0)); // 绿色

    // 计算绘制区域
    int w = width();
    int h = height();

    // 绘制分数列表
    painter.setFont(font);

    for (int i = 0; i < MAX_DISPLAY_SCORES; ++i) {
        int x = w * SCORE_X_RATIO;
        int y = h * (SCORE_Y_BASE_RATIO + SCORE_Y_STEP_RATIO * i);
        int itemWidth = w * SCORE_ITEM_WIDTH_RATIO;
        int itemHeight = h * SCORE_ITEM_HEIGHT_RATIO;

        if (i < m_entries.size()) {
            // 排名和名字（左对齐）
            painter.setPen(QColor(0, 255, 0)); // 绿色
            QString nameText = QString("%1").arg(m_entries[i].first);
            QString scoreText = QString::number(m_entries[i].second);
            QString text = QString(nameText + ":" + scoreText);
            painter.drawText(x, y, itemWidth * 0.6, itemHeight,
                Qt::AlignLeft | Qt::AlignVCenter, text);

           
        }
        else {
            // 空排名
            painter.setPen(QColor(100, 100, 100)); // 灰色表示空位
            painter.drawText(x, y, itemWidth, itemHeight,
                Qt::AlignLeft | Qt::AlignVCenter,
                QString("---"));
        }
    }
}

void HighScoreDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);

    // 更新返回按钮位置
    if (m_returnBtn) {
        m_returnBtn->move(width() - m_returnBtn->width() - 50,
            height() - m_returnBtn->height() - 30);
    }
}