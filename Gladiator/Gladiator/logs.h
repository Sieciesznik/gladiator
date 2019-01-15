#pragma once
#define DEBUG
#define LEVEL 3

#ifdef DEBUG
inline void logThisInt(const char* logType, int level, const char* message, int value, const char* file, int line) {

	if (LEVEL >= level) {
		printf("%s%d:%s:%d:", logType, level, file, line);
		printf(message, value);
		printf("\n");
	}
}

inline void logThis(const char* logType, int level, const char* message, const char* file, int line) {

	if (LEVEL >= level)		printf("%s%d:%s:%d:%s\n", logType, level, file, line, message);
}

inline void logError(const char* message, int value, const char* file, int line) {
	printf("ERROR:%s:%d:", file, line);
	printf(message, value);
	printf("\n");
}
#endif

#ifdef DEBUG
//macros with value to print
#define LOG_INFO_I(level, message, value) \
logThisInt("INFO", level, message, value, __FILE__, __LINE__)

#define LOG_DECISION_I(level, message, value) \
logThisInt("DECISION", level, message, value, __FILE__, __LINE__)

#define LOG_ERROR_I(message, value) \
logError(message, value, __FILE__, __LINE__)

//macros without value to print
#define LOG_INFO(level, message) \
logThis("INFO", level, message, __FILE__, __LINE__)

#define LOG_DECISION(level, message) \
logThis("DECISION", level, message, __FILE__, __LINE__)

#define LOG_ERROR(message) \
printf("ERROR:%s:%d:%s\n", __FILE__, __LINE__, message);

#else

#define LOG_INFO_I(level, message, value)
#define LOG_DECISION_I(level, message, value)
#define LOG_ERROR_I(message, value)

#define LOG_INFO(level, message)
#define LOG_DECISION(level, message)
#define LOG_ERROR(message)
#endif

