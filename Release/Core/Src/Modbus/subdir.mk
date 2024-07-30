################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Modbus/RTU.c 

OBJS += \
./Core/Src/Modbus/RTU.o 

C_DEPS += \
./Core/Src/Modbus/RTU.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Modbus/%.o Core/Src/Modbus/%.su Core/Src/Modbus/%.cyclo: ../Core/Src/Modbus/%.c Core/Src/Modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F030x6 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Modbus

clean-Core-2f-Src-2f-Modbus:
	-$(RM) ./Core/Src/Modbus/RTU.cyclo ./Core/Src/Modbus/RTU.d ./Core/Src/Modbus/RTU.o ./Core/Src/Modbus/RTU.su

.PHONY: clean-Core-2f-Src-2f-Modbus

