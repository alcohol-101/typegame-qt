// filename: nameinputdialog.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-04
// description: Implementation of NameInputDialog

#include "nameinputdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

NameInputDialog::NameInputDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Dialog);
    
    setFixedSize(400,200);
    setWindowTitle(tr("高分英雄"));
    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* layout1 = new QHBoxLayout();
    layout->setContentsMargins(30, 30, 30, 30);
    QLabel* label = new QLabel(tr("请输入你的名字:"), this);
    label->setStyleSheet("color: black; font-size: 18px; background: transparent;");
    layout1->addWidget(label);

    QVBoxLayout* btnLayout = new QVBoxLayout;
    m_okBtn = new QPushButton(tr("确定"),this);
    m_okBtn->setFixedSize(92,46);
    
    m_cancelBtn = new QPushButton(tr("取消"),this);
    m_cancelBtn->setFixedSize(92, 46);
   
    btnLayout->addStretch();
    btnLayout->addWidget(m_okBtn);
    btnLayout->addWidget(m_cancelBtn);
    btnLayout->addStretch();
    layout1->addLayout(btnLayout);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setStyleSheet("background: white; font-size: 18px;");

    layout->addLayout(layout1);
    layout->addWidget(m_lineEdit);
    layout->addStretch();

    connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString NameInputDialog::name() const
{
    return m_lineEdit->text().trimmed();
}

void NameInputDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
   
        painter.fillRect(rect(), QColor(255,255,255));
}