// filename: tst_buttonwithtext.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-05-05
// description: Unit tests for ButtonWithText

#include <QtTest>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include "buttonwithtext.h"

class TestButtonWithText : public QObject
{
    Q_OBJECT

public:

private slots:
    void initTestCase();
    void testConstructor();
    void testSetButtonSizeInt();
    void testSetButtonSizeIntInt();
    void testSetSpacing();
    void testSetLabelStyle();
    void testGetLabel();
    void testEventFilterClick();
    void cleanupTestCase();

private:
    ButtonWithText* btn;
    QWidget* parent;
};

void TestButtonWithText::initTestCase()
{
    parent = new QWidget();
    btn = new ButtonWithText(":/nonexistent.png", tr("Test Label"), parent);
}

void TestButtonWithText::testConstructor()
{
    QVERIFY(btn != nullptr);
    QVERIFY(btn->getLabel() != nullptr);
    QCOMPARE(btn->getLabel()->text(), tr("Test Label"));
    QCOMPARE(btn->text(), QString(""));
    QVERIFY(btn->testAttribute(Qt::WA_Hover));
}

void TestButtonWithText::testSetButtonSizeInt()
{
    btn->setButtonSize(100);
    QCOMPARE(btn->width(), 100);
    QCOMPARE(btn->height(), 100);
}

void TestButtonWithText::testSetButtonSizeIntInt()
{
    btn->setButtonSize(200, 150);
    QCOMPARE(btn->width(), 200);
    QCOMPARE(btn->height(), 150);
}

void TestButtonWithText::testSetSpacing()
{
    btn->setSpacing(10);
    QVERIFY(true); // no crash
}

void TestButtonWithText::testSetLabelStyle()
{
    QString style = "color: red;";
    btn->setLabelStyle(style);
    QCOMPARE(btn->getLabel()->styleSheet(), style);
}

void TestButtonWithText::testGetLabel()
{
    QLabel* label = btn->getLabel();
    QVERIFY(label != nullptr);
    QCOMPARE(label->text(), tr("Test Label"));
}

void TestButtonWithText::testEventFilterClick()
{
    QLabel* label = btn->getLabel();
    bool clicked = false;
    QObject::connect(btn, &QPushButton::clicked, [&clicked]() {
        clicked = true;
    });
    QMouseEvent pressEvent(QEvent::MouseButtonPress, QPointF(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(label, &pressEvent);
    QVERIFY(clicked);
}

void TestButtonWithText::cleanupTestCase()
{
    delete parent;
}

QTEST_MAIN(TestButtonWithText)
#include "tst_buttonwithtext.moc"
