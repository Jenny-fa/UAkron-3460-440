/**
 * @file		cli.cpp
 * Contains helper function definitions for command-line interface programs.
 *
 * @author		Jennifer Yao
 * @date		10/11/2015
 * @copyright	All rights reserved.
 */

#include "cli.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <iostream>

#ifdef _WIN32
#  define kDirectorySeparator '\\'
#else
#  define kDirectorySeparator '/'
#endif

namespace calc {
	static std::string program_name;
	static bool interactive;

	static std::string filename(const std::string& path) {
		if (path.empty())
			return std::string();
		std::size_t start_index = path.rfind(kDirectorySeparator) + 1;
		if (start_index == std::string::npos)
			start_index = 0;
		return path.substr(start_index);
	}

	static std::string stem(const std::string& path) {
		std::string result = filename(path);
		if (result == "." || result == "..")
			return result;
		std::size_t pos = result.rfind('.');
		return result.substr(0, pos);
	}

	void init(const char* progname) {
		program_name = stem(progname);
	}

	void init(int argc, char* argv[]) {
		program_name = stem(argv[0]);
#if HAVE_UNISTD_H
		interactive = isatty(STDIN_FILENO) && isatty(STDERR_FILENO);
#else
		interactive = false;
#endif

#if HAVE_UNISTD_H
		// process command-line arguments
		int c;

		while ((c = getopt(argc, argv, "i")) != -1) {
			switch (c) {
				case 'i':
					interactive = true;
					break;
				default:
					std::abort();
			}
		}
#endif
	}

	bool is_interactive() {
		return interactive;
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
