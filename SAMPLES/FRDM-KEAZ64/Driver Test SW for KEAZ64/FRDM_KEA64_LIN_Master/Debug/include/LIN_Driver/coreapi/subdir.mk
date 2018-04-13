################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/LIN_Driver/coreapi/lin_common_api.c" \
"../include/LIN_Driver/coreapi/lin_common_proto.c" \
"../include/LIN_Driver/coreapi/lin_j2602_api.c" \
"../include/LIN_Driver/coreapi/lin_j2602_proto.c" \
"../include/LIN_Driver/coreapi/lin_lin21_api.c" \
"../include/LIN_Driver/coreapi/lin_lin21_proto.c" \

C_SRCS += \
../include/LIN_Driver/coreapi/lin_common_api.c \
../include/LIN_Driver/coreapi/lin_common_proto.c \
../include/LIN_Driver/coreapi/lin_j2602_api.c \
../include/LIN_Driver/coreapi/lin_j2602_proto.c \
../include/LIN_Driver/coreapi/lin_lin21_api.c \
../include/LIN_Driver/coreapi/lin_lin21_proto.c \

OBJS += \
./include/LIN_Driver/coreapi/lin_common_api.o \
./include/LIN_Driver/coreapi/lin_common_proto.o \
./include/LIN_Driver/coreapi/lin_j2602_api.o \
./include/LIN_Driver/coreapi/lin_j2602_proto.o \
./include/LIN_Driver/coreapi/lin_lin21_api.o \
./include/LIN_Driver/coreapi/lin_lin21_proto.o \

C_DEPS += \
./include/LIN_Driver/coreapi/lin_common_api.d \
./include/LIN_Driver/coreapi/lin_common_proto.d \
./include/LIN_Driver/coreapi/lin_j2602_api.d \
./include/LIN_Driver/coreapi/lin_j2602_proto.d \
./include/LIN_Driver/coreapi/lin_lin21_api.d \
./include/LIN_Driver/coreapi/lin_lin21_proto.d \

OBJS_QUOTED += \
"./include/LIN_Driver/coreapi/lin_common_api.o" \
"./include/LIN_Driver/coreapi/lin_common_proto.o" \
"./include/LIN_Driver/coreapi/lin_j2602_api.o" \
"./include/LIN_Driver/coreapi/lin_j2602_proto.o" \
"./include/LIN_Driver/coreapi/lin_lin21_api.o" \
"./include/LIN_Driver/coreapi/lin_lin21_proto.o" \

OBJS_OS_FORMAT += \
./include/LIN_Driver/coreapi/lin_common_api.o \
./include/LIN_Driver/coreapi/lin_common_proto.o \
./include/LIN_Driver/coreapi/lin_j2602_api.o \
./include/LIN_Driver/coreapi/lin_j2602_proto.o \
./include/LIN_Driver/coreapi/lin_lin21_api.o \
./include/LIN_Driver/coreapi/lin_lin21_proto.o \

C_DEPS_QUOTED += \
"./include/LIN_Driver/coreapi/lin_common_api.d" \
"./include/LIN_Driver/coreapi/lin_common_proto.d" \
"./include/LIN_Driver/coreapi/lin_j2602_api.d" \
"./include/LIN_Driver/coreapi/lin_j2602_proto.d" \
"./include/LIN_Driver/coreapi/lin_lin21_api.d" \
"./include/LIN_Driver/coreapi/lin_lin21_proto.d" \


# Each subdirectory must supply rules for building sources it contributes
include/LIN_Driver/coreapi/lin_common_api.o: ../include/LIN_Driver/coreapi/lin_common_api.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/coreapi/lin_common_api.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/coreapi/lin_common_api.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LIN_Driver/coreapi/lin_common_proto.o: ../include/LIN_Driver/coreapi/lin_common_proto.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/coreapi/lin_common_proto.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/coreapi/lin_common_proto.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LIN_Driver/coreapi/lin_j2602_api.o: ../include/LIN_Driver/coreapi/lin_j2602_api.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/coreapi/lin_j2602_api.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/coreapi/lin_j2602_api.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LIN_Driver/coreapi/lin_j2602_proto.o: ../include/LIN_Driver/coreapi/lin_j2602_proto.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/coreapi/lin_j2602_proto.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/coreapi/lin_j2602_proto.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LIN_Driver/coreapi/lin_lin21_api.o: ../include/LIN_Driver/coreapi/lin_lin21_api.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/coreapi/lin_lin21_api.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/coreapi/lin_lin21_api.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LIN_Driver/coreapi/lin_lin21_proto.o: ../include/LIN_Driver/coreapi/lin_lin21_proto.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LIN_Driver/coreapi/lin_lin21_proto.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "include/LIN_Driver/coreapi/lin_lin21_proto.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


