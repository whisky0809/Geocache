################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/utils/joystick/joystick.c 

C_DEPS += \
./source/utils/joystick/joystick.d 

OBJS += \
./source/utils/joystick/joystick.o 


# Each subdirectory must supply rules for building sources it contributes
source/utils/joystick/%.o: ../source/utils/joystick/%.c source/utils/joystick/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MCXA153VLH -DCPU_MCXA153VLH_cm33_nodsp -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=0 -DMCUX_META_BUILD -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\source" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\CMSIS" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\CMSIS\m-profile" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\device" -I"C:\Users\henri\Documents\MCUXpressoIDE_24.12.148\workspace\frdmmcxa153_ese_driver_examples_lpuart_interrupt\device\periph" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33+nodsp -c -ffunction-sections -fdata-sections -fno-builtin -u _print_float -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-utils-2f-joystick

clean-source-2f-utils-2f-joystick:
	-$(RM) ./source/utils/joystick/joystick.d ./source/utils/joystick/joystick.o

.PHONY: clean-source-2f-utils-2f-joystick

