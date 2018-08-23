#ifndef FRAMEWORK_TERMINAL_HPP__
#define FRAMEWORK_TERMINAL_HPP__

#include <terminal/util.hpp>

#include <algorithm>
#include <iostream>
#include <string>

#ifdef _WIN32
	#include <stdio.h>
	#include <wchar.h>
	#include <windows.h>
#endif

namespace framework {

static constexpr const char* HIDE = CSI"0;8m";

enum TerminalMode {
	None = 0,
	In = 1 << 1,
	Out = 1 << 2,
	IN_OUT = In | Out
};

class terminal
{
private:
	int mode;
	std::string buffer;

#ifdef _WIN32
	HANDLE hIn, hOut;
	DWORD dwOriginalOutMode = 0;
	DWORD dwOriginalInMode = 0;
#endif

public:
	terminal( int terminalMode = TerminalMode::IN_OUT ) {
		mode = terminalMode;
#ifdef _WIN32
		if ( terminalMode & TerminalMode::In ) {
			hIn = GetStdHandle( STD_INPUT_HANDLE );
			GetConsoleMode( hIn, &dwOriginalInMode );
			DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;
	    DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
	    SetConsoleMode( hIn, dwInMode );
		}

		if ( terminalMode & TerminalMode::Out ) {
			hOut = GetStdHandle( STD_OUTPUT_HANDLE );
			GetConsoleMode( hOut, &dwOriginalOutMode );
	    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	    DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
	    SetConsoleMode( hOut, dwOutMode );
		}
#endif
	}

	~terminal()	{
#ifdef _WIN32
		if ( mode & TerminalMode::In ) {
			SetConsoleMode( hIn, dwOriginalInMode );
		}
		if ( mode & TerminalMode::Out ) {
			SetConsoleMode( hOut, dwOriginalOutMode );
		}
#endif
	}

	terminal append( std::string const& s ) {
		return { buffer + s };
	}

	terminal flip( std::string const& next ) {
		auto const& transition = computeTransition( next );
		if ( transition == "" ) {
			return *this;
		}
		std::cout << transition;// << HIDE;
		std::flush( std::cout );
		return { next };
	}

private:
	terminal( std::string str ) :
		buffer( str )
	{}

	std::string computeTransition( std::string const& next ) {
		if (buffer == next) {
			return "";
		}
		unsigned const n = std::count( buffer.begin(), buffer.end(), '\n' );
		return clearLines(n) + CSI"0m" + next;
	}
};

}


#endif // FRAMEWORK_TERMINAL_HPP__
