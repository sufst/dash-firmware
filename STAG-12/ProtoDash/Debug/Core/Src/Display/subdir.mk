################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Display/ili9341.c 

OBJS += \
./Core/Src/Display/ili9341.o 

C_DEPS += \
./Core/Src/Display/ili9341.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Display/%.o Core/Src/Display/%.su Core/Src/Display/%.cyclo: ../Core/Src/Display/%.c Core/Src/Display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DLV_LVGL_H_INCLUDE_SIMPLE -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Src/lvgl -I../Core/Src/ui -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Display

clean-Core-2f-Src-2f-Display:
	-$(RM) ./Core/Src/Display/ili9341.cyclo ./Core/Src/Display/ili9341.d ./Core/Src/Display/ili9341.o ./Core/Src/Display/ili9341.su

.PHONY: clean-Core-2f-Src-2f-Display

