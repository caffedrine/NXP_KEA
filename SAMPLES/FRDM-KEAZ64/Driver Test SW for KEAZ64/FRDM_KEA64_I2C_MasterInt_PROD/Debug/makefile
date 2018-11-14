################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 8
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
FRDM_KEA64_I2C_MasterInt_PROD.elf \

EXECUTABLES_QUOTED += \
"FRDM_KEA64_I2C_MasterInt_PROD.elf" \

EXECUTABLES_OS_FORMAT += \
FRDM_KEA64_I2C_MasterInt_PROD.elf \

SECONDARY_FLASH += \
FRDM_KEA64_I2C_MasterInt_PROD.bin \

SECONDARY_FLASH_QUOTED += \
"FRDM_KEA64_I2C_MasterInt_PROD.bin" \

SECONDARY_FLASH_OS_FORMAT += \
FRDM_KEA64_I2C_MasterInt_PROD.bin \


# All Target
all: warning-messages explicit-dependencies FRDM_KEA64_I2C_MasterInt_PROD.elf secondary-outputs

# Tool invocations
FRDM_KEA64_I2C_MasterInt_PROD.elf: $(OBJS) C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Driver\ Test\ SW\ for\ KEAZ64/FRDM_KEA64_I2C_MasterInt_PROD/Project_Settings/Linker_Files/SKEAZ_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #9 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "FRDM_KEA64_I2C_MasterInt_PROD.elf" "@FRDM_KEA64_I2C_MasterInt_PROD.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

FRDM_KEA64_I2C_MasterInt_PROD.bin: $(EXECUTABLES)
	@echo 'Executing target #10 $@'
	@echo 'Invoking: Standard S32DS Create Flash Image'
	arm-none-eabi-objcopy -O binary $(EXECUTABLES)  "FRDM_KEA64_I2C_MasterInt_PROD.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*/*.o ./*/*.o  $(SECONDARY_FLASH_QUOTED) $(EXECUTABLES_QUOTED) ./*/*/*.d ./*/*.d  C:\Users\B48683\Documents\Auto Apps\FRDM+ Firmware\Quick Start Packages\QSP Raw Material\SDK Drivers and Code Example Projects\FRDM-KEAZ64\Driver Test SW for KEAZ64\FRDM_KEA64_I2C_MasterInt_PROD\Debug\FRDM_KEA64_I2C_MasterInt_PROD.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

