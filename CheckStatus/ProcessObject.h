#pragma once

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <windows.h>
#include <tlhelp32.h>


class ProcessObject : public QObject
{
	Q_OBJECT

public:
	ProcessObject(QObject* parent = nullptr);

	void setParam(QString name, QString URL, QString updateSecond, bool checkParse, bool checkSend);
	void check();
	QString getStartString(QString any);

signals:
	void messageReceived(const QString&);

private:
	void classTimerIsDone();

private:
	QString m_name;
	QString m_URL;
	QString m_updateSecond;
	bool m_checkParse = false;
	bool m_checkSend = false;
	QTimer* classTimer = nullptr;
};
