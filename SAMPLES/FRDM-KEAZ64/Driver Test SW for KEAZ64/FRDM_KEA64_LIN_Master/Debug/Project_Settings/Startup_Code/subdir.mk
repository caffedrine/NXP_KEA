################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/startup_SKEAZ1284.S" \

S_UPPER_SRCS += \
../Project_Settings/Startup_Code/startup_SKEAZ1284.S \

C_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/system_SKEAZ1284.c" \

C_SRCS += \
../Project_Settings/Startup_Code/system_SKEAZ1284.c \

OBJS += \
./Project_Settings/Startup_Code/startup_SKEAZ1284.o \
./Project_Settings/Startup_Code/system_SKEAZ1284.o \

C_DEPS += \
./Project_Settings/Startup_Code/system_SKEAZ1284.d \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/startup_SKEAZ1284.o" \
"./Project_Settings/Startup_Code/system_SKEAZ1284.o" \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/startup_SKEAZ1284.o \
./Project_Settings/Startup_Code/system_SKEAZ1284.o \

C_DEPS_QUOTED += \
"./Project_Settings/Startup_Code/system_SKEAZ1284.d" \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/startup_SKEAZ1284.o: ../Project_Settings/Startup_Code/startup_SKEAZ1284.S
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: Standard S32DS Assembler'
	arm-none-eabi-gcc "@Project_Settings/Startup_Code/startup_SKEAZ1284.args" -c -o "Project_Settings/Startup_Code/startup_SKEAZ1284.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Project_Settings/Startup_Code/system_SKEAZ1284.o: ../Project_Settings/Startup_Code/system_SKEAZ1284.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Project_Settings/Startup_Code/system_SKEAZ1284.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "Project_Settings/Startup_Code/system_SKEAZ1284.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


