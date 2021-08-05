add_executable(frameworkUnitTest test.cpp mainTest.cpp)
target_link_libraries(frameworkUnitTest framework ${CPPUNIT_LIBRARY})
