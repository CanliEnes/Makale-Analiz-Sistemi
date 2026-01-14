/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lblToplamRef;
    QLabel *lblToplamMakale;
    QLabel *lblEnCokAtif;
    QLineEdit *txtMakaleId;
    QPushButton *btnHesapla;
    QLabel *lblHIndex;
    QLabel *lblHMedian;
    QPushButton *btnGenelGoster;
    QPushButton *btnAnalizYap;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1522, 791);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lblToplamRef = new QLabel(centralwidget);
        lblToplamRef->setObjectName("lblToplamRef");
        lblToplamRef->setGeometry(QRect(1220, 0, 121, 31));
        lblToplamMakale = new QLabel(centralwidget);
        lblToplamMakale->setObjectName("lblToplamMakale");
        lblToplamMakale->setGeometry(QRect(1220, 30, 241, 20));
        lblEnCokAtif = new QLabel(centralwidget);
        lblEnCokAtif->setObjectName("lblEnCokAtif");
        lblEnCokAtif->setGeometry(QRect(1220, 50, 601, 21));
        txtMakaleId = new QLineEdit(centralwidget);
        txtMakaleId->setObjectName("txtMakaleId");
        txtMakaleId->setGeometry(QRect(1240, 370, 281, 24));
        btnHesapla = new QPushButton(centralwidget);
        btnHesapla->setObjectName("btnHesapla");
        btnHesapla->setGeometry(QRect(1240, 400, 121, 31));
        lblHIndex = new QLabel(centralwidget);
        lblHIndex->setObjectName("lblHIndex");
        lblHIndex->setGeometry(QRect(1220, 80, 171, 16));
        lblHMedian = new QLabel(centralwidget);
        lblHMedian->setObjectName("lblHMedian");
        lblHMedian->setGeometry(QRect(1220, 110, 171, 20));
        btnGenelGoster = new QPushButton(centralwidget);
        btnGenelGoster->setObjectName("btnGenelGoster");
        btnGenelGoster->setGeometry(QRect(1240, 440, 281, 24));
        btnAnalizYap = new QPushButton(centralwidget);
        btnAnalizYap->setObjectName("btnAnalizYap");
        btnAnalizYap->setGeometry(QRect(1360, 400, 171, 31));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 1211, 731));
        MainWindow->setCentralWidget(centralwidget);
        graphicsView->raise();
        lblToplamRef->raise();
        lblToplamMakale->raise();
        lblEnCokAtif->raise();
        txtMakaleId->raise();
        btnHesapla->raise();
        lblHIndex->raise();
        lblHMedian->raise();
        btnGenelGoster->raise();
        btnAnalizYap->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1522, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lblToplamRef->setText(QCoreApplication::translate("MainWindow", "ToplamRef:", nullptr));
        lblToplamMakale->setText(QCoreApplication::translate("MainWindow", "ToplamMakale:", nullptr));
        lblEnCokAtif->setText(QCoreApplication::translate("MainWindow", "EnCokAtif:", nullptr));
        btnHesapla->setText(QCoreApplication::translate("MainWindow", "H-Core Analiz", nullptr));
        lblHIndex->setText(QCoreApplication::translate("MainWindow", "HIndex:", nullptr));
        lblHMedian->setText(QCoreApplication::translate("MainWindow", "HMedian:", nullptr));
        btnGenelGoster->setText(QCoreApplication::translate("MainWindow", "Genel G\303\266r\303\274n\303\274m", nullptr));
        btnAnalizYap->setText(QCoreApplication::translate("MainWindow", "K-core Analiz", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
