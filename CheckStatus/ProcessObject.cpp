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
		classTimer->start(m_updateSecond.toInt()); // ������ ��� �������
	else
		classTimer->stop();
}


void ProcessObject::classTimerIsDone()
{
	check();
}


void ProcessObject::check()
{
	// ��������� snapshot ��������� ���������, � ����� ����, ������ � ������, ������������ ����� ����������. 
	// TH32CS_SNAPPROCESS - �������� ��� �������� � ������� � snapshot.
	// ���� ������� ����������� �������, ��� ���������� �������� ���������� ��������� snapshot.

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// ���� ������� ����������� �����, ��� ���������� INVALID_HANDLE_VALUE.�������������� �������� �� ������ ����� ��������, ������ GetLastError.��������� ���� ������ �������� ERROR_BAD_LENGTH.

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		qDebug() << GetLastError();
	}

	PROCESSENTRY32 pe; // ��������� PROCESSENTRY32 (tlhelp32.h) ��������� ������ �� ������ ���������, ����������� � ��������� �������� ������������ ��� snapshot.

	pe.dwSize = sizeof(PROCESSENTRY32); // ������ ��������� � ������. ����� ������� ������� Process32First ������� ��� ����� �������� �������� sizeof(PROCESSENTRY32). ���� �� ���������������� dwSize, process32First ����������� �����.

	Process32First(hSnapshot, &pe); // ��������� �������� � ������ ��������, ������������ � ��������� snapshot. ������ ��� �������� ������� �������, �� ������ � �������.

	bool OK = false;

	while (Process32Next(hSnapshot, &pe)) // false � ������ ���������� ���������� ������� �������� �.�. ����� �� ����� ��� ������ snapshot ������
	{
		if (QString::fromWCharArray(pe.szExeFile) == m_URL)
		{
			OK = true;
		}
	}

	if (!OK)
	{
		qDebug() << QDateTime::currentDateTime() << ": " << m_URL << " NOT WORK";
		emit messageReceived(m_URL);
	}
}
