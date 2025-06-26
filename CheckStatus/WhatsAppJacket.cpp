#include "WhatsAppJacket.h"

WhatsAppJacket::WhatsAppJacket(QObject* parent)
	: QObject(parent), manager(new QNetworkAccessManager)
{
	AttachConsole(ATTACH_PARENT_PROCESS);
	urlString = QString(getTokenFromFile());
	chatId = getChatIdFromFile();
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
	QFile file("token.txt");

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
	QFile file("chatId.txt");

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