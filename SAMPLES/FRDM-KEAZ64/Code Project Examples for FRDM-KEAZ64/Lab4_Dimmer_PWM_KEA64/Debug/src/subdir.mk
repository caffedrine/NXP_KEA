################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/CLK.c" \
"../src/FTM.c" \
"../src/Lab4_Dimmer_PWM_KEA64.c" \

C_SRCS += \
../src/CLK.c \
../src/FTM.c \
../src/Lab4_Dimmer_PWM_KEA64.c \

OBJS += \
./src/CLK.o \
./src/FTM.o \
./src/Lab4_Dimmer_PWM_KEA64.o \

C_DEPS += \
./src/CLK.d \
./src/FTM.d \
./src/Lab4_Dimmer_PWM_KEA64.d \

OBJS_QUOTED += \
"./src/CLK.o" \
"./src/FTM.o" \
"./src/Lab4_Dimmer_PWM_KEA64.o" \

OBJS_OS_FORMAT += \
./src/CLK.o \
./src/FTM.o \
./src/Lab4_Dimmer_PWM_KEA64.o \

C_DEPS_QUOTED += \
"./src/CLK.d" \
"./src/FTM.d" \
"./src/Lab4_Dimmer_PWM_KEA64.d" \


# Each subdirectory must supply rules for building sources it contributes
src/CLK.o: ../src/CLK.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/CLK.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/CLK.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FTM.o: ../src/FTM.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/FTM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/FTM.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Lab4_Dimmer_PWM_KEA64.o: ../src/Lab4_Dimmer_PWM_KEA64.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Lab4_Dimmer_PWM_KEA64.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "src/Lab4_Dimmer_PWM_KEA64.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


