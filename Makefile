
ARDMK_DIR=../Arduino-Makefile
BOARD_TAG     = uno
MONITOR_PORT  = /dev/ttyACM0

#ARDUINO_QUIET=1

include $(ARDMK_DIR)/Arduino.mk



#ARDUINO_QUIET=1
check-syntax:	
	$(CXX_NAME) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)


#check-syntax:
#	avr-g++ -c   -x c++ -include Arduino.h -MMD   -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=105 -I. -I/usr/share/arduino/hardware/arduino/cores/arduino -I/usr/share/arduino/hardware/arduino/variants/standard -I/usr/share/arduino/libraries/LiquidCrystal  -Wall   -fsyntax-only $(CHK_SOURCES)
