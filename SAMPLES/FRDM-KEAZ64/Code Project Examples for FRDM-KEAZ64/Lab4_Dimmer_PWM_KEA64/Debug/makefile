################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 5
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
Lab4_Dimmer_PWM_KEA64.elf \

EXECUTABLES_QUOTED += \
"Lab4_Dimmer_PWM_KEA64.elf" \

EXECUTABLES_OS_FORMAT += \
Lab4_Dimmer_PWM_KEA64.elf \

SECONDARY_FLASH += \
Lab4_Dimmer_PWM_KEA64.srec \

SECONDARY_FLASH_QUOTED += \
"Lab4_Dimmer_PWM_KEA64.srec" \

SECONDARY_FLASH_OS_FORMAT += \
Lab4_Dimmer_PWM_KEA64.srec \


# All Target
all: warning-messages explicit-dependencies Lab4_Dimmer_PWM_KEA64.elf secondary-outputs

# Tool invocations
Lab4_Dimmer_PWM_KEA64.elf: $(OBJS) C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab4_Dimmer_PWM_KEA64/Project_Settings/Linker_Files/SKEAZ_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #6 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "Lab4_Dimmer_PWM_KEA64.elf" "@Lab4_Dimmer_PWM_KEA64.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

Lab4_Dimmer_PWM_KEA64.srec: $(EXECUTABLES)
	@echo 'Executing target #7 $@'
	@echo 'Invoking: Standard S32DS Create Flash Image'
	arm-none-eabi-objcopy -O srec $(EXECUTABLES)  "Lab4_Dimmer_PWM_KEA64.srec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*/*.o ./*/*.o  $(SECONDARY_FLASH_QUOTED) $(EXECUTABLES_QUOTED) ./*/*/*.d ./*/*.d  C:\Users\B48683\Documents\Auto Apps\FRDM+ Firmware\Quick Start Packages\QSP Raw Material\SDK Drivers and Code Example Projects\FRDM-KEAZ64\Code Project Examples for FRDM-KEAZ64\Lab4_Dimmer_PWM_KEA64\Debug\Lab4_Dimmer_PWM_KEA64.elf
	-@echo ' '

post-build:
	-arm-none-eabi-size "Lab4_Dimmer_PWM_KEA64.elf"; arm-none-eabi-objcopy -O binary "Lab4_Dimmer_PWM_KEA64.elf" "Lab4_Dimmer_PWM_KEA64.bin" ;  arm-none-eabi-objcopy -O srec "Lab4_Dimmer_PWM_KEA64.elf" "Lab4_Dimmer_PWM_KEA64.hex"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY: post-build

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

