################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/Src/ds1307.c 

OBJS += \
./bsp/Src/ds1307.o 

C_DEPS += \
./bsp/Src/ds1307.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/Src/%.o bsp/Src/%.su bsp/Src/%.cyclo: ../bsp/Src/%.c bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/Users/Joelikane/Desktop/Embedded_C_programming_coursera/Mastering_Microcontroller_Embedded_Driver_Development/MCU1_Course_Workspace/target/RealTimeClock_LCD/bsp/Inc" -I"C:/Users/Joelikane/Desktop/Embedded_C_programming_coursera/Mastering_Microcontroller_Embedded_Driver_Development/MCU1_Course_Workspace/target/RealTimeClock_LCD/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-bsp-2f-Src

clean-bsp-2f-Src:
	-$(RM) ./bsp/Src/ds1307.cyclo ./bsp/Src/ds1307.d ./bsp/Src/ds1307.o ./bsp/Src/ds1307.su

.PHONY: clean-bsp-2f-Src

