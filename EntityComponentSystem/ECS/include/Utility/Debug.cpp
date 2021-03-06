#include "Debug.h"
#include <fstream>
#include <iostream>

std::string Debug::outputLogFileName( "ECS-Output-Log.txt" );

void Debug::DebugInit()
{
	std::ofstream outputFile;
	outputFile.open( outputLogFileName, std::ios::out );	// We create the file using out just to make sure it is there and clears it
	outputFile.close();
}

void Debug::Log( const LOG logType, const std::string& message, const std::string& fileName, const std::string& function, const int line )
{
	// Note on @param fileName:
		// FileName is currently not in use, 
		// it ends up causing a lot of clutter in logs, 
		// Once I am able to use a relative rather than a direct path, it will be reconsidered 
	std::ofstream outputFile;
	outputFile.open( outputLogFileName, std::ios::app | std::ios::out );
	outputFile << "[" << ToString( logType ) << "]:\t" << function << "(" << line << "):\t" << message << std::endl;
	outputFile.flush();
	outputFile.close();
}

void Debug::Console_Log( const LOG logType, const std::string & message, const std::string & fileName, const std::string& function, const int line )
{
	std::cout << "[" << ToString( logType ) << "]:\t" << function << "(" << line << "):\t" << message << std::endl;
}
