################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BootContext.cpp \
../src/Ciphertext.cpp \
../src/EvaluatorUtils.cpp \
../src/Key.cpp \
../src/MKScheme.cpp \
../src/Plaintext.cpp \
../src/Ring.cpp \
../src/RingMultiplier.cpp \
../src/Scheme.cpp \
../src/SchemeAlgo.cpp \
../src/SecretKey.cpp \
../src/SerializationUtils.cpp \
../src/StringUtils.cpp \
../src/TestScheme.cpp \
../src/TimeUtils.cpp \
../src/client.cpp \
../src/server.cpp \
../src/MKScheme_server.cpp \
../src/MKScheme_client.cpp


OBJS += \
./src/BootContext.o \
./src/Ciphertext.o \
./src/EvaluatorUtils.o \
./src/Key.o \
./src/MKScheme.o \
./src/Plaintext.o \
./src/Ring.o \
./src/RingMultiplier.o \
./src/Scheme.o \
./src/SchemeAlgo.o \
./src/SecretKey.o \
./src/SerializationUtils.o \
./src/StringUtils.o \
./src/TestScheme.o \
./src/TimeUtils.o \
./src/client.o \
./src/server.o \
./src/MKScheme_server.o \
./src/MKScheme_client.o


CPP_DEPS += \
./src/BootContext.d \
./src/Ciphertext.d \
./src/EvaluatorUtils.d \
./src/Key.d \
./src/MKScheme.d \
./src/Plaintext.d \
./src/Ring.d \
./src/RingMultiplier.d \
./src/Scheme.d \
./src/SchemeAlgo.d \
./src/SecretKey.d \
./src/SerializationUtils.d \
./src/StringUtils.d \
./src/TestScheme.d \
./src/TimeUtils.d \
./src/client.d \
./src/server.d \
./src/MKScheme_server.d \
./src/MKScheme_client.d  


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -O3 -c -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


