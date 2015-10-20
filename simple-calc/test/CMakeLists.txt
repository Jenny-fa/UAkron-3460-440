# Link all subsequently added targets against libcalc.
link_libraries(libcalc)

# Add test executables.
add_executable(test_lexer lexer.cpp)
add_executable(test_parser parser.cpp)
add_dependencies(check test_lexer test_parser)

# Add tests.
foreach(i RANGE 1 4)
	add_test(
		NAME lexer_${i}
		COMMAND test_lexer "${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt"
		WORKING_DIRECTORY $<TARGET_LINKER_FILE_DIR:libcalc>
	)
	set_tests_properties(lexer_${i} PROPERTIES
		REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt)
endforeach()

foreach(i RANGE 1 4)
	add_test(
		NAME parser_${i}
		COMMAND test_parser "${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt"
		WORKING_DIRECTORY $<TARGET_LINKER_FILE_DIR:libcalc>
	)
	set_tests_properties(parser_${i} PROPERTIES
		REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt)
endforeach()
# set_tests_properties(parser_2 PROPERTIES WILL_FAIL TRUE)
# set_tests_properties(parser_4 PROPERTIES WILL_FAIL TRUE)
