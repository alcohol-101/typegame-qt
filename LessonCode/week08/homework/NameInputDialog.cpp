// NameInputDialog.cpp
#include "NameInputDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

NameInputDialog::NameInputDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog);
    
    setFixedSize(400,200);
    setWindowTitle("高分英雄");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout* layout_1 = new QHBoxLayout();
    layout->setContentsMargins(30, 30, 30, 30);
    QLabel *label = new QLabel("请输入你的名字:", this);
    label->setStyleSheet("color: black; font-size: 18px; background: transparent;");
    layout_1->addWidget(label);

    QVBoxLayout *btnLayout = new QVBoxLayout;
    m_okBtn = new QPushButton("OK",this);
    m_okBtn->setFixedSize(92,46);
    
    m_cancelBtn = new QPushButton("Cancel",this);
    m_cancelBtn->setFixedSize(92, 46);
   
    btnLayout->addStretch();
    btnLayout->addWidget(m_okBtn);
    btnLayout->addWidget(m_cancelBtn);
    btnLayout->addStretch();
    layout_1->addLayout(btnLayout);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setStyleSheet("background: white; font-size: 18px;");

    layout->addLayout(layout_1);
    layout->addWidget(m_lineEdit);
    layout->addStretch();

    connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString NameInputDialog::name() const
{
    return m_lineEdit->text().trimmed();
}

void NameInputDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
   
        painter.fillRect(rect(), QColor(255,255,255));
}