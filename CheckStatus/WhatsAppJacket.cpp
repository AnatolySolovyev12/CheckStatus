#include "WhatsAppJacket.h"

WhatsAppJacket::WhatsAppJacket(QObject* parent)
	: QObject(parent), manager(new QNetworkAccessManager)
{
	AttachConsole(ATTACH_PARENT_PROCESS);
}

void WhatsAppJacket::sendMessage(const QString message)
{
	if (message.isEmpty()) {
		qWarning() << "Attempt to send empty message";
		return;
	}

	// ������������ URL �������
	QString urlString = QString("https://1103.api.green-api.com/waInstance1103223344/sendMessage/8f294624f5da41fabeb8a41950bde3b041fe06f7b7034567a9"); // Watcher

	QUrl url(urlString);

	QJsonObject json;
	json["chatId"] = "79825313114@c.us";
	//json["chatId"] = "120363044487282624@g.us"; // ����

	json["message"] = message; // ���������� ���������� ���������

	// �������������� JSON-������� � ������
	QJsonDocument jsonDoc(json);
	QByteArray jsonData = jsonDoc.toJson();

	// �������� �������
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	// �������� �������
	manager->post(request, jsonData);

	// ���������� ������ (���� ����������). ����������.
	QObject::connect(manager, &QNetworkAccessManager::finished, [](QNetworkReply* reply) {

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
