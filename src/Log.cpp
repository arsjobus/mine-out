#include "log.h"

std::string Log::filename = "log.txt";

Log::Log(void) {
	infoLabel = "[INFO] ";
	warningLabel = "[WARN] ";
	errorLabel = "[ERRO] ";
	successLabel = "[SUCC] ";
	setSeparator(": ");
	setFilename("log.txt");
	// Clear log
	clear();
}

Log::~Log(void) { logfileWriteable.close(); }

void Log::clear() {
	// Clear the contents of the log file
	logfileWriteable.open(filename, std::ofstream::out | std::ofstream::trunc);
	logfileWriteable.close();
}

bool Log::eventIsAllowedToConsoleOutput(int logtype) {
	if (DEBUG_OUTPUT_ENABLED &&
		logtype == logtype::LOG_INFO && DEBUG_OUTPUT_INFO ||
		logtype == logtype::LOG_SUCCESS && DEBUG_OUTPUT_SUCCESS ||
		logtype == logtype::LOG_WARNING && DEBUG_OUTPUT_WARNING ||
		logtype == logtype::LOG_ERROR && DEBUG_OUTPUT_ERROR)
			return true;
	else
		return false;
}

bool Log::eventIsAllowedToLog(int logtype) {
	if (DEBUG_LOG_ENABLED &&
		logtype == logtype::LOG_INFO && DEBUG_LOG_INFO ||
		logtype == logtype::LOG_SUCCESS && DEBUG_LOG_SUCCESS ||
		logtype == logtype::LOG_WARNING && DEBUG_LOG_WARNING ||
		logtype == logtype::LOG_ERROR && DEBUG_LOG_ERROR)
			return true;
	else
		return false;
}

bool Log::exists() {
	std::string error_message = filename;
	error_message.append(" could not be found!");
	logfileReadable.open(filename.c_str());
	if (logfileReadable.good()) {
        logfileReadable.close();
        return true;
    } else {
        logfileReadable.close();

		throw error_message.c_str();
        return false;
    }   
}

bool Log::exists(const char* customFilename) {
	std::string error_message = customFilename;
	error_message.append(" could not be found!");
	logfileReadable.open(customFilename);
	if (logfileReadable.good()) {
        logfileReadable.close();
        return true;
    } else {
        logfileReadable.close();

		throw error_message.c_str();
        return false;
    }   
}

std::string Log::floatToString(float fltNumber) {
	std::stringstream ss;
	ss << fltNumber;
	std::string strNumber = ss.str();
	return strNumber;
}

std::string Log::intToString(int intNumber) {
	std::stringstream ss;
	ss << intNumber;
	std::string strNumber = ss.str();
	return strNumber;
}

void Log::open() {
	if (exists())
		logfileWriteable.open(filename.c_str(), std::fstream::out | std::fstream::app);
}

void Log::open(const char* customFilename) {
	if (exists(customFilename))
		logfileWriteable.open(customFilename, std::fstream::out | std::fstream::app);
}

const char *Log::outputLogType(int logtype) {
	switch (logtype) {
		case LOG_INFO:
			return infoLabel.c_str();
			break;
		case LOG_WARNING:
			return warningLabel.c_str();
			break;
		case LOG_ERROR:
			return errorLabel.c_str();
			break;
		case LOG_SUCCESS:
			return successLabel.c_str();
			break;
	}
}

void Log::quickWrite(const char* textToWrite) {
	if (eventIsAllowedToLog(logtype::LOG_INFO) || eventIsAllowedToConsoleOutput(logtype::LOG_INFO)) {
		open();
		if (exists()) {
			write(logtype::LOG_INFO, textToWrite);
			logfileWriteable.close();
		}
		else throw "failed to write to log file.";
	}
}

void Log::quickWrite(std::string textToWrite) {
	if (eventIsAllowedToLog(logtype::LOG_INFO) || eventIsAllowedToConsoleOutput(logtype::LOG_INFO)) {
		open();
		if (exists()) {
			write(logtype::LOG_INFO, textToWrite);
			logfileWriteable.close();
		}
		else throw "failed to write to log file.";
	}
}

void Log::quickWrite(int logtype, const char* textToWrite) {
	if (eventIsAllowedToLog(logtype) || eventIsAllowedToConsoleOutput(logtype)) {
		open();
		if (exists()) {
			write(logtype, textToWrite);
			logfileWriteable.close();
		}
		else throw "failed to write to log file.";
	}
}

void Log::quickWrite(int logtype, const std::string &textToWrite) {
	if (eventIsAllowedToLog(logtype) || eventIsAllowedToConsoleOutput(logtype)) {
		open();
		if (exists()) {
			write(logtype, textToWrite);
			logfileWriteable.close();
		}
		else throw "failed to write to log file.";
	}
}

void Log::setFilename(const char* newFilename) {
	if (logfileWriteable.is_open())
		logfileWriteable.close();
	filename = newFilename;
}

void Log::setSeparator(const char *newSeparator) { separator = newSeparator; }

std::string &Log::getSeparator() { return separator; }

void Log::write(int logtype, const char* textToWrite) {
	if (logfileWriteable.is_open()) {
		// Output to console
		if (eventIsAllowedToConsoleOutput(logtype))
			std::cout << outputLogType(logtype) << textToWrite << std::endl;
		// Output to logfile
		if (eventIsAllowedToLog(logtype))
			logfileWriteable << outputLogType(logtype) << textToWrite << std::endl;
	} else throw "failed to write to log file.";
}

void Log::write(int logtype, const std::string &textToWrite) {
	if (logfileWriteable.is_open()) {
		// Output to console
		if (eventIsAllowedToConsoleOutput(logtype))
			std::cout << outputLogType(logtype) << textToWrite << std::endl;
		// Output to logfile
		if (eventIsAllowedToLog(logtype))
			logfileWriteable << outputLogType(logtype) << textToWrite << std::endl;
	} else throw "failed to write to log file.";
}