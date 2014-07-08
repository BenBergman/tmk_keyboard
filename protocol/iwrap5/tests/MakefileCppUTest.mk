#--- Outputs ---#
COMPONENT_NAME = iwrap

#--- Inputs ---#
CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_HOME = /usr
#CPP_PLATFORM = Gcc
CXX = clang++
CC = clang

CPPFLAGS += -I(CPPUTEST_HOME)/include
CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
SRC_DIRS = \
		   .. \

TEST_SRC_DIRS = \
				. \

INCLUDE_DIRS = \
			   $(CPPUTEST_HOME)/include \
			   .. \
			   ../.. \
			   ../../../common \

include MakefileWorker.mk
