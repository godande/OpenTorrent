#pragma once

#include <fstream>
#include <string>

#define LOG_FILE_PATH "log.txt"

class Logger {
private:
	static Logger *logger_instance;
	static std::ofstream file;

public:
	static Logger *GetInstance();
	void Debug(const std::string &);
	void Info(const std::string &);
	void Warning(const std::string &);
	void Error(const std::string &);
private:
	Logger() = default;
	Logger(const Logger &);
	void Write(const std::string &, const std::string &);
	Logger &operator=(const Logger &) { return *this; };
};