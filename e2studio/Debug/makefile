################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/SEGGER_RTT/subdir.mk
-include src/subdir.mk
-include ra_gen/subdir.mk
-include ra/fsp/src/r_ssi/subdir.mk
-include ra/fsp/src/r_ioport/subdir.mk
-include ra/fsp/src/r_iic_master/subdir.mk
-include ra/fsp/src/r_gpt/subdir.mk
-include ra/fsp/src/r_dtc/subdir.mk
-include ra/fsp/src/bsp/mcu/ra8p1/subdir.mk
-include ra/fsp/src/bsp/mcu/all/subdir.mk
-include ra/fsp/src/bsp/cmsis/Device/RENESAS/Source/subdir.mk
-include ra/board/ra8p1_ek/subdir.mk
-include ra/arm/CMSIS_6/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
$(C_DEPS):$(SMS_ASM)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
OBJCOPY += \
ssi_ek_ra8p1_ep.hex \

SECONDARY_SIZE += \
ssi_ek_ra8p1_ep.siz \


# All Target
all: ssi_ek_ra8p1_ep.elf secondary-outputs

# Tool invocations
ssi_ek_ra8p1_ep.elf: $(OBJS) $(USER_OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(LINKER_SCRIPT)
	$(file > ssi_ek_ra8p1_ep.elf.in,-mcpu=cortex-m85 -mthumb -mlittle-endian -mfloat-abi=hard -O2 -ffunction-sections -fdata-sections -fmessage-length=0 -funsigned-char -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Waggregate-return -Wno-parentheses-equality -Wfloat-equal -gdwarf-4 -g3 -o "ssi_ek_ra8p1_ep.elf" $(OBJS) $(USER_OBJS) -T "fsp.lld" -L"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/script" -L"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/Debug" -Wl,-Map=ssi_ek_ra8p1_ep.map -Wl,--start-group -lcrt0 -lc++abi  -Wl,--end-group -Wl,--gc-sections -Wl,--cref -Wl,--icf=none -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/src" -I"." -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/fsp/inc" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/fsp/inc/api" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/fsp/inc/instances" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra_gen" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra_cfg/fsp_cfg/bsp" -I"/Users/kim_kyoungkun/Desktop/ssi/ssi_ek_ra8p1_ep/e2studio/ra_cfg/fsp_cfg")
	@echo Building target: $@ && clang  --target=arm-none-eabi @"ssi_ek_ra8p1_ep.elf.in"> ssi_ek_ra8p1_ep.cref
ssi_ek_ra8p1_ep.hex: ssi_ek_ra8p1_ep.elf
	llvm-objcopy "ssi_ek_ra8p1_ep.elf" -O ihex  "ssi_ek_ra8p1_ep.hex"
ssi_ek_ra8p1_ep.siz: ssi_ek_ra8p1_ep.elf
	llvm-size --format=berkeley "ssi_ek_ra8p1_ep.elf"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(S_DEPS)  $(C_DEPS)  $(CREF)  $(OBJCOPY)  $(SX_DEPS)  $(ASM_UPPER_DEPS)  $(OBJS)  $(SX_UPPER_DEPS)  $(ASM_DEPS)  $(MAP)  $(S_UPPER_DEPS) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  ssi_ek_ra8p1_ep.elf
	-@echo ' '

secondary-outputs: $(OBJCOPY) $(SECONDARY_SIZE)

.PHONY: all clean dependents 

-include ../makefile.targets
