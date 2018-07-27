#ifndef FRAMEWORK_TERMINAL_UTIL_HPP__
#define FRAMEWORK_TERMINAL_UTIL_HPP__

#include <foundation/string_helper.hpp>

#include <algorithm>
#include <string>
#include <vector>

// #define ESC "\x1b"
// #define CSI "\x1b["
#define ESC "\e"
#define CSI "\e["

namespace framework {

// I suspect that the clearBeforeCursor and ClearAfterCursor functions are backwards?

inline std::string clearBeforeCursor() {
	return CSI"0K";
}

inline std::string clearAfterCursor() {
	return CSI"1K";
}

inline std::string clearLine() {
	return CSI"2K\r";
}

inline std::string moveUp(unsigned n = 1 ) {
	return CSI + std::to_string(n) + "A\r";
}

inline std::string clearLines(unsigned n = 1) {
	return CSI"0m" + clearBeforeCursor() + ((n) ? foundation::repeat(n, clearLine() + moveUp()) /*+ clearLine()*/ : std::string(""));
}

}

#endif // FRAMEWORK_TERMINAL_UTIL_HPP__
