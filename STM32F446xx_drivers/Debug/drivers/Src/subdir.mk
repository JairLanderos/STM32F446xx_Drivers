################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/STM32F446xx_GPIO_drivers.c \
../drivers/Src/STM32F446xx_I2C_drivers.c \
../drivers/Src/STM32F446xx_RCC_drivers.c \
../drivers/Src/STM32F446xx_SPI_drivers.c \
../drivers/Src/STM32F446xx_USART_drivers.c 

OBJS += \
./drivers/Src/STM32F446xx_GPIO_drivers.o \
./drivers/Src/STM32F446xx_I2C_drivers.o \
./drivers/Src/STM32F446xx_RCC_drivers.o \
./drivers/Src/STM32F446xx_SPI_drivers.o \
./drivers/Src/STM32F446xx_USART_drivers.o 

C_DEPS += \
./drivers/Src/STM32F446xx_GPIO_drivers.d \
./drivers/Src/STM32F446xx_I2C_drivers.d \
./drivers/Src/STM32F446xx_RCC_drivers.d \
./drivers/Src/STM32F446xx_SPI_drivers.d \
./drivers/Src/STM32F446xx_USART_drivers.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I"C:/Users/Fam. Landeros Franco/Desktop/Embedded Systems/MCU1-Course/MCU1/STM32F446xx_drivers/drivers/Inc" -I"C:/Users/Fam. Landeros Franco/Desktop/Embedded Systems/MCU1-Course/MCU1/STM32F446xx_drivers/bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/STM32F446xx_GPIO_drivers.d ./drivers/Src/STM32F446xx_GPIO_drivers.o ./drivers/Src/STM32F446xx_I2C_drivers.d ./drivers/Src/STM32F446xx_I2C_drivers.o ./drivers/Src/STM32F446xx_RCC_drivers.d ./drivers/Src/STM32F446xx_RCC_drivers.o ./drivers/Src/STM32F446xx_SPI_drivers.d ./drivers/Src/STM32F446xx_SPI_drivers.o ./drivers/Src/STM32F446xx_USART_drivers.d ./drivers/Src/STM32F446xx_USART_drivers.o

.PHONY: clean-drivers-2f-Src

