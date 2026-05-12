#include "WhatsAppJacket.h"

WhatsAppJacket::WhatsAppJacket(QObject* parent)
	: QObject(parent), manager(new QNetworkAccessManager)
{
	AttachConsole(ATTACH_PARENT_PROCESS);
	urlString = QString(getTokenFromFile());
	chatId = getChatIdFromFile();

	connect(this, &WhatsAppJacket::sendIdNotificationForDelete, this, &WhatsAppJacket::deleteNotification);

	QTimer::singleShot(2000, [this]() { getLastMessageAsync(); });
}



void WhatsAppJacket::sendMessage(const QString message)
{
	if (message.isEmpty()) {
		qWarning() << "Attempt to send empty message";
		return;
	}

	QUrl url(urlString);

	QJsonObject json;
	json["chatId"] = chatId;
	json["message"] = message; // Используем переданное сообщение

	// Преобразование JSON-объекта в строку
	QJsonDocument jsonDoc(json);
	QByteArray jsonData = jsonDoc.toJson();

	// Создание запроса
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// Отправка запроса
	QNetworkReply* reply = manager->post(request, jsonData);

	// Обработчик ответа (если необходимо). Пригодится.
	QObject::connect(reply, &QNetworkReply::finished, [reply]() {

		if (reply->error() == QNetworkReply::NoError)
		{
			QString response = reply->readAll();
			qDebug() << response;
		}
		else
		{
			qDebug() << "Error:: " << reply->error();
		}
		reply->deleteLater();
		});
}



QString WhatsAppJacket::getTokenFromFile()
{
	QFile file(QCoreApplication::applicationDirPath() + "\\token.txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Don't find browse file. Add a directory with a token (token.txt).";
		return 0;
	}

	QTextStream out(&file);

	QString myLine = out.readLine(); // метод readLine() считывает одну строку из потока

	if (myLine == "")
	{
		qDebug() << "Don't find browse file. Add a directory with a token (token.txt).";
		file.close();
		return 0;
	}

	file.close();

	return myLine;
}



QString WhatsAppJacket::getChatIdFromFile()
{
	QFile file(QCoreApplication::applicationDirPath() + "\\chatId.txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Don't find browse file. Add a directory with a token (chatId.txt).";
		return 0;
	}

	QTextStream out(&file);

	QString myLine = out.readLine(); // метод readLine() считывает одну строку из потока

	if (myLine == "")
	{
		qDebug() << "Don't find browse file. Add a directory with a token (chatId.txt).";
		file.close();
		return 0;
	}

	file.close();

	return myLine;
}



void WhatsAppJacket::getLastMessageAsync()
{
	if (isBusy) return;
	isBusy = true;
	QString urlStringTemp = QString("https://3100.api.green-api.com/waInstance3100514553/receiveNotification/134edc19c6c64e4f971e4578b787f54725492643c588466095");

	QUrl url(urlStringTemp);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QNetworkReply* reply = manager->get(request);

	QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
		if (reply->error() == QNetworkReply::NoError)
		{
			QByteArray responseData = reply->readAll();
			QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

			if (!responseDoc.isNull())
			{
				qDebug() << "receiptId: " << responseDoc["receiptId"].toInt();

				QJsonObject objBody = responseDoc["body"].toObject();
				QJsonObject objMessage = objBody["messageData"].toObject();
				QJsonObject objText = objMessage["textMessageData"].toObject();

				qDebug() << "text: " << objText["textMessage"].toString();

				QJsonObject objId = objBody["senderData"].toObject();

				qDebug() << "chatId: " << objId["chatId"].toString();

				emit sendIdNotificationForDelete(QString::number(responseDoc["receiptId"].toInt()));
				emit lastMessageReceived(qMakePair<QString, QString>(objId["chatId"].toString(), objText["textMessage"].toString()));
			}
			else
				qDebug() << "Not array or null";
		}
		else
			qDebug() << "Error:" << reply->errorString();

		reply->deleteLater();
		isBusy = false;

		// Самовызов через 5 секунд
		QTimer::singleShot(5000, this, &WhatsAppJacket::getLastMessageAsync);
		});
}



void WhatsAppJacket::deleteNotification(QString idNotification)
{
	QString urlStringTemp = QString("https://3100.api.green-api.com/waInstance3100514553/deleteNotification/134edc19c6c64e4f971e4578b787f54725492643c588466095/");

	urlStringTemp += idNotification;

	QUrl url(urlStringTemp);

	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	QNetworkReply* reply = manager->deleteResource(request);

	QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {

		if (reply->error() == QNetworkReply::NoError)
		{
			QByteArray responseData = reply->readAll();
			QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);

			if (!responseDoc.isNull())
			{
				if (!responseDoc["result"].toBool())
					qDebug() << responseDoc["reason"].toString();
				else
					qDebug() << "Notification was delete";
			}
			else
				qDebug() << "Not array or null";
		}
		else
			qDebug() << "Error:" << reply->errorString();

		reply->deleteLater();
		}
	);
}