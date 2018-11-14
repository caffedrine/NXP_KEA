################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/LIN_Driver/diagnostic/lin_diagnostic_service.c" \

C_SRCS += \
../include/LIN_Driver/diagnostic/lin_diagnostic_service.c \

OBJS += \
./include/LIN_Driver/diagnostic/lin_diagnostic_service.o \

C_DEPS += \
./include/LIN_Driver/diagnostic/lin_diagnostic_service.d \

OBJS_QUOTED += \
"./include/LIN_Driver/diagnostic/lin_diagnostic_service.o" \

OBJS_OS_FORMAT += \
./include/LIN_Driver/diagnostic/lin_diagnostic_service.o \

C_DEPS_QUOTED += \
"./include/LIN_Driver/diagnostic/lin_diagnostic_service.d" \


# Each subdirectory must supply rules for building sources it contributes
include/LIN_Driver/diagnostic/lin_diagnostic_service.o: ../include/LIN_Driver/diagnostic/lin_diagnostic_service.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/diagnostic/lin_diagnostic_service.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/diagnostic/lin_diagnostic_service.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


