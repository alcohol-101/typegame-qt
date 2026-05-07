// filename: mainwindow.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Implementation of MainWindow

#include "mainwindow.h"

#include<QLayout>
#include<QLabel>
#include<QPushButton>
#include<QTextEdit>
#include<QStringList>
#include<QDebug>
#include<QScreen>
#include<QApplication>
#include<QTimer>

#include "buttonwithtext.h"
#include "applegamewidget.h"
#include "spacewarwidget.h"
#include "respath.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(nullptr)
    
{
    QScreen* screen = QApplication::primaryScreen();
    QRect fullGeometry = screen->geometry();
    double wPrimary = fullGeometry.width();
    double hPrimary = fullGeometry.height();
    this->resize(wPrimary * 147.0 / 256.0, hPrimary * 1045.0 / 1440.0);

    setWindowFlags(Qt::FramelessWindowHint);
    QWidget* centralContainer = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    centralContainer->setStyleSheet(R"(border-radius: 5px;)");

    m_titleBar = new QWidget();
    m_titleBar->setObjectName("titleBar");
    m_titleBar->setStyleSheet("QWidget#titleBar { background-color: #58a2ff; }");
    m_titleBar->setFixedHeight(this->height()*0.14832);

    QHBoxLayout* titleLayout = new QHBoxLayout(m_titleBar);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(10);

    QLabel* iconLabel = new QLabel(m_titleBar);
    QPixmap pixmap(":/res/image/title_left.png");
    if (!pixmap.isNull()) {
        iconLabel->setPixmap(pixmap);
    } else {
        iconLabel->setText(tr("加载失败"));
        iconLabel->setStyleSheet("font-size: 18px;");
    }
    titleLayout->addWidget(iconLabel);

    QLabel* titleLabel = new QLabel( m_titleBar);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(50);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    titleLayout->addWidget(titleLabel);

    titleLayout->addStretch();

    QPushButton* minimizeBtn = new QPushButton(QChar(0x2014), m_titleBar);
    minimizeBtn->setFixedSize(40, 30);
    minimizeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; font-size: 25px; border: none;font-weight: bold; }"
        "QPushButton:hover { background-color: #d9e7ff; }"
        );

    QPushButton* maximizeBtn = new QPushButton(QChar(0x25A1),m_titleBar);
    maximizeBtn->setFixedSize(40,30);
    maximizeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; font-size:25px; border: none; font-weight: bold; }"
        "QPushButton:hover { background-color: #d9e7ff;  }"
        );

    QPushButton* closeBtn = new QPushButton(QChar(0x00D7), m_titleBar);
    closeBtn->setFixedSize(50, 30);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; font-size:25px; border: none; font-weight: bold;}"
        "QPushButton:hover { background-color: #e74c3c; }"
        );

    QVBoxLayout* threeV=new QVBoxLayout();
    QHBoxLayout* threeH=new QHBoxLayout();


    threeH->addWidget(minimizeBtn);
    threeH->addWidget(maximizeBtn);
    threeH->addWidget(closeBtn);
    threeV->addLayout(threeH);
    threeV->addStretch();

    titleLayout->addLayout(threeV);

    connect(closeBtn, &QPushButton::clicked, []() {
             exit(0); 
         });
    connect(minimizeBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    connect(maximizeBtn, &QPushButton::clicked, [this, maximizeBtn]() {
        if (isMaximized()) {
            showNormal();
            maximizeBtn->setText(QChar(0x25A1));
        } else {
            showMaximized();
            maximizeBtn->setText(QChar(0x2750));
        }
    });

    mainLayout->addWidget(m_titleBar, 14.84);
    setCentralWidget(centralContainer);
    QTimer::singleShot(0, this, &MainWindow::setupContent);
}

void MainWindow::setupContent()
{
    QScreen* screen = QApplication::primaryScreen();
    QRect fullGeometry = screen->geometry();
    double wPrimary = fullGeometry.width();
    double hPrimary = fullGeometry.height();

    QWidget* cw = centralWidget();
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(cw->layout());

    QWidget* contentArea = new QWidget();
    contentArea->setStyleSheet("QWidget { background-color: #edf7ff; }");
    QVBoxLayout* contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0,0,0,0);
    int height_content=100;
    contentLayout->addStretch(12);

    QString gameString=QChar(0x25B6);
    gameString+= tr(" 经典游戏");
    QLabel* gameLabel=new QLabel(gameString);
    QFont font;
    font.setBold(true);
    gameLabel->setFont(font);
    contentLayout->addWidget(gameLabel,5);

    QHBoxLayout* layout = new QHBoxLayout();

    QString photo(":/res/image/live.png");
    ButtonWithText* btn1 = new ButtonWithText(photo, tr("生死时速"));
    btn1->setButtonSize(250.0/2560.0*wPrimary,270.0/1440.0*hPrimary);

    photo=":/res/image/mouse.png";
    ButtonWithText* btn2 = new ButtonWithText(photo, tr("鼠的故事"));
    btn2->setButtonSize(250.0 / 2560.0 * wPrimary, 270.0 / 1440.0 * hPrimary);

    photo=":/res/image/apple.png";
    ButtonWithText* btn3 = new ButtonWithText(photo, tr("拯救苹果"));
    btn3->setButtonSize(250.0 / 2560.0 * wPrimary, 270.0 / 1440.0 * hPrimary);

    photo=":/res/image/space.png";
    ButtonWithText* btn4 = new ButtonWithText(photo, tr("太空大战"));
    btn4->setButtonSize(250.0 / 2560.0 * wPrimary, 270.0 / 1440.0 * hPrimary);

    photo=":/res/image/water.png";
    ButtonWithText* btn5 = new ButtonWithText(photo, tr("激流勇进"));
    btn5->setButtonSize(250.0 / 2560.0 * wPrimary, 270.0 / 1440.0 * hPrimary);

    QObject::connect(btn1, &QPushButton::clicked, []() {
        qDebug() << "生死时速被点击！";
    });

    QObject::connect(btn2, &QPushButton::clicked, []() {
        qDebug() << "鼠的故事被点击！";
    });

    QObject::connect(btn3, &QPushButton::clicked, [this]() {
        qDebug() << "拯救苹果被点击！";
        if (!EnsureGameResources())
            return;
        AppleGameWidget* gameWidget = new AppleGameWidget();
        gameWidget->show();
        QObject::connect(gameWidget, &QWidget::destroyed, this, [this]() {
            this->show();
            });
        });

    QObject::connect(btn4, &QPushButton::clicked, [this]() {
        qDebug() << "太空大战被点击！";
        if (!EnsureGameResources())
            return;
        SpaceWarWidget* gameWidget = new SpaceWarWidget();
        gameWidget->show();
        QObject::connect(gameWidget, &QWidget::destroyed, this, [this]() {
            this->show();
            });
        });

    QObject::connect(btn5, &QPushButton::clicked, []() {
        qDebug() << "激流勇进被点击！";
    });

    layout->addStretch();
    layout->addWidget(btn1, 0, Qt::AlignCenter);
    layout->addWidget(btn2, 0, Qt::AlignCenter);
    layout->addWidget(btn3, 0, Qt::AlignCenter);
    layout->addWidget(btn4, 0, Qt::AlignCenter);
    layout->addWidget(btn5, 0, Qt::AlignCenter);
    layout->addStretch();

    contentLayout->addLayout(layout,25);
    contentLayout->addStretch(height_content-48);

    QWidget* floor=new QWidget();
    floor->setStyleSheet("QWidget{background-color:#4f95ff}");

    int total=100.0;
    mainLayout->addWidget(contentArea,81.5);
    mainLayout->addWidget(floor,total-81.5-14.84);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint localPos = m_titleBar->mapFromParent(event->pos());

        if (m_titleBar->rect().contains(localPos)) {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
            m_bDrag = true;
            event->accept();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_bDrag) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_bDrag = false;
    }
}
