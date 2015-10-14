#include "config.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "cli.hpp"
#include "lexer.hpp"

#define LOG_EXPR(x) std::cout << #x << " = " << (x) << std::endl

int main(int argc, char* argv[]) {
	calc::init(argc, argv);

	if (argc > 2) {
		calc::report_error("Too many arguments.");
		return 2;
	}

	std::stringbuf buffer;

	if (argc == 2) {
		std::ifstream in_file(argv[1]);
		if (!in_file) {
			calc::report_error("Could not open %s.", argv[1]);
			return 1;
		}
		in_file >> &buffer;
	}
	else {
		std::cin >> &buffer;
	}

	calc::lexer lexer(&buffer);
	std::size_t token_count = 0;

	try {
		while (true) {
			const calc::lexer::token_type token = lexer.next_token();

			std::cout << "Token " << ++token_count << ":\n";
			std::cout << "\textent: " << token.extent() << '\n';
			std::cout << "\tkind: ";
			switch (token.kind()) {
				case calc::token_base::unknown:
					std::cout << "unknown";
					break;
				case calc::token_base::eof:
					std::cout << "eof";
					break;
				case calc::token_base::newline:
					std::cout << "newline";
					break;
#if !MOAR_DIGITS
				case calc::token_base::digit:
					std::cout << "digit";
					break;
#else
				case calc::token_base::integer:
					std::cout << "integer";
					break;
#endif
				case calc::token_base::addition_operator:
					std::cout << "addition_operator";
					break;
				case calc::token_base::subtraction_operator:
					std::cout << "subtraction_operator";
					break;
				case calc::token_base::multiplication_operator:
					std::cout << "multiplication_operator";
					break;
				case calc::token_base::division_operator:
					std::cout << "division_operator";
					break;
				case calc::token_base::modulus_operator:
					std::cout << "modulus_operator";
					break;
				case calc::token_base::left_parenthesis:
					std::cout << "left_parenthesis";
					break;
				case calc::token_base::right_parenthesis:
					std::cout << "right_parenthesis";
					break;
				default:
					std::cout << "<invalid token kind>";
					break;
			}
			std::cout << '\n';
			std::cout << "\tflags: " << token.flags();
			if (!token)
				std::cout << " (ERROR)";
			std::cout << '\n';
			std::cout << "\ttext: " << token.text();
			std::cout << std::endl;

			if (token.kind() == calc::token_base::eof)
				break;
		}
	}
	catch (const std::ios_base::failure& exception) {
		calc::report_error("An unexpected I/O error occurred.\n\twhat: %s",
			exception.what());
		return 1;
	}

	return 0;
}
