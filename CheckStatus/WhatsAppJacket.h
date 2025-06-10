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
#include <QFile.h>


class WhatsAppJacket : public QObject
{
	Q_OBJECT

public:
	WhatsAppJacket(QObject* parent = nullptr);
	QString getTokenFromFile();
	QString getChatIdFromFile();

public slots:
	void sendMessage(const QString message);

private:
	QNetworkAccessManager* manager = nullptr;
	QString chatId = "";
	QString urlString = "";
};