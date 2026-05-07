// filename: tst_nameinputdialog.cpp
// creator: alcohol-101@users.noreply.github.com
// date: 2026-05-05
// description: Unit tests for NameInputDialog

#include <QtTest>
#include <QApplication>
#include <QLineEdit>
#include "nameinputdialog.h"

class TestNameInputDialog : public QObject
{
    Q_OBJECT

public:

private slots:
    void testConstructor();
    void testNameEmpty();
    void testNameWithText();
};

void TestNameInputDialog::testConstructor()
{
    NameInputDialog dlg;
    QVERIFY(dlg.windowTitle() == tr("高分英雄"));
}

void TestNameInputDialog::testNameEmpty()
{
    NameInputDialog dlg;
    QCOMPARE(dlg.name(), QString(""));
}

void TestNameInputDialog::testNameWithText()
{
    NameInputDialog dlg;
    QLineEdit* edit = dlg.findChild<QLineEdit*>();
    QVERIFY(edit != nullptr);
    edit->setText("PlayerOne");
    QCOMPARE(dlg.name(), QString("PlayerOne"));
}

QTEST_MAIN(TestNameInputDialog)
#include "tst_nameinputdialog.moc"
