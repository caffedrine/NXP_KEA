################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/FRDM_KEA64_LIN_Slave.c" \
"../src/ftm.c" \
"../src/ics.c" \
"../src/nvic.c" \
"../src/spi.c" \
"../src/uart.c" \

C_SRCS += \
../src/FRDM_KEA64_LIN_Slave.c \
../src/ftm.c \
../src/ics.c \
../src/nvic.c \
../src/spi.c \
../src/uart.c \

OBJS += \
./src/FRDM_KEA64_LIN_Slave.o \
./src/ftm.o \
./src/ics.o \
./src/nvic.o \
./src/spi.o \
./src/uart.o \

C_DEPS += \
./src/FRDM_KEA64_LIN_Slave.d \
./src/ftm.d \
./src/ics.d \
./src/nvic.d \
./src/spi.d \
./src/uart.d \

OBJS_QUOTED += \
"./src/FRDM_KEA64_LIN_Slave.o" \
"./src/ftm.o" \
"./src/ics.o" \
"./src/nvic.o" \
"./src/spi.o" \
"./src/uart.o" \

OBJS_OS_FORMAT += \
./src/FRDM_KEA64_LIN_Slave.o \
./src/ftm.o \
./src/ics.o \
./src/nvic.o \
./src/spi.o \
./src/uart.o \

C_DEPS_QUOTED += \
"./src/FRDM_KEA64_LIN_Slave.d" \
"./src/ftm.d" \
"./src/ics.d" \
"./src/nvic.d" \
"./src/spi.d" \
"./src/uart.d" \


# Each subdirectory must supply rules for building sources it contributes
src/FRDM_KEA64_LIN_Slave.o: ../src/FRDM_KEA64_LIN_Slave.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/FRDM_KEA64_LIN_Slave.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/FRDM_KEA64_LIN_Slave.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ftm.o: ../src/ftm.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/ftm.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/ftm.o" "$<"
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

src/spi.o: ../src/spi.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/spi.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/spi.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/uart.o: ../src/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/uart.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


