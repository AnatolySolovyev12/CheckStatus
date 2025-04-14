#include "ProcessObject.h"

ProcessObject::ProcessObject(QObject* parent)
	: QObject(parent), classTimer(new QTimer())
{
	AttachConsole(ATTACH_PARENT_PROCESS);

	connect(classTimer, &QTimer::timeout, this, &ProcessObject::classTimerIsDone);
	classTimer->stop();
}


void ProcessObject::setParam(QString name, QString URL, QString updateSecond, bool checkParse)
{
	m_name = name;
	m_URL = URL;
	m_updateSecond = updateSecond;
	m_checkParse = checkParse;

	if (m_checkParse)
		classTimer->start(m_updateSecond.toInt()); // Каждые три секунды
	else
		classTimer->stop();
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
	else
	{
		qDebug() << "Correct: " << hSnapshot;
	}

	PROCESSENTRY32 pe; // Структура PROCESSENTRY32 (tlhelp32.h) описывает запись из списка процессов, находящихся в системном адресном пространстве при snapshot.

	pe.dwSize = sizeof(PROCESSENTRY32); // Размер структуры в байтах. Перед вызовом функции Process32First задайте для этого элемента значение sizeof(PROCESSENTRY32). Если не инициализировать dwSize, process32First завершается сбоем.

	Process32First(hSnapshot, &pe); // Извлекает сведения о первом процессе, обнаруженном в системном snapshot. Обычно это дежурный процесс системы, не нужный в анализе.

	bool OK = false;

	while (Process32Next(hSnapshot, &pe)) // false в случае отсутствия отсутствия модулей процесса т.е. дошли до конца или вообще snapshot пустой
	{
		//qDebug() << QString::fromWCharArray(pe.szExeFile) << "   " << pe.th32ProcessID;

		if (QString::fromWCharArray(pe.szExeFile) == m_URL)
		{
			OK = true;
		}
	}

	if (!OK)
		emit messageReceived(m_URL);
	else
		qDebug() << m_URL << " OK";
	//cmake-gui.exe
}
