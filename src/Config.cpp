#include "Config.h"

Config::Config(void)
{
}

Config::~Config(void)
{
}

void Config::openFileInReadOnlyMode(const char *filename)
{
	fileSettings.open(filename, std::ios::in);
}

void Config::openFileInWriteMode(const char *filename)
{
	fileSettings.open(filename, std::ios::out);
}

std::string Config::findValue(const char *variableName, const char *filename)
{
	openFileInReadOnlyMode(filename);

	if (fileSettings.is_open())
	{
		bool foundVariableName = false;
		std::string strDataSetting;
		std::string strVariableName = variableName;
		std::string strVariableValue = "";

		// Search the open file for the string that is specified by variable name.
		while(!fileSettings.eof() && !foundVariableName)
		{
			std::getline(fileSettings, strDataSetting);

			if (strVariableName.length() <= strDataSetting.length())
			{
				for (int i = 0; i < strVariableName.length(); ++i)
				{
					// If a letter in the requested variable name mismatches with the string in strDataString
					if (strDataSetting.at(i) != strVariableName.at(i))
						break;

					// Else if the last letter of the specified variable name has been checked.
					else if (i + 1 == strVariableName.length()) {
						// Iterate past spaces and equal signs
						for (int j = ++i; j < strDataSetting.length(); ++j)
						{
							if ((strDataSetting.at(j) != (const char&)" ") && (strDataSetting.at(j) != (const char&)"="))
							{
								strVariableValue = strDataSetting.substr(j, strDataSetting.length());

								foundVariableName = true;

								break;
							}
						}

						break;
					}
				}
			}
		}

		fileSettings.close();

		return strVariableValue;
	}
}

bool Config::getSettingAsBool(const char*variableName, const char *filename)
{
	bool bVariableValue;

	std::stringstream ss(findValue(variableName, filename));
	
	if (ss.str() != "")
		ss >> bVariableValue;
	else
		bVariableValue = false;

	return bVariableValue;
}

/**
* Retrieves the specified user setting from the file and returns its value as an unsigned int.
*/
size_t &Config::getSettingAsUInt(const char *variableName, const char *filename)
{
	unsigned int uVariableValue;

	std::stringstream ss(findValue(variableName, filename));
	
	if (ss.str() != "")
		ss >> uVariableValue;
	else
		uVariableValue = 0;

	return uVariableValue;
}
