################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/LIN_Driver/lowlevel/lin.c" \

C_SRCS += \
../include/LIN_Driver/lowlevel/lin.c \

OBJS += \
./include/LIN_Driver/lowlevel/lin.o \

C_DEPS += \
./include/LIN_Driver/lowlevel/lin.d \

OBJS_QUOTED += \
"./include/LIN_Driver/lowlevel/lin.o" \

OBJS_OS_FORMAT += \
./include/LIN_Driver/lowlevel/lin.o \

C_DEPS_QUOTED += \
"./include/LIN_Driver/lowlevel/lin.d" \


# Each subdirectory must supply rules for building sources it contributes
include/LIN_Driver/lowlevel/lin.o: ../include/LIN_Driver/lowlevel/lin.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/lowlevel/lin.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/lowlevel/lin.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


