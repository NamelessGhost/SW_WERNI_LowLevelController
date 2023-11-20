################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../os/Message.cpp \
../os/MsgPool.cpp \
../os/Mutex.cpp \
../os/TaskClass.cpp \
../os/TaskList.cpp \
../os/Timer.cpp \
../os/operators.cpp 

OBJS += \
./os/Message.o \
./os/MsgPool.o \
./os/Mutex.o \
./os/TaskClass.o \
./os/TaskList.o \
./os/Timer.o \
./os/operators.o 

CPP_DEPS += \
./os/Message.d \
./os/MsgPool.d \
./os/Mutex.d \
./os/TaskClass.d \
./os/TaskList.d \
./os/Timer.d \
./os/operators.d 


# Each subdirectory must supply rules for building sources it contributes
os/%.o os/%.su os/%.cyclo: ../os/%.cpp os/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G491xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../os -I../app -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-os

clean-os:
	-$(RM) ./os/Message.cyclo ./os/Message.d ./os/Message.o ./os/Message.su ./os/MsgPool.cyclo ./os/MsgPool.d ./os/MsgPool.o ./os/MsgPool.su ./os/Mutex.cyclo ./os/Mutex.d ./os/Mutex.o ./os/Mutex.su ./os/TaskClass.cyclo ./os/TaskClass.d ./os/TaskClass.o ./os/TaskClass.su ./os/TaskList.cyclo ./os/TaskList.d ./os/TaskList.o ./os/TaskList.su ./os/Timer.cyclo ./os/Timer.d ./os/Timer.o ./os/Timer.su ./os/operators.cyclo ./os/operators.d ./os/operators.o ./os/operators.su

.PHONY: clean-os

