#ifndef FRAMEWORK_TERMINAL_HPP__
#define FRAMEWORK_TERMINAL_HPP__

#include <framework/terminal/util.hpp>

#include <algorithm>
#include <iostream>
#include <string>

namespace framework {

struct VirtualTerminal {
	std::string buffer;

	std::string computeTransition(std::string const& next) {
		if (buffer == next) return "";
		unsigned const n = std::count(buffer.begin(), buffer.end(), '\n');
		return clearLines(n) + CSI"0m" + next;
	}

	static std::string hide() { return CSI"0;8m"; }

	void append(std::string s) {
		buffer += s;
	}

	VirtualTerminal flip(std::string const& next) {
		auto const& transition = computeTransition(next);
		if (transition == "") return *this;
		std::cout << transition << hide();
		std::flush(std::cout);
		return {next};
	}
};

}


#endif // FRAMEWORK_TERMINAL_HPP__
