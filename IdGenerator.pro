TEMPLATE = app
CONFIG += c++14
CONFIG -= qt
TARGET = IdGenerator
HEADERS = include/IdGenerator.h
SOURCES = \
        tests/IdGeneratorTest.cpp
        
DESTDIR = build
OBJECTS_DIR =   ./obj
INCLUDEPATH = 
LIBS += -lpthread -lboost_test_exec_monitor
