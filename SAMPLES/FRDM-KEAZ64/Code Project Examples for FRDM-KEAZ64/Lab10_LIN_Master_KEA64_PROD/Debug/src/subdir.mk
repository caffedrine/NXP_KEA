################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/ADC.c" \
"../src/Lab10_LIN_Master_KEA64_PROD.c" \
"../src/ics.c" \
"../src/nvic.c" \
"../src/spi.c" \

C_SRCS += \
../src/ADC.c \
../src/Lab10_LIN_Master_KEA64_PROD.c \
../src/ics.c \
../src/nvic.c \
../src/spi.c \

OBJS += \
./src/ADC.o \
./src/Lab10_LIN_Master_KEA64_PROD.o \
./src/ics.o \
./src/nvic.o \
./src/spi.o \

C_DEPS += \
./src/ADC.d \
./src/Lab10_LIN_Master_KEA64_PROD.d \
./src/ics.d \
./src/nvic.d \
./src/spi.d \

OBJS_QUOTED += \
"./src/ADC.o" \
"./src/Lab10_LIN_Master_KEA64_PROD.o" \
"./src/ics.o" \
"./src/nvic.o" \
"./src/spi.o" \

OBJS_OS_FORMAT += \
./src/ADC.o \
./src/Lab10_LIN_Master_KEA64_PROD.o \
./src/ics.o \
./src/nvic.o \
./src/spi.o \

C_DEPS_QUOTED += \
"./src/ADC.d" \
"./src/Lab10_LIN_Master_KEA64_PROD.d" \
"./src/ics.d" \
"./src/nvic.d" \
"./src/spi.d" \


# Each subdirectory must supply rules for building sources it contributes
src/ADC.o: ../src/ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/ADC.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Lab10_LIN_Master_KEA64_PROD.o: ../src/Lab10_LIN_Master_KEA64_PROD.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Lab10_LIN_Master_KEA64_PROD.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/Lab10_LIN_Master_KEA64_PROD.o" "$<"
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


