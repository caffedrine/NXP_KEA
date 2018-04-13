################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/FRDM_KEA64_I2C_MasterInt.c" \
"../src/i2c.c" \
"../src/ics.c" \
"../src/nvic.c" \
"../src/printf.c" \
"../src/uart.c" \

C_SRCS += \
../src/FRDM_KEA64_I2C_MasterInt.c \
../src/i2c.c \
../src/ics.c \
../src/nvic.c \
../src/printf.c \
../src/uart.c \

OBJS += \
./src/FRDM_KEA64_I2C_MasterInt.o \
./src/i2c.o \
./src/ics.o \
./src/nvic.o \
./src/printf.o \
./src/uart.o \

C_DEPS += \
./src/FRDM_KEA64_I2C_MasterInt.d \
./src/i2c.d \
./src/ics.d \
./src/nvic.d \
./src/printf.d \
./src/uart.d \

OBJS_QUOTED += \
"./src/FRDM_KEA64_I2C_MasterInt.o" \
"./src/i2c.o" \
"./src/ics.o" \
"./src/nvic.o" \
"./src/printf.o" \
"./src/uart.o" \

OBJS_OS_FORMAT += \
./src/FRDM_KEA64_I2C_MasterInt.o \
./src/i2c.o \
./src/ics.o \
./src/nvic.o \
./src/printf.o \
./src/uart.o \

C_DEPS_QUOTED += \
"./src/FRDM_KEA64_I2C_MasterInt.d" \
"./src/i2c.d" \
"./src/ics.d" \
"./src/nvic.d" \
"./src/printf.d" \
"./src/uart.d" \


# Each subdirectory must supply rules for building sources it contributes
src/FRDM_KEA64_I2C_MasterInt.o: ../src/FRDM_KEA64_I2C_MasterInt.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/FRDM_KEA64_I2C_MasterInt.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/FRDM_KEA64_I2C_MasterInt.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/i2c.o: ../src/i2c.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/i2c.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/i2c.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ics.o: ../src/ics.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/ics.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/ics.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/nvic.o: ../src/nvic.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/nvic.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/nvic.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/printf.o: ../src/printf.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/printf.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/printf.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/uart.o: ../src/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/uart.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


