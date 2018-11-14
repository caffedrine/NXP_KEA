################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 25
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include include/lin_cfg/subdir.mk
-include include/LIN_Driver/transport/subdir.mk
-include include/LIN_Driver/lowlevel/subdir.mk
-include include/LIN_Driver/diagnostic/subdir.mk
-include include/LIN_Driver/coreapi/subdir.mk
-include include/LIN_Driver/bsp/UART/subdir.mk
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
FRDM_KEA64_LIN_Master.elf \

EXECUTABLES_QUOTED += \
"FRDM_KEA64_LIN_Master.elf" \

EXECUTABLES_OS_FORMAT += \
FRDM_KEA64_LIN_Master.elf \

SECONDARY_FLASH += \
FRDM_KEA64_LIN_Master.srec \

SECONDARY_FLASH_QUOTED += \
"FRDM_KEA64_LIN_Master.srec" \

SECONDARY_FLASH_OS_FORMAT += \
FRDM_KEA64_LIN_Master.srec \


# All Target
all: warning-messages explicit-dependencies FRDM_KEA64_LIN_Master.elf secondary-outputs

# Tool invocations
FRDM_KEA64_LIN_Master.elf: $(OBJS) C:/Users/B48683/Desktop/FRDM_KEA64_LIN_Master/Project_Settings/Linker_Files/SKEAZ_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #26 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "FRDM_KEA64_LIN_Master.elf" "@FRDM_KEA64_LIN_Master.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

FRDM_KEA64_LIN_Master.srec: $(EXECUTABLES)
	@echo 'Executing target #27 $@'
	@echo 'Invoking: Standard S32DS Create Flash Image'
	arm-none-eabi-objcopy -O srec $(EXECUTABLES)  "FRDM_KEA64_LIN_Master.srec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*/*.o ./*/*/*/*/*.o ./*/*/*/*.o ./*/*.o  $(SECONDARY_FLASH_QUOTED) $(EXECUTABLES_QUOTED) ./*/*/*.d ./*/*/*/*/*.d ./*/*/*/*.d ./*/*.d  C:\Users\B48683\Desktop\FRDM_KEA64_LIN_Master\Debug\FRDM_KEA64_LIN_Master.elf
	-@echo ' '

post-build:
	-arm-none-eabi-size "FRDM_KEA64_LIN_Master.elf"; arm-none-eabi-objcopy -O binary "FRDM_KEA64_LIN_Master.elf" "FRDM_KEA64_LIN_Master.bin" ;  arm-none-eabi-objcopy -O srec "FRDM_KEA64_LIN_Master.elf" "FRDM_KEA64_LIN_Master.hex"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY: post-build

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

