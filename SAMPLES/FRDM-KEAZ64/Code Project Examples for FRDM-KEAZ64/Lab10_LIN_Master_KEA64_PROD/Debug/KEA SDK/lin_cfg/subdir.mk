################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"C:/Users/B48683/Documents/Auto Apps/FRDM+ Firmware/Quick Start Packages/QSP Raw Material/SDK Drivers and Code Example Projects/FRDM-KEAZ64/SDK Drivers for KEAZ64/lin_cfg/lin_cfg.c" \

C_SRCS += \
C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/SDK\ Drivers\ for\ KEAZ64/lin_cfg/lin_cfg.c \

OBJS += \
./KEA\ SDK/lin_cfg/lin_cfg.o \

C_DEPS += \
./KEA\ SDK/lin_cfg/lin_cfg.d \

OBJS_QUOTED += \
"./KEA SDK/lin_cfg/lin_cfg.o" \

OBJS_OS_FORMAT += \
./KEA\ SDK/lin_cfg/lin_cfg.o \

C_DEPS_QUOTED += \
"./KEA SDK/lin_cfg/lin_cfg.d" \


# Each subdirectory must supply rules for building sources it contributes
KEA\ SDK/lin_cfg/lin_cfg.o: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/SDK\ Drivers\ for\ KEAZ64/lin_cfg/lin_cfg.c
	@echo 'Building file: $<'
	@echo 'Executing target #41 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@KEA SDK/lin_cfg/lin_cfg.args" -MMD -MP -MF"KEA SDK/lin_cfg/lin_cfg.d" -MT"KEA\ SDK/lin_cfg/lin_cfg.d" -c -o "KEA SDK/lin_cfg/lin_cfg.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


