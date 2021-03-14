#ifndef DEBUG_H
#define DEBUG_H

#include <string>
using namespace std;

enum class LOG : unsigned short
{
	NONE = 0,
	FATAL = 1,
	ERRORLOG = 2,
	WARNING = 3,
	TRACE = 4,
	INFO = 5
};

constexpr static const char* ToString( LOG logType )
{
	switch ( logType )
	{
	case LOG::NONE:			return "NONE";
	case LOG::FATAL:		return "FATAL ERROR";
	case LOG::ERRORLOG:		return "ERROR";
	case LOG::WARNING:		return "WARNING";
	case LOG::TRACE:		return "TRACE";
	case LOG::INFO:			return "INFO";
	}
}

#ifndef DEBUG_MODE 
#define DEBUG_MODE 1
#endif

#if DEBUG_MODE == 1



/* Creates brand new output file for logging */
#define DEBUG_INIT() ( Debug::DebugInit() )

/* Logs error to engine output log */
#define DEBUG_LOG( LogType, Message ) ( Debug::Log( LogType, Message, __FILE__, __FUNCTION__, __LINE__ ) )

/* Will print to console log */
#define CONSOLE_LOG( LogType, Message ) ( Debug::Console_Log( LogType, Message, __FILE__, __FUNCTION__, __LINE__ ) )

#else

/* Logs error to engine output log */
#define DEBUG_LOG( LogType, Message )

/* Will print to console log */
#define CONSOLE_LOG( LogType, Message ) 

/* Creates brand new output file for logging */
#define DEBUG_INIT()

#endif


// Static Debug Log Class
class Debug
{

public:

	Debug() = delete;	// Static class, no constructor needed
	Debug( const Debug& ) = delete;
	Debug& operator=( const Debug& ) = delete;
	Debug( Debug&& ) = delete;
	Debug& operator=( Debug&& ) = delete;

	static void DebugInit();
	static void Log( const LOG logType, const string& message, const string& fileName, const string& function, const int line );
	static void Console_Log( const LOG logType, const string& message, const string& fileName, const string& function, const int line );
	
private:

	static string outputLogFileName;


};



#endif
