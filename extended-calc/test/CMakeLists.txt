# Link all subsequently added targets against libcalc.
link_libraries(libcalc)

# Add test executables.
add_executable(test_lexer lexer.cpp)
add_executable(test_parser parser.cpp)

add_dependencies(check test_lexer test_parser)

# Add tests.
set(INPUT_FILE_COUNT 5)
foreach(i RANGE 1 ${INPUT_FILE_COUNT})
	add_test(
		NAME lexer_${i}
		COMMAND test_lexer ${CMAKE_CURRENT_SOURCE_DIR}/input-${i}.txt
	)
	set_tests_properties(lexer_${i} PROPERTIES
		REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/input-${i}.txt)
endforeach()
foreach(i RANGE 1 ${INPUT_FILE_COUNT})
	add_test(
		NAME parser_${i}
		COMMAND test_parser ${CMAKE_CURRENT_SOURCE_DIR}/input-${i}.txt
	)
	set_tests_properties(parser_${i} PROPERTIES
		REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/input-${i}.txt)
endforeach()
