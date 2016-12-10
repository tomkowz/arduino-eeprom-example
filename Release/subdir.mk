################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../eeprom_demo.cpp 

LINK_OBJ += \
./eeprom_demo.cpp.o 

CPP_DEPS += \
./eeprom_demo.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
eeprom_demo.cpp.o: ../eeprom_demo.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/tomkowz/eclipse/cpp-neon/Eclipse.app/Contents/Eclipse/arduinoPlugin/tools/arduino/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-g++" -c -g -Os -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR   -I"/Users/tomkowz/eclipse/cpp-neon/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.14/cores/arduino" -I"/Users/tomkowz/eclipse/cpp-neon/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.14/variants/standard" -I"/Users/tomkowz/eclipse/cpp-neon/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.14/libraries/EEPROM" -I"/Users/tomkowz/eclipse/cpp-neon/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.14/libraries/EEPROM/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<" -o "$@"  -Wall
	@echo 'Finished building: $<'
	@echo ' '


