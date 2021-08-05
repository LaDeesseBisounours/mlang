# Adapted from : https://github.com/phtrivier/cppunit-money-example-cmake/blob/master/code/cmake_modules/FindCPPUNIT.cmake
#
# Find the CppUnit includes and library
#
# This module defines
# CPPUNIT_INCLUDE_DIR, where to find tiff.h, etc.
# CPPUNIT_LIBRARY, where to find the CppUnit library.
# CPPUNIT_FOUND, If false, do not try to use CppUnit.

# also defined, but not for general use are
# CPPUNIT_DEBUG_LIBRARY, where to find the CppUnit library in debug
# mode.

SET(CPPUNIT_FOUND "NO")
SET(CPPUNIT_FIND_REQUIRED "YES")        
FIND_PATH(CPPUNIT_INCLUDE_DIR
	cppunit/TestCase.h
	PATHS
		cppunit/include
		/usr/local/include
		/usr/include
    REQUIRED
)

FIND_LIBRARY(CPPUNIT_LIBRARY cppunit
    HINTS
           ${CPPUNIT_INCLUDE_DIR}/../lib
           /usr/lib
           /usr/local/lib
    REQUIRED)
       
MESSAGE( STATUS "CPPUNIT include: ${CPPUNIT_INCLUDE_DIR}")
MESSAGE( STATUS "CPPUNIT library: ${CPPUNIT_LIBRARY}")


IF(CPPUNIT_INCLUDE_DIR)
    IF(CPPUNIT_LIBRARY)
        SET(CPPUNIT_FOUND "YES")
    ELSE (CPPUNIT_LIBRARY)
        MESSAGE(WARNING "Could not find library CppUnit.")
    ENDIF(CPPUNIT_LIBRARY)
ELSE(CPPUNIT_INCLUDE_DIR)
    MESSAGE(WARNING "Could not find library CppUnit.")
ENDIF(CPPUNIT_INCLUDE_DIR)

