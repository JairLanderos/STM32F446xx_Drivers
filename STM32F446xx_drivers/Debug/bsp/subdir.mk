################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/DS1307.c \
../bsp/LCD.c 

OBJS += \
./bsp/DS1307.o \
./bsp/LCD.o 

C_DEPS += \
./bsp/DS1307.d \
./bsp/LCD.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/%.o: ../bsp/%.c bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I"C:/Users/Fam. Landeros Franco/Desktop/Embedded Systems/MCU1-Course/MCU1/STM32F446xx_drivers/drivers/Inc" -I"C:/Users/Fam. Landeros Franco/Desktop/Embedded Systems/MCU1-Course/MCU1/STM32F446xx_drivers/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bsp

clean-bsp:
	-$(RM) ./bsp/DS1307.d ./bsp/DS1307.o ./bsp/LCD.d ./bsp/LCD.o

.PHONY: clean-bsp

