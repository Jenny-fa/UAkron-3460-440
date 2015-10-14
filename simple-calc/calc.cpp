#include "config.hpp"

#include <iostream>
#include <sstream>

#include "cli.hpp"
#include "parser.hpp"

#define LOG_EXPR(x) std::cout << #x << " = " << (x) << std::endl

int main(int argc, char* argv[]) {
	calc::init(argc, argv);

	if (argc > 1) {
		calc::report_error("Too many arguments.");
		return 2;
	}

	std::stringbuf buffer;
	std::cin.get(buffer);

	try {
		calc::parser parser(&buffer);

		while (true) {
			const calc::expr* expr = parser.next_expr();
			if (!expr)
				break;
			try {
				std::cout << expr->eval() << std::endl;
			}
			catch (const std::domain_error& exception) {
				calc::report_error("Division by zero.");
			}
			delete expr;
		}
	}
	catch (const std::ios_base::failure& exception) {
		calc::report_error("An unexpected I/O error occurred.\n\twhat: %s",
			exception.what());
		return 1;
	}
	catch (const calc::parse_error& exception) {
		calc::report_error(exception);
		return 1;
	}

	return 0;
}
