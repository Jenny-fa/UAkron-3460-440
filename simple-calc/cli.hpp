/**
 * @file		cli.hpp
 * Contains helper function declarations for command-line interface programs.
 *
 * @author		Jennifer Yao
 * @date		10/11/2015
 * @copyright	All rights reserved.
 */

#ifndef CALC_CLI_HPP
#define CALC_CLI_HPP

#include "config.hpp"

#include "parse_error.hpp"

namespace calc {
	void init(const char* name);
	void init(int argc, char* argv[]);
	bool is_interactive();
	void show_prompt();
	void report_error(const char* format, ...);

	template <typename CharT, class Traits>
	void report_error(const basic_parse_error<CharT, Traits>& error);
} // namespace calc

#include "cli.ipp"

#endif // CALC_CLI_HPP
