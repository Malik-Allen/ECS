#ifndef DEBUG_H
#define DEBUG_H

#include <string>

enum class LOG : unsigned short {
	NONE = 0,
	FATAL,
	ERROR,
	WARNING,
	TRACE,
	INFO
};

constexpr static const char* ToString( LOG logType )
{
	switch ( logType )
	{
	case LOG::NONE:			return "NONE";
	case LOG::FATAL:		return "FATAL ERROR";
	case LOG::ERROR:		return "ERROR";
	case LOG::WARNING:		return "WARNING";
	case LOG::TRACE:		return "TRACE";
	case LOG::INFO:			return "INFO";
	}
}

/* Logs error to engine output log */
#define DEBUG_LOG( LogType, Message ) ( Debug::Log( LogType, Message, __FILE__, __FUNCTION__, __LINE__ ) )

/* Will print to console log */
#define CONSOLE_LOG( LogType, Message ) ( Debug::Console_Log( LogType, Message, __FILE__, __FUNCTION__, __LINE__ ) )

// Static Debug Log Class
class Debug {

public:
	Debug() = delete;	// Static class, no constructor needed
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(Debug&&) = delete;
	
	static void DebugInit();
	static void Log( const LOG logType, const std::string& message, const std::string& fileName, const std::string& function, const int line );
	static void Console_Log( const LOG logType, const std::string& message, const std::string& fileName, const std::string& function, const int line );

private:

	static std::string outputLogFileName;

};

#endif