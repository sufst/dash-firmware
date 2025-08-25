################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/drivers/wayland/lv_wayland.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_window.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.c \
../Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.c 

OBJS += \
./Core/Src/lvgl/src/drivers/wayland/lv_wayland.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_window.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.o \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.o 

C_DEPS += \
./Core/Src/lvgl/src/drivers/wayland/lv_wayland.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_window.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.d \
./Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/drivers/wayland/%.o Core/Src/lvgl/src/drivers/wayland/%.su Core/Src/lvgl/src/drivers/wayland/%.cyclo: ../Core/Src/lvgl/src/drivers/wayland/%.c Core/Src/lvgl/src/drivers/wayland/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DLV_LVGL_H_INCLUDE_SIMPLE -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Core/Inc/Display -I../Core/Src/lvgl -I../Core/Src/ui -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-drivers-2f-wayland

clean-Core-2f-Src-2f-lvgl-2f-src-2f-drivers-2f-wayland:
	-$(RM) ./Core/Src/lvgl/src/drivers/wayland/lv_wayland.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wayland.d ./Core/Src/lvgl/src/drivers/wayland/lv_wayland.o ./Core/Src/lvgl/src/drivers/wayland/lv_wayland.su ./Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.d ./Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.o ./Core/Src/lvgl/src/drivers/wayland/lv_wayland_smm.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_cache.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_dmabuf.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_keyboard.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_pointer_axis.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_seat.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shell.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_shm.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_touch.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_window_decorations.su ./Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.cyclo ./Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.d ./Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.o ./Core/Src/lvgl/src/drivers/wayland/lv_wl_xdg_shell.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-drivers-2f-wayland

