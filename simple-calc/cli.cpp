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

#if defined(_WIN32) || defined(__CYGWIN__)
	#define IS_SLASH(C) ((C) == '\\' || (C) == '/')
#else
	#define IS_SLASH(C) ((C) == '/')
#endif

namespace calc {
	static std::string program_name;
	static bool program_interactive;

	static bool path_has_drive(const std::string& path) {
		if (path.size() >= 2) {
			if (path[1] == ':' &&
				(('A' <= path[0] && path[0] <= 'Z') ||
				 ('a' <= path[0] && path[0] <= 'z')))
				return true;
		}
		return false;
	}

	static std::size_t path_length_without_trailing_slashes(const std::string& path) {
		const bool dest_has_drive = path_has_drive(path);
		std::size_t len = path.size();
		while (((dest_has_drive && len > 3) || (!dest_has_drive && len > 1))
		       && IS_SLASH(path[len - 1]))
			len--;
		return len;
	}

	static std::size_t path_filename_component_start(const std::string& path) {
		const std::size_t len = path.size();
		if (len < 2)
			return 0;
		for (std::size_t i = len - 1; i; i--)
			if (IS_SLASH(path[i - 1]))
				return i;
		// if (len > 2 && path_has_drive(path))
		// 	return 2;
		return 0;
	}

	static std::string path_filename(const std::string& path) {
		const std::size_t len = path_length_without_trailing_slashes(path);
		const std::size_t start_index = path_filename_component_start(path);
		return path.substr(start_index, len - start_index);
	}

	static std::string path_stem(const std::string& path) {
		const std::string filename = path_filename(path);
		if (filename == "." || filename == "..")
			return filename;
		const std::size_t end_index = filename.rfind('.');
		return filename.substr(0, end_index);
	}

	void init(const char* name) {
		program_name = path_stem(name);
	}

	void init(int argc, char* argv[]) {
		program_name = path_stem(argv[0]);
#if HAVE_UNISTD_H
		program_interactive = isatty(STDIN_FILENO) && isatty(STDERR_FILENO);
#else
		program_interactive = false;
#endif

#if HAVE_UNISTD_H
		// process command-line arguments
		int c;

		while ((c = getopt(argc, argv, "i")) != -1) {
			switch (c) {
				case 'i':
					program_interactive = true;
					break;
				case '?':
					std::exit(2);
				default:
					std::abort();
			}
		}
#endif
	}

	bool is_interactive() {
		return program_interactive;
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
