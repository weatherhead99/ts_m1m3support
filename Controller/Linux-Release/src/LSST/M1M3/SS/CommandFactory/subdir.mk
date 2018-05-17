################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/LSST/M1M3/SS/CommandFactory/CommandFactory.cpp 

OBJS += \
./src/LSST/M1M3/SS/CommandFactory/CommandFactory.o 

CPP_DEPS += \
./src/LSST/M1M3/SS/CommandFactory/CommandFactory.d 


# Each subdirectory must supply rules for building sources it contributes
src/LSST/M1M3/SS/CommandFactory/%.o: ../src/LSST/M1M3/SS/CommandFactory/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/usr/boost_1_65_1" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/examples/include" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/examples" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/include" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/include/sys" -I"/usr/ts_opensplice/OpenSpliceDDS/V6.7/HDE/x86_64.linux/include/dcps/C++/SACPP" -I"/usr/ts_sal/test/m1m3/cpp/src" -I"/usr/ts_sal/test/m1m3/cpp" -I"/usr/ts_sal/test/MTMount/cpp/src" -I"/usr/ts_sal/lsstsal/include" -I"/usr/ts_sal/test/m1m3/cpp" -I"/usr/ts_sal/test/MTMount/cpp" -I"/usr/ts_sal/test" -I"/usr/ts_sal/test/include" -I"../src/LSST/M1M3/SS/DigitalInputOutput" -I"../src/LSST/M1M3/SS/FirmwareUpdate" -I"../src/LSST/M1M3/SS/Accelerometer" -I"../src/LSST/M1M3/SS/AutomaticOperationsController" -I"../src/LSST/M1M3/SS/BusLists" -I"../src/LSST/M1M3/SS/CommandFactory" -I"../src/LSST/M1M3/SS/Displacement" -I"../src/LSST/M1M3/SS/Inclinometer" -I"../src/LSST/M1M3/SS/ForceController" -I"../src/LSST/M1M3/SS/Commands" -I"../src/LSST/M1M3/SS/Context" -I"../src/LSST/M1M3/SS/Controller" -I"../src/LSST/M1M3/SS/FPGA" -I"../src/LSST/M1M3/SS/Gyro" -I"../src/LSST/M1M3/SS/ILC" -I"../src/LSST/M1M3/SS/Include" -I"../src/LSST/M1M3/SS/Logging" -I"../src/LSST/M1M3/SS/Modbus" -I"../src/LSST/M1M3/SS/Model" -I"../src/LSST/M1M3/SS/PID" -I"../src/LSST/M1M3/SS/PositionController" -I"../src/LSST/M1M3/SS/PowerController" -I"../src/LSST/M1M3/SS/ProfileController" -I"../src/LSST/M1M3/SS/Publisher" -I"../src/LSST/M1M3/SS/SafetyController" -I"../src/LSST/M1M3/SS/Settings" -I"../src/LSST/M1M3/SS/StateFactory" -I"../src/LSST/M1M3/SS/States" -I"../src/LSST/M1M3/SS/Subscriber" -I"../src/LSST/M1M3/SS/Threads" -I"../src/LSST/M1M3/SS/Utility" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


