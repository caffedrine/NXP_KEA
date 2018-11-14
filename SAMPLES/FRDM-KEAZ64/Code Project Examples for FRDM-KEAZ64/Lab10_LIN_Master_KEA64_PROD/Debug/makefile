################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 56
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
-include KEA\ SDK/sources/subdir.mk
-include KEA\ SDK/lin_cfg/subdir.mk
-include KEA\ SDK/LIN_Driver/transport/subdir.mk
-include KEA\ SDK/LIN_Driver/lowlevel/subdir.mk
-include KEA\ SDK/LIN_Driver/diagnostic/subdir.mk
-include KEA\ SDK/LIN_Driver/coreapi/subdir.mk
-include KEA\ SDK/LIN_Driver/bsp/UART/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Lab10_LIN_Master_KEA64_PROD.elf \

EXECUTABLES_QUOTED += \
"Lab10_LIN_Master_KEA64_PROD.elf" \

EXECUTABLES_OS_FORMAT += \
Lab10_LIN_Master_KEA64_PROD.elf \


# All Target
all: warning-messages explicit-dependencies Lab10_LIN_Master_KEA64_PROD.elf secondary-outputs

# Tool invocations
Lab10_LIN_Master_KEA64_PROD.elf: $(OBJS) C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Project_Settings/Linker_Files/SKEAZ_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #57 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "Lab10_LIN_Master_KEA64_PROD.elf" "@Lab10_LIN_Master_KEA64_PROD.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) ./*/*/*/*/*.o ./*/*/*/*.o ./*/*/*.o ./*/*.o  $(EXECUTABLES_QUOTED) ./*/*/*/*/*.d ./*/*/*/*.d ./*/*/*.d ./*/*.d  C:\Users\B48683\Documents\Auto Apps\FRDM+ Firmware\Quick Start Packages\QSP Raw Material\SDK Drivers and Code Example Projects\FRDM-KEAZ64\Code Project Examples for FRDM-KEAZ64\Lab10_LIN_Master_KEA64_PROD\Debug\Lab10_LIN_Master_KEA64_PROD.elf
	-@echo ' '

post-build:
	-arm-none-eabi-size "Lab10_LIN_Master_KEA64_PROD.elf"; arm-none-eabi-objcopy -O binary "Lab10_LIN_Master_KEA64_PROD.elf" "Lab10_LIN_Master_KEA64_PROD.bin" ;  arm-none-eabi-objcopy -O srec "Lab10_LIN_Master_KEA64_PROD.elf" "Lab10_LIN_Master_KEA64_PROD.hex"
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY: post-build

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/transport/lin_commontl_api.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/transport/lin_commontl_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/transport/lin_j2602tl_api.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/transport/lin_lin21tl_api.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/include/LIN_Driver/diagnostic/lin_diagnostic_service.c
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/diagnostic/lin_diagnostic_service.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/coreapi/lin_common_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/coreapi/lin_j2602_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/coreapi/lin_lin21_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/include/LIN_Driver/bsp/UART/lin_lld_timesrv.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/Project_Settings/Startup_Code/startup_SKEAZ1284.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/Project_Settings/Startup_Code/system_SKEAZ1284.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/transport/lin_commontl_api.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/transport/lin_commontl_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/transport/lin_j2602tl_api.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/transport/lin_lin21tl_api.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/diagnostic/lin_diagnostic_service.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/coreapi/lin_common_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/coreapi/lin_j2602_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/coreapi/lin_lin21_proto.o
	echo make warning: the file path exceeds 255 characters and may cause unexpected errors: C:/Users/B48683/Documents/Auto\ Apps/FRDM+\ Firmware/Quick\ Start\ Packages/QSP\ Raw\ Material/SDK\ Drivers\ and\ Code\ Example\ Projects/FRDM-KEAZ64/Code\ Project\ Examples\ for\ FRDM-KEAZ64/Lab10_LIN_Master_KEA64_PROD/Debug/KEA\ SDK/LIN_Driver/bsp/UART/lin_lld_timesrv.o

