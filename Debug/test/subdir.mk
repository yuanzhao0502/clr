################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/test_graph.cpp \
../test/test_run_metis_clr.cpp 

OBJS += \
./test/test_graph.o \
./test/test_run_metis_clr.o 

CPP_DEPS += \
./test/test_graph.d \
./test/test_run_metis_clr.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/mac-97-41/Work/cluster_local_restrictions/src/clr/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


