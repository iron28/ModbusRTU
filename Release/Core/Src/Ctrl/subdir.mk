################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Ctrl/ADC.c \
../Core/Src/Ctrl/FLASH.c \
../Core/Src/Ctrl/GPIO.c \
../Core/Src/Ctrl/Timer.c 

OBJS += \
./Core/Src/Ctrl/ADC.o \
./Core/Src/Ctrl/FLASH.o \
./Core/Src/Ctrl/GPIO.o \
./Core/Src/Ctrl/Timer.o 

C_DEPS += \
./Core/Src/Ctrl/ADC.d \
./Core/Src/Ctrl/FLASH.d \
./Core/Src/Ctrl/GPIO.d \
./Core/Src/Ctrl/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Ctrl/%.o Core/Src/Ctrl/%.su Core/Src/Ctrl/%.cyclo: ../Core/Src/Ctrl/%.c Core/Src/Ctrl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Ctrl

clean-Core-2f-Src-2f-Ctrl:
	-$(RM) ./Core/Src/Ctrl/ADC.cyclo ./Core/Src/Ctrl/ADC.d ./Core/Src/Ctrl/ADC.o ./Core/Src/Ctrl/ADC.su ./Core/Src/Ctrl/FLASH.cyclo ./Core/Src/Ctrl/FLASH.d ./Core/Src/Ctrl/FLASH.o ./Core/Src/Ctrl/FLASH.su ./Core/Src/Ctrl/GPIO.cyclo ./Core/Src/Ctrl/GPIO.d ./Core/Src/Ctrl/GPIO.o ./Core/Src/Ctrl/GPIO.su ./Core/Src/Ctrl/Timer.cyclo ./Core/Src/Ctrl/Timer.d ./Core/Src/Ctrl/Timer.o ./Core/Src/Ctrl/Timer.su

.PHONY: clean-Core-2f-Src-2f-Ctrl

