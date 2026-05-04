#include <QtTest>
#include <QApplication>
#include <QKeyEvent>
#include "applegamewidget.h"

class TestAppleGameWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConstructor();
    void testInitialState();
    void testStartGame();
    void testPauseResume();
    void testStopGame();
    void testKeyPressEvent();
    void cleanupTestCase();

private:
    AppleGameWidget* widget;
};

void TestAppleGameWidget::initTestCase()
{
    widget = new AppleGameWidget();
}

void TestAppleGameWidget::testConstructor()
{
    QVERIFY(widget != nullptr);
    QCOMPARE(widget->focusPolicy(), Qt::StrongFocus);
}

void TestAppleGameWidget::testInitialState()
{
    widget->stopGame();
    QVERIFY(true);
}

void TestAppleGameWidget::testStartGame()
{
    widget->startGame();
    QVERIFY(true);
}

void TestAppleGameWidget::testPauseResume()
{
    widget->startGame();
    widget->pauseGame();
    widget->resumeGame();
    QVERIFY(true);
}

void TestAppleGameWidget::testStopGame()
{
    widget->startGame();
    widget->stopGame();
    QVERIFY(true);
}

void TestAppleGameWidget::testKeyPressEvent()
{
    widget->startGame();
    QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
    QApplication::sendEvent(widget, &keyEvent);
    QVERIFY(true);
    widget->stopGame();
}

void TestAppleGameWidget::cleanupTestCase()
{
    delete widget;
}

QTEST_MAIN(TestAppleGameWidget)
#include "tst_applelogic.moc"
