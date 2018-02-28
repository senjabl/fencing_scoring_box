################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/senja/Arduino/libraries/TimedAction/TimedAction.cpp 

LINK_OBJ += \
./libraries/TimedAction/TimedAction.cpp.o 

CPP_DEPS += \
./libraries/TimedAction/TimedAction.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/TimedAction/TimedAction.cpp.o: /home/senja/Arduino/libraries/TimedAction/TimedAction.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/senja/sloeber//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu= -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR   -I"/home/senja/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/cores/arduino" -I"/home/senja/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/variants/mega" -I"/home/senja/Arduino/libraries/IRremote" -I"/home/senja/Arduino/libraries/MD_MAX72xx" -I"/home/senja/Arduino/libraries/MD_Parola" -I"/home/senja/Arduino/libraries/MD_Parola/src" -I"/home/senja/Arduino/libraries/TimedAction" -I"/home/senja/Arduino/libraries/Timer1" -I"/home/senja/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/SPI" -I"/home/senja/sloeber/arduinoPlugin/packages/arduino/hardware/avr/1.6.19/libraries/SPI/src" -I"/home/senja/Arduino/libraries/MD_MAX72XX" -I"/home/senja/Arduino/libraries/MD_MAX72XX/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


