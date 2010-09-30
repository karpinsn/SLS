/********************************************************************************
** Form generated from reading UI file 'Holoencoder.ui'
**
** Created: Fri Aug 27 10:24:35 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOLOENCODER_H
#define UI_HOLOENCODER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpenXYZM;
    QAction *actionOpen_Holovideo;
    QWidget *MainWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QWidget *glView;
    QVBoxLayout *verticalLayout_8;
    QListWidget *fileList;
    QPushButton *encodeButton;
    QMenuBar *menubar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(708, 560);
        actionOpenXYZM = new QAction(MainWindow);
        actionOpenXYZM->setObjectName(QString::fromUtf8("actionOpenXYZM"));
        actionOpen_Holovideo = new QAction(MainWindow);
        actionOpen_Holovideo->setObjectName(QString::fromUtf8("actionOpen_Holovideo"));
        MainWidget = new QWidget(MainWindow);
        MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        horizontalLayout_2 = new QHBoxLayout(MainWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        glView = new QWidget(MainWidget);
        glView->setObjectName(QString::fromUtf8("glView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(glView->sizePolicy().hasHeightForWidth());
        glView->setSizePolicy(sizePolicy);
        glView->setMinimumSize(QSize(512, 512));
        glView->setMaximumSize(QSize(512, 512));

        horizontalLayout_3->addWidget(glView);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        fileList = new QListWidget(MainWidget);
        fileList->setObjectName(QString::fromUtf8("fileList"));

        verticalLayout_8->addWidget(fileList);

        encodeButton = new QPushButton(MainWidget);
        encodeButton->setObjectName(QString::fromUtf8("encodeButton"));

        verticalLayout_8->addWidget(encodeButton);


        horizontalLayout_3->addLayout(verticalLayout_8);


        horizontalLayout_2->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(MainWidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 708, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpenXYZM);
        menuFile->addAction(actionOpen_Holovideo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpenXYZM->setText(QApplication::translate("MainWindow", "Open XYZ", 0, QApplication::UnicodeUTF8));
        actionOpen_Holovideo->setText(QApplication::translate("MainWindow", "Open Holovideo", 0, QApplication::UnicodeUTF8));
        MainWidget->setWindowTitle(QApplication::translate("MainWindow", "Holoprojector", 0, QApplication::UnicodeUTF8));
        encodeButton->setText(QApplication::translate("MainWindow", "Encode Geometery", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOLOENCODER_H
