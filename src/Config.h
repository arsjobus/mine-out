#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <sstream>
#include "Log.h"

class Config
{
public:
	Config(void);
	~Config(void);

	Log log;

	bool getSettingAsBool(const char*variableName, const char *filename);
	size_t getSettingAsUInt(const char *variableName, const char *filename);

private:
	std::string findValue(const char *variableName, const char *filename);

	void openFileInReadOnlyMode(const char *filename);
	void openFileInWriteMode(const char *filename);

private:
	std::fstream fileSettings;
};

#endif
