/********************************************************************************
** Form generated from reading UI file 'klogoturtle.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KLOGOTURTLE_H
#define UI_KLOGOTURTLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KlogoTurtleApp
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *KlogoTurtleApp)
    {
        if (KlogoTurtleApp->objectName().isEmpty())
            KlogoTurtleApp->setObjectName(QStringLiteral("KlogoTurtleApp"));
        KlogoTurtleApp->resize(829, 676);
        centralWidget = new QWidget(KlogoTurtleApp);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        KlogoTurtleApp->setCentralWidget(centralWidget);

        retranslateUi(KlogoTurtleApp);

        QMetaObject::connectSlotsByName(KlogoTurtleApp);
    } // setupUi

    void retranslateUi(QMainWindow *KlogoTurtleApp)
    {
        KlogoTurtleApp->setWindowTitle(QApplication::translate("KlogoTurtleApp", "KlogoTurtleApp", 0));
    } // retranslateUi

};

namespace Ui {
    class KlogoTurtleApp: public Ui_KlogoTurtleApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KLOGOTURTLE_H
