#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CheckStatus.h"

#include <ProcessObject.h>
#include "WhatsAppJacket.h"

#include <QFileDialog>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamAttribute>
#include <QMouseEvent>

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include <windows.h>

class CheckStatus : public QMainWindow
{
    Q_OBJECT

public:

    CheckStatus(QWidget* parent = nullptr);
    ~CheckStatus();

    void addItemInList();
    void deleteItemInList();
    void exportXml();
    void recursionXmlWriter(QTreeWidgetItem* some, QXmlStreamWriter& someXmlWriter);
    void importXml();
    void loopXmlReader(QXmlStreamReader& xmlReader);
    void startingImportXml();
    void mousePressEvent(QMouseEvent* event);
    void initializationPoolFunc();

    void setData();
    void closeEditor(QTreeWidgetItem* any);
    void otherItemWasChecked(QTreeWidgetItem* any);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void cmdOpen();
    void cmdClose();

private:
    Ui::CheckStatusClass ui;
    bool offChanger = false;
    bool sortBool = true;

    int middleColumn = 0;
    int countRow = 1;

    QTreeWidgetItem* middleItem = nullptr;
    QStatusBar* sBar;

    QString line;

    QList<QSharedPointer<ProcessObject>>poolParse;
    WhatsAppJacket* tgObject = nullptr;

    int countOfTopItems;


    QSystemTrayIcon* trayIcon = nullptr;
    QDateTime fullTimeWork;

    bool windowShow = false;
};

