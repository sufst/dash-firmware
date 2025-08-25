################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.c \
../Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.c 

OBJS += \
./Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.o \
./Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.o 

C_DEPS += \
./Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.d \
./Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/misc/cache/instance/%.o Core/Src/lvgl/src/misc/cache/instance/%.su Core/Src/lvgl/src/misc/cache/instance/%.cyclo: ../Core/Src/lvgl/src/misc/cache/instance/%.c Core/Src/lvgl/src/misc/cache/instance/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DLV_LVGL_H_INCLUDE_SIMPLE -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Src/lvgl -I../Core/Src/ui -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc-2f-cache-2f-instance

clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc-2f-cache-2f-instance:
	-$(RM) ./Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.cyclo ./Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.d ./Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.o ./Core/Src/lvgl/src/misc/cache/instance/lv_image_cache.su ./Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.cyclo ./Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.d ./Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.o ./Core/Src/lvgl/src/misc/cache/instance/lv_image_header_cache.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc-2f-cache-2f-instance

