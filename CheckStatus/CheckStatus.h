#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CheckStatus.h"

class CheckStatus : public QMainWindow
{
    Q_OBJECT

public:
    CheckStatus(QWidget *parent = nullptr);
    ~CheckStatus();

private:
    Ui::CheckStatusClass ui;
};
