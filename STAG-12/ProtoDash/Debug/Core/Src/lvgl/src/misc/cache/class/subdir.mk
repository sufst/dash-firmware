################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.c \
../Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.c 

OBJS += \
./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.o \
./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.o 

C_DEPS += \
./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.d \
./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/misc/cache/class/%.o Core/Src/lvgl/src/misc/cache/class/%.su Core/Src/lvgl/src/misc/cache/class/%.cyclo: ../Core/Src/lvgl/src/misc/cache/class/%.c Core/Src/lvgl/src/misc/cache/class/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DLV_LVGL_H_INCLUDE_SIMPLE -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Src/lvgl -I../Core/Src/ui -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc-2f-cache-2f-class

clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc-2f-cache-2f-class:
	-$(RM) ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.cyclo ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.d ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.o ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_ll.su ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.cyclo ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.d ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.o ./Core/Src/lvgl/src/misc/cache/class/lv_cache_lru_rb.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc-2f-cache-2f-class

