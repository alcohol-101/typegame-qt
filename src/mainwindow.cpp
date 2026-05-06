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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)
    
{
    QScreen* screen = QApplication::primaryScreen();
    QRect fullGeometry = screen->geometry();
    double w_primary = fullGeometry.width();
    double h_primary = fullGeometry.height();
    this->resize(w_primary * 147.0 / 256.0, h_primary * 1045.0 / 1440.0);

    setWindowFlags(Qt::FramelessWindowHint);
    QWidget *centralContainer = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    centralContainer->setStyleSheet(R"(border-radius: 5px;)");

    titleBar = new QWidget();
    titleBar->setObjectName("titleBar");
    titleBar->setStyleSheet("QWidget#titleBar { background-color: #58a2ff; }");
    titleBar->setFixedHeight(this->height()*0.14832);

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(10);

    QLabel *iconLabel = new QLabel(titleBar);
    QPixmap pixmap(":/res/image/title_left.png");
    if (!pixmap.isNull()) {
        iconLabel->setPixmap(pixmap);
    } else {
        iconLabel->setText(tr("加载失败"));
        iconLabel->setStyleSheet("font-size: 18px;");
    }
    titleLayout->addWidget(iconLabel);

    QLabel *titleLabel = new QLabel( titleBar);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(50);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    titleLayout->addWidget(titleLabel);

    titleLayout->addStretch();


    QPushButton *minimizeBtn = new QPushButton(QChar(0x2014), titleBar);
    minimizeBtn->setFixedSize(40, 30);
    minimizeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; font-size: 25px; border: none;font-weight: bold; }"
        "QPushButton:hover { background-color: #d9e7ff; }"
        );

    QPushButton *maximizeBtn = new QPushButton(QChar(0x25A1),titleBar);
    maximizeBtn->setFixedSize(40,30);
    maximizeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; font-size:25px; border: none; font-weight: bold; }"
        "QPushButton:hover { background-color: #d9e7ff;  }"
        );

    QPushButton *closeBtn = new QPushButton(QChar(0x00D7), titleBar);
    closeBtn->setFixedSize(50, 30);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: white; font-size:25px; border: none; font-weight: bold;}"
        "QPushButton:hover { background-color: #e74c3c; }"
        );

    QVBoxLayout *three_v=new QVBoxLayout();
    QHBoxLayout *three_h=new QHBoxLayout();


    three_h->addWidget(minimizeBtn);
    three_h->addWidget(maximizeBtn);
    three_h->addWidget(closeBtn);
    three_v->addLayout(three_h);
    three_v->addStretch();

    titleLayout->addLayout(three_v);

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

    mainLayout->addWidget(titleBar, 14.84);
    setCentralWidget(centralContainer);
    QTimer::singleShot(0, this, &MainWindow::setupContent);
}

void MainWindow::setupContent()
{
    QScreen* screen = QApplication::primaryScreen();
    QRect fullGeometry = screen->geometry();
    double w_primary = fullGeometry.width();
    double h_primary = fullGeometry.height();

    QWidget* cw = centralWidget();
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(cw->layout());

    QWidget *contentArea = new QWidget();
    contentArea->setStyleSheet("QWidget { background-color: #edf7ff; }");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentArea);
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
    btn1->setButtonSize(250.0/2560.0*w_primary,270.0/1440.0*h_primary);

    photo=":/res/image/mouse.png";
    ButtonWithText* btn2 = new ButtonWithText(photo, tr("鼠的故事"));
    btn2->setButtonSize(250.0 / 2560.0 * w_primary, 270.0 / 1440.0 * h_primary);

    photo=":/res/image/apple.png";
    ButtonWithText* btn3 = new ButtonWithText(photo, tr("拯救苹果"));
    btn3->setButtonSize(250.0 / 2560.0 * w_primary, 270.0 / 1440.0 * h_primary);

    photo=":/res/image/space.png";
    ButtonWithText* btn4 = new ButtonWithText(photo, tr("太空大战"));
    btn4->setButtonSize(250.0 / 2560.0 * w_primary, 270.0 / 1440.0 * h_primary);

    photo=":/res/image/water.png";
    ButtonWithText* btn5 = new ButtonWithText(photo, tr("激流勇进"));
    btn5->setButtonSize(250.0 / 2560.0 * w_primary, 270.0 / 1440.0 * h_primary);

    QObject::connect(btn1, &QPushButton::clicked, []() {
        qDebug() << "生死时速被点击！";
    });

    QObject::connect(btn2, &QPushButton::clicked, []() {
        qDebug() << "鼠的故事被点击！";
    });

    QObject::connect(btn3, &QPushButton::clicked, [this]() {
        qDebug() << "拯救苹果被点击！";
        if (!ensureGameResources())
            return;
        AppleGameWidget* gameWidget = new AppleGameWidget();
        gameWidget->show();
        QObject::connect(gameWidget, &QWidget::destroyed, this, [this]() {
            this->show();
            });
        });

    QObject::connect(btn4, &QPushButton::clicked, [this]() {
        qDebug() << "太空大战被点击！";
        if (!ensureGameResources())
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

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint localPos = titleBar->mapFromParent(event->pos());

        if (titleBar->rect().contains(localPos)) {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
            m_bDrag = true;
            event->accept();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_bDrag) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_bDrag = false;
    }
}
