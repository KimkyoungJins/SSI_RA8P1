################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/da7212_speaker_init.c \
../src/hal_entry.c \
../src/ssi_ep.c 

C_DEPS += \
./src/da7212_speaker_init.d \
./src/hal_entry.d \
./src/ssi_ep.d 

CREF += \
ssi_ek_ra8p1_ep.cref 

OBJS += \
./src/da7212_speaker_init.o \
./src/hal_entry.o \
./src/ssi_ep.o 

MAP += \
ssi_ek_ra8p1_ep.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-m85 -mthumb -mlittle-endian -mfloat-abi=hard -O2 -ffunction-sections -fdata-sections -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -gdwarf-4 -g3 -std=c99 -flax-vector-conversions -fshort-enums -fno-unroll-loops -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/src" -I"." -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/fsp/inc" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/fsp/inc/api" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/fsp/inc/instances" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra_gen" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra_cfg/fsp_cfg/bsp" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra_cfg/fsp_cfg" -D_RENESAS_RA_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -x c "$<" -c -o "$@")
	@clang --target=arm-none-eabi @"$@.in"

