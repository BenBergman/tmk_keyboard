CPPUTEST_MAKEFILE="MakefileCppUTest.mk"
CPPUTEST_HOME = /usr
# HARDWARE_MAKEFILE="XXX.mk"

# all: codeCppUTest codeXXX
all: codeCppUTest

# clean: cleanCodeCppUTest cleanCodeXXX
clean: cleanCodeCppUTest

prog: upload

upload: codeXXX
	make -i -f $(HARDWARE_MAKEFILE) upload

#codeCppUTest: CppUTest
codeCppUTest: CppUTest CppUTestExt
	make -i -f $(CPPUTEST_MAKEFILE)

codeXXX:
	echo "Target hardware compilation procedure not specified"

cleanCodeCppUTest:
	make -i -f $(CPPUTEST_MAKEFILE) clean

cleanCodeXXX:
	echo "Target hardware compilation procedure not specified"

CppUTest: $(CPPUTEST_HOME)/lib/libCppUTest.a

CppUTestExt: $(CPPUTEST_HOME)/lib/libCppUTestExt.a

#
#
#
#
#
#
#CPPUTEST_HOME = ../../../cpputest
#CPPFLAGS += -I$(CPPUTEST_HOME)/include
#
## Needs to be in production code too
#CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
#CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
#
#LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
#
#cpputest/lib/libCppUTest:
#	cd $(CPPUTEST_HOME); cmake CMakeLists.txt && make
#
#all: iwrap_tests.cpp cpputest/lib/libCppUTest
#	clang++ tests.cpp
