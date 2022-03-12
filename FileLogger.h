#pragma once

#include <fstream>
#include <string>

using namespace std;

enum log_level {debug, info, error, critical};
class FileLogger
{
protected:
	log_level m_level;
	ofstream m_of;

	void output(string s, log_level level);

public:
	FileLogger(string filePath, log_level level = error);
	~FileLogger();

	void Debug(string s);
	void Info(string s);
	void Error(string s);
	void Critical(string s);

	void SetLogLevel(log_level ll);
};

