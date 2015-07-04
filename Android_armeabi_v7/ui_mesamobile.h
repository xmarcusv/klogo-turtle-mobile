/********************************************************************************
** Form generated from reading UI file 'mesamobile.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESAMOBILE_H
#define UI_MESAMOBILE_H

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

class Ui_MesaMobile
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MesaMobile)
    {
        if (MesaMobile->objectName().isEmpty())
            MesaMobile->setObjectName(QStringLiteral("MesaMobile"));
        MesaMobile->resize(800, 600);
        menubar = new QMenuBar(MesaMobile);
        menubar->setObjectName(QStringLiteral("menubar"));
        MesaMobile->setMenuBar(menubar);
        centralwidget = new QWidget(MesaMobile);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MesaMobile->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MesaMobile);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MesaMobile->setStatusBar(statusbar);

        retranslateUi(MesaMobile);

        QMetaObject::connectSlotsByName(MesaMobile);
    } // setupUi

    void retranslateUi(QMainWindow *MesaMobile)
    {
        MesaMobile->setWindowTitle(QApplication::translate("MesaMobile", "MainWindow", 0));
    } // retranslateUi

};

namespace Ui {
    class MesaMobile: public Ui_MesaMobile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESAMOBILE_H
