#include "config.hpp"

#include <iostream>
#include <fstream>

#include "cli.hpp"
#include "lexer.hpp"

#define LOG_EXPR(x) std::cout << #x << " = " << (x) << std::endl

int main(int argc, char* argv[]) {
	calc::init(argv[0]);

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

	calc::lexer lexer(buffer);
	std::size_t token_count = 0;

	try {
		while (true) {
			calc::lexer::token_type token = lexer.next_token();

			std::cout << "Token " << ++token_count << ":\n";
			std::cout << "\textent: " << token.extent() << '\n';
			std::cout << "\tkind: ";
			switch (token.kind()) {
				case calc::token_kind::unknown:
					std::cout << "unknown";
					break;
				case calc::token_kind::eof:
					std::cout << "eof";
					break;
				case calc::token_kind::newline:
					std::cout << "newline";
					break;
#if !MOAR_DIGITS
				case calc::token_kind::digit:
					std::cout << "digit";
					break;
#else
				case calc::token_kind::integer:
					std::cout << "integer";
					break;
#endif
				case calc::token_kind::addition_operator:
					std::cout << "addition_operator";
					break;
				case calc::token_kind::subtraction_operator:
					std::cout << "subtraction_operator";
					break;
				case calc::token_kind::multiplication_operator:
					std::cout << "multiplication_operator";
					break;
				case calc::token_kind::division_operator:
					std::cout << "division_operator";
					break;
				case calc::token_kind::modulus_operator:
					std::cout << "modulus_operator";
					break;
				case calc::token_kind::left_parenthesis:
					std::cout << "left_parenthesis";
					break;
				case calc::token_kind::right_parenthesis:
					std::cout << "right_parenthesis";
					break;
				default:
					std::cout << "<invalid token kind>";
					break;
			}
			std::cout << '\n';
			std::cout << "\tflags: " << static_cast<unsigned int>(token.flags());;
			if (!token)
				std::cout << " (ERROR)";
			std::cout << '\n';
			std::cout << "\ttext: " << token.text();
			std::cout << std::endl;

			if (token.kind() == calc::token_kind::eof)
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
