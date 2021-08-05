add_executable(frameworkUnitTest test.cpp datatypes/testCounter.cpp ${UNIT_TEST_MAIN})
target_link_libraries(frameworkUnitTest framework ${CPPUNIT_LIBRARY})
