#include "ProcessObject.h"

ProcessObject::ProcessObject(QObject* parent)
	: QObject(parent), classTimer(new QTimer())
{
	AttachConsole(ATTACH_PARENT_PROCESS);

	connect(classTimer, &QTimer::timeout, this, &ProcessObject::classTimerIsDone);
	classTimer->stop();
}


void ProcessObject::setParam(QString name, QString URL, QString updateSecond, bool checkParse, bool checkSend, QString checkAppName)
{
	m_name = name;
	m_URL = URL;
	m_updateSecond = updateSecond;
	m_checkParse = checkParse;
	m_checkSend = checkSend;

	if (m_checkParse || m_checkSend)
		classTimer->start(m_updateSecond.toInt()); // Каждые три секунды
	else
		classTimer->stop();

	m_checkApp = checkAppName;
}


void ProcessObject::classTimerIsDone()
{
	check();
}


void ProcessObject::check()
{
	// Принимает snapshot указанных процессов, а также кучи, модули и потоки, используемые этими процессами. 
	// TH32CS_SNAPPROCESS - Включает все процессы в системе в snapshot.
	// Если функция завершается успешно, она возвращает открытый дескриптор указанной snapshot.

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// Если функция завершается сбоем, она возвращает INVALID_HANDLE_VALUE.Дополнительные сведения об ошибке можно получить, вызвав GetLastError.Возможные коды ошибок включают ERROR_BAD_LENGTH.

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		qDebug() << GetLastError();
	}

	PROCESSENTRY32 pe; // Структура PROCESSENTRY32 (tlhelp32.h) описывает запись из списка процессов, находящихся в системном адресном пространстве при snapshot.

	pe.dwSize = sizeof(PROCESSENTRY32); // Размер структуры в байтах. Перед вызовом функции Process32First задайте для этого элемента значение sizeof(PROCESSENTRY32). Если не инициализировать dwSize, process32First завершается сбоем.

	Process32First(hSnapshot, &pe); // Извлекает сведения о первом процессе, обнаруженном в системном snapshot. Обычно это дежурный процесс системы, не нужный в анализе.

	while (Process32Next(hSnapshot, &pe)) // false в случае отсутствия отсутствия модулей процесса т.е. дошли до конца или вообще snapshot пустой
	{
		if (QString::fromWCharArray(pe.szExeFile) == m_checkApp)
		{
			qDebug() << QDateTime::currentDateTime() << ": " << m_checkApp << "OK";
			return;
		}
	}

	qDebug() << QDateTime::currentDateTime() << ": " << m_checkApp << " NOT WORK";

	if (m_checkSend)
		emit messageReceived("Не работает " + m_checkApp);

	historyOfObject += QDateTime::currentDateTime().toString() + '\n';

	QString temporary = getStartString(m_URL);
	qDebug() << temporary.toUtf8().constData();
	if (m_checkParse)
		QTimer::singleShot(5000, [temporary]() {

		system(temporary.toUtf8().constData());

			});
}



QString ProcessObject::getStartString(QString any)
{
	QString temporary = "cd /d \"" + any + "\" && start \"\" \"";
	// start - для того чтобы system() не блокировала выполнение запускающей программы.
	//Параметр "" используется для указания заголовка окна (можно оставить пустым).
	// /d - позволяет не только сменить директорию н ои диск.

	// Если не сделать предварительный переход то приложение при запуске будет искать свои конфигурации по пути этого приложения а не своего.

	if (!any.endsWith("\\"))
		temporary += any + "\\";
	else
		temporary += any;

	temporary += m_name + "\"";

	return temporary;
}



QString ProcessObject::getHistoryObject()
{
	return historyOfObject;
}



QString ProcessObject::getNameObject()
{
	return m_checkApp;
}
