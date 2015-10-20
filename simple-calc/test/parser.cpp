#include "config.hpp"

#include <iostream>
#include <fstream>

#include "cli.hpp"
#include "parser.hpp"

#define LOG_EXPR(x) std::cout << #x << " = " << (x) << std::endl

int main(int argc, char* argv[]) {
	calc::init(argc, argv);

	if (argc > 2) {
		calc::report_error("Too many arguments.");
		return 2;
	}

	std::ifstream in;
	std::streambuf* buffer;

	if (argc == 2) {
		in.open(argv[1]);
		if (!in) {
			calc::report_error("Could not open %s.", argv[1]);
			return 1;
		}
		buffer = in.rdbuf();
	}
	else {
		buffer = std::cin.rdbuf();
	}

	try {
		calc::parser parser(buffer);

		while (true) {
			if (calc::is_interactive() && buffer == std::cin.rdbuf())
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
				calc::report_error("Division by zero.");
			}
		}
	}
	catch (const std::ios_base::failure& exception) {
		calc::report_error("An unexpected I/O error occurred.\n\twhat: %s",
			exception.what());
		return 1;
	}

	return 0;
}
