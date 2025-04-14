#pragma once

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QUrlQuery>
#include <windows.h>


class WhatsAppJacket : public QObject
{
	Q_OBJECT

public:
	WhatsAppJacket(QObject* parent = nullptr);

public slots:
	void sendMessage(const QString message);

private:
	QNetworkAccessManager* manager = nullptr;
	//const QString chatId = "79825313114@c.us";
	const QString chatId = "120363044487282624@g.us";
};