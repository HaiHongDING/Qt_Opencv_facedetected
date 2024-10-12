/********************************************************************************
** Form generated from reading UI file 'camra.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMRA_H
#define UI_CAMRA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_camra
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *camra)
    {
        if (camra->objectName().isEmpty())
            camra->setObjectName(QString::fromUtf8("camra"));
        camra->resize(1920, 1080);
        label = new QLabel(camra);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(550, 40, 841, 501));
        pushButton = new QPushButton(camra);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(1460, 80, 191, 191));
        pushButton->setMinimumSize(QSize(191, 191));
        pushButton->setMaximumSize(QSize(191, 191));

        retranslateUi(camra);

        QMetaObject::connectSlotsByName(camra);
    } // setupUi

    void retranslateUi(QWidget *camra)
    {
        camra->setWindowTitle(QCoreApplication::translate("camra", "Form", nullptr));
        label->setText(QCoreApplication::translate("camra", "TextLabel", nullptr));
        pushButton->setText(QCoreApplication::translate("camra", "PUSH", nullptr));
    } // retranslateUi

};

namespace Ui {
    class camra: public Ui_camra {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMRA_H
