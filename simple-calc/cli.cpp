/**
 * @file		cli.cpp
 * Contains helper function definitions for command-line interface programs.
 *
 * @author		Jennifer Yao
 * @date		10/11/2015
 * @copyright	All rights reserved.
 */

#include "cli.hpp"

#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <cstdarg>
#include <cstdio>
#include <iostream>

#ifdef _WIN32
#  define kDirectorySeparator '\\'
#else
#  define kDirectorySeparator '/'
#endif

namespace calc {
	static std::string program_name;

	void init(int argc, char* argv[]) {
		// process command-line arguments
		program_name = argv[0];

		if (!program_name.empty()) {
			std::string::const_iterator begin, end;
			for (begin = --(program_name.cend()); begin != program_name.cbegin(); --begin)
				if (*begin == kDirectorySeparator)
					break;
			++begin;
			for (end = begin; end != program_name.cend(); ++end)
				if (*end == '.')
					break;
			program_name.assign(begin, end);
		}
	}

	bool is_interactive() {
#if HAVE_UNISTD_H
		return isatty(STDIN_FILENO) && isatty(STDERR_FILENO);
#else
		return false;
#endif
	}

	void show_prompt() {
		std::cerr << "> ";
	}

	void report_error(const char* format, ...) {
		va_list args;
		va_start(args, format);
		std::cerr << program_name << ": ";
		std::vfprintf(stderr, format, args);
		std::cerr << std::endl;
		va_end(args);
	}

	// Explicit instantiations for the report_error function template.
	template void report_error(const parse_error&);
	template void report_error(const wparse_error&);
} // namespace calc
