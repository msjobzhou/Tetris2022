#include "FileLogger.h"



FileLogger::FileLogger(string filePath, log_level level)
{
	m_of.open(filePath.c_str(), ios::out | ios::app);
	m_level = level;
	if (!m_of.is_open())
	{

	}
}


FileLogger::~FileLogger()
{
	m_of.close();
}


void FileLogger::output(string s, log_level level)
{
	string prefix;
	switch (level)
	{
	case debug:
		prefix = "[debug]: ";
		break;
		
	case info:
		prefix = "[info]: ";
		break;
	case error:
		prefix = "[error]: ";
		break;
	case critical:
		prefix = "[critical]: ";
		break;
	default:
		break;
	}

	if (m_level <= level)
	{
		m_of << s << endl;
	}
}


void FileLogger::Debug(string s)
{
	output(s, debug);
}
void FileLogger::Info(string s)
{
	output(s, info);
}
void FileLogger::Error(string s)
{
	output(s, error);
}
void FileLogger::Critical(string s)
{
	output(s, critical);
}
void FileLogger::SetLogLevel(log_level ll)
{
	m_level = ll;
}