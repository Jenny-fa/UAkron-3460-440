#include "config.hpp"

#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <iostream>

#include "cli.hpp"
#include "parser.hpp"

#define LOG_EXPR(x) std::cout << #x << " = " << (x) << std::endl

int main(int argc, char* argv[]) {
	calc::init(argc, argv);

#if HAVE_UNISTD_H
	if (optind < argc) {
#else
	if (argc > 1) {
#endif
		calc::report_error("Too many arguments.");
		return 2;
	}

	try {
		calc::parser parser(std::cin);

		while (true) {
			if (calc::is_interactive())
				calc::show_prompt();
			try {
				std::unique_ptr<const calc::expr> expr = parser.next_expr();
				if (!expr)
					break;
				std::cout << expr->value() << std::endl;
			}
			catch (const calc::parse_error& exception) {
				calc::report_error(exception);
			}
			catch (const std::domain_error& exception) {
				calc::report_error("Attempt to divide by zero.");
			}
		}
	}
	catch (const std::ios_base::failure& exception) {
		calc::report_error("An unexpected I/O error occurred.");
		return 1;
	}

	return 0;
}
