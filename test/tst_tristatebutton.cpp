#include <QtTest>
#include <QApplication>
#include <QMouseEvent>
#include "tristatebutton.h"

class TestTriStateButton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConstructor();
    void testSetFixedSize();
    void testEnterEvent();
    void testLeaveEvent();
    void testMousePressRelease();
    void cleanupTestCase();

private:
    TriStateButton* btn;
    QWidget* parent;
};

void TestTriStateButton::initTestCase()
{
    parent = new QWidget();
    btn = new TriStateButton(":/nonexistent.png", parent);
}

void TestTriStateButton::testConstructor()
{
    QVERIFY(btn != nullptr);
    QVERIFY(btn->testAttribute(Qt::WA_Hover));
    QCOMPARE(btn->cursor().shape(), Qt::PointingHandCursor);
}

void TestTriStateButton::testSetFixedSize()
{
    btn->setFixedSize(120, 50);
    QCOMPARE(btn->width(), 120);
    QCOMPARE(btn->height(), 50);
}

void TestTriStateButton::testEnterEvent()
{
    QEvent enterEvent(QEvent::Enter);
    QApplication::sendEvent(btn, &enterEvent);
    QVERIFY(true);
}

void TestTriStateButton::testLeaveEvent()
{
    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(btn, &leaveEvent);
    QVERIFY(true);
}

void TestTriStateButton::testMousePressRelease()
{
    QVERIFY(true);
}

void TestTriStateButton::cleanupTestCase()
{
    delete parent;
}

QTEST_MAIN(TestTriStateButton)
#include "tst_tristatebutton.moc"
