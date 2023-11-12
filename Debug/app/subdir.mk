################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app/ComHandlerTask.cpp \
../app/TemplateTask.cpp \
../app/app_init.cpp 

OBJS += \
./app/ComHandlerTask.o \
./app/TemplateTask.o \
./app/app_init.o 

CPP_DEPS += \
./app/ComHandlerTask.d \
./app/TemplateTask.d \
./app/app_init.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.cpp app/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../os -I../app -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/ComHandlerTask.cyclo ./app/ComHandlerTask.d ./app/ComHandlerTask.o ./app/ComHandlerTask.su ./app/TemplateTask.cyclo ./app/TemplateTask.d ./app/TemplateTask.o ./app/TemplateTask.su ./app/app_init.cyclo ./app/app_init.d ./app/app_init.o ./app/app_init.su

.PHONY: clean-app

