# Link all subsequently added targets against libcalc.
link_libraries(libcalc)

# Add tests.
add_test_executable(lexer lexer.cpp)
foreach(i RANGE 1 4)
	add_test(
		NAME ${TEST_SUITE}_lexer_${i}
		COMMAND test_${TEST_SUITE}_lexer "${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt"
		WORKING_DIRECTORY $<TARGET_LINKER_FILE_DIR:libcalc>
	)
	set_tests_properties(${TEST_SUITE}_lexer_${i} PROPERTIES
		REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt)
endforeach()

add_test_executable(parser parser.cpp)
foreach(i RANGE 1 4)
	add_test(
		NAME ${TEST_SUITE}_parser_${i}
		COMMAND test_${TEST_SUITE}_parser "${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt"
		WORKING_DIRECTORY $<TARGET_LINKER_FILE_DIR:libcalc>
	)
	set_tests_properties(${TEST_SUITE}_parser_${i} PROPERTIES
		REQUIRED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/input_${i}.txt)
endforeach()
set_tests_properties(${TEST_SUITE}_parser_2 PROPERTIES WILL_FAIL TRUE)
set_tests_properties(${TEST_SUITE}_parser_4 PROPERTIES WILL_FAIL TRUE)
