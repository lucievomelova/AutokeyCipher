CXX = g++

HEADERS = Autokey.h CLI.h RandomStringGenerator.h BlockCipherModes/*.h StreamCipherModes/*.h
SOURCES = main.cpp Autokey.cpp CLI.cpp RandomStringGenerator.cpp BlockCipherModes/*.cpp StreamCipherModes/*.cpp


.PHONY: all clean

.EXPORT_ALL_VARIABLES:

all: autokey

autokey: $(HEADERS) $(SOURCES)
	$(CXX) -o $@ $^ -Wall

clean:
	rm autokey
