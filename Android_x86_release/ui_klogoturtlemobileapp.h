/********************************************************************************
** Form generated from reading UI file 'klogoturtlemobileapp.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KLOGOTURTLEMOBILEAPP_H
#define UI_KLOGOTURTLEMOBILEAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KlogoTurtleMobileApp
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *KlogoTurtleMobileApp)
    {
        if (KlogoTurtleMobileApp->objectName().isEmpty())
            KlogoTurtleMobileApp->setObjectName(QStringLiteral("KlogoTurtleMobileApp"));
        KlogoTurtleMobileApp->resize(800, 600);
        centralwidget = new QWidget(KlogoTurtleMobileApp);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        KlogoTurtleMobileApp->setCentralWidget(centralwidget);
        menubar = new QMenuBar(KlogoTurtleMobileApp);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        KlogoTurtleMobileApp->setMenuBar(menubar);
        statusbar = new QStatusBar(KlogoTurtleMobileApp);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        KlogoTurtleMobileApp->setStatusBar(statusbar);

        retranslateUi(KlogoTurtleMobileApp);

        QMetaObject::connectSlotsByName(KlogoTurtleMobileApp);
    } // setupUi

    void retranslateUi(QMainWindow *KlogoTurtleMobileApp)
    {
        KlogoTurtleMobileApp->setWindowTitle(QApplication::translate("KlogoTurtleMobileApp", "MainWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class KlogoTurtleMobileApp: public Ui_KlogoTurtleMobileApp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KLOGOTURTLEMOBILEAPP_H
