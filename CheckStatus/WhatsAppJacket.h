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

public:
	QString token = ""; // SolovevAA617AvitoParser
	const QString chatId = "79825313114@c.us";

private:
	QNetworkAccessManager* manager = nullptr;
};