################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/lin_cfg/lin_cfg.c" \

C_SRCS += \
../include/lin_cfg/lin_cfg.c \

OBJS += \
./include/lin_cfg/lin_cfg.o \

C_DEPS += \
./include/lin_cfg/lin_cfg.d \

OBJS_QUOTED += \
"./include/lin_cfg/lin_cfg.o" \

OBJS_OS_FORMAT += \
./include/lin_cfg/lin_cfg.o \

C_DEPS_QUOTED += \
"./include/lin_cfg/lin_cfg.d" \


# Each subdirectory must supply rules for building sources it contributes
include/lin_cfg/lin_cfg.o: ../include/lin_cfg/lin_cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/lin_cfg/lin_cfg.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/lin_cfg/lin_cfg.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


