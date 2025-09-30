#pragma once

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define DEBUG_LOG_ENABLED true

#define DEBUG_LOG_ERROR true
#define DEBUG_LOG_INFO true
#define DEBUG_LOG_SUCCESS true
#define DEBUG_LOG_WARNING true

#define DEBUG_OUTPUT_ENABLED true

#define DEBUG_OUTPUT_ERROR true
#define DEBUG_OUTPUT_INFO true
#define DEBUG_OUTPUT_WARNING true
#define DEBUG_OUTPUT_SUCCESS false

enum logtype {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_SUCCESS,
};

class Log
{
public:
	Log(void);
	~Log(void);

	void clear();
	std::string floatToString(float fltNumber);
	std::string intToString(int intNumber);
	void open();
	void open(const char* customFilename);
	const char* outputLogType(int logtype);
	void quickWrite(const char* textToWrite);
	void quickWrite(std::string textToWrite);
	void quickWrite(int logtype, const char* textToWrite);
	void quickWrite(int logtype, std::string &textToWrite);
	void write(int logtype, const char* textToWrite);
	void write(int logtype, std::string &textToWrite);
	void setFilename(const char* newFilename);

	void setSeparator(const char *newSeparator);
	std::string &getSeparator();

private:
	bool exists();
	bool exists(const char* customFilename);
	bool eventIsAllowedToLog(int logtype);
	bool eventIsAllowedToConsoleOutput(int logtype);

	static std::string filename;
	std::string infoLabel;
	std::string warningLabel;
	std::string errorLabel;
	std::string separator;
	std::string successLabel;
	std::ofstream logfileWriteable;
	std::ifstream logfileReadable;
};

#endif
