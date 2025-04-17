################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/command.c \
../Src/dio.c \
../Src/main.c \
../Src/serial.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer.c 

OBJS += \
./Src/command.o \
./Src/dio.o \
./Src/main.o \
./Src/serial.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer.o 

C_DEPS += \
./Src/command.d \
./Src/dio.d \
./Src/main.d \
./Src/serial.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/command.cyclo ./Src/command.d ./Src/command.o ./Src/command.su ./Src/dio.cyclo ./Src/dio.d ./Src/dio.o ./Src/dio.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/serial.cyclo ./Src/serial.d ./Src/serial.o ./Src/serial.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su

.PHONY: clean-Src

