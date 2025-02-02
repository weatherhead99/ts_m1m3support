include ../Makefile.inc

all: libM1M3SS.a

.PHONY: DEPS clean

C_SRCS = $(shell find LSST NiFpga -name '*.c')
LIB_CPP_SRCS = $(shell find LSST NiFpga -name '*.cpp')
ALL_CPP_SRCS = $(LIB_CPP_SRCS) $(shell ls *.cpp)

LIB_OBJS = $(patsubst %.c,%.c.o,$(C_SRCS)) $(patsubst %.cpp,%.cpp.o,$(LIB_CPP_SRCS))
ALL_OBJS = $(patsubst %.c,%.c.o,$(C_SRCS)) $(patsubst %.cpp,%.cpp.o,$(ALL_CPP_SRCS))

C_DEPS = $(patsubst %.c,%.c.d,$(C_SRCS))
CPP_DEPS = $(patsubst %.cpp,%.cpp.d,$(ALL_CPP_SRCS))

ifneq ($(MAKECMDGOALS),clean)
  -include ${C_DEPS} ${CPP_DEPS}
endif

M1M3_CPPFLAGS := -I. \
	-I"../$(CRIOCPP)/include" \
	-I"NiFpga" \
	-I"LSST/M1M3/SS/DigitalInputOutput" \
	-I"LSST/M1M3/SS/FirmwareUpdate" \
	-I"LSST/M1M3/SS/Accelerometer" \
	-I"LSST/M1M3/SS/BusLists" \
	-I"LSST/M1M3/SS/Controllers" \
	-I"LSST/M1M3/SS/Displacement" \
	-I"LSST/M1M3/SS/Inclinometer" \
	-I"LSST/M1M3/SS/ForceComponents" \
	-I"LSST/M1M3/SS/Commands" \
	-I"LSST/M1M3/SS/Context" \
	-I"LSST/M1M3/SS/Domain" \
	-I"LSST/M1M3/SS/FPGA" \
	-I"LSST/M1M3/SS/Gyro" \
	-I"LSST/M1M3/SS/ILC" \
	-I"LSST/M1M3/SS/Include" \
	-I"LSST/M1M3/SS/Logging" \
	-I"LSST/M1M3/SS/Modbus" \
	-I"LSST/M1M3/SS/Model" \
	-I"LSST/M1M3/SS/PID" \
	-I"LSST/M1M3/SS/Publisher" \
	-I"LSST/M1M3/SS/Settings" \
	-I"LSST/M1M3/SS/StateFactory" \
	-I"LSST/M1M3/SS/States" \
	-I"LSST/M1M3/SS/Subscriber" \
	-I"LSST/M1M3/SS/Threads" \
	-I"LSST/M1M3/SS/Utility" \
	-DVERSION="\"$(VERSION)\"" \
	-DGIT_HASH="\"${GIT_HASH}\""

libM1M3SS.a: $(LIB_OBJS)
	@echo '[AR ] $@'
	${co}$(AR) rs $@ $^

clean:
	@$(foreach file,$(ALL_OBJS) $(shell ls *.cpp.d *.cpp.o) $(C_DEPS) $(CPP_DEPS) libM1M3SS.a $(shell find -name '*.d'), echo '[RM ] ${file}'; $(RM) -r $(file);)

# file targets
%.cpp.o: %.cpp.d
	@echo '[CPP] $(patsubst %.d,%,$<)'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -c -fmessage-length=0 -o $@ $(patsubst %.d,%,$<)

%.c.o: %.c.d
	@echo '[C  ] $(patsubst %.d,%d,$<)'
	${co}$(C) -c -fmessage-length=0 -o $@ $(patsubst %.d,%,$<)

%.cpp.d: %.cpp
	@echo '[DPP] $<'
	${co}$(CPP) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -M $< -MF $@ -MT '$(patsubst %.cpp,%.o,$<) $@'

%.c.d: %.c
	@echo '[DEP] $<'
	${co}$(C) $(BOOST_CPPFLAGS) $(SAL_CPPFLAGS) $(M1M3_CPPFLAGS) -M $< -MF $@ -MT '$(patsubst %.c,%.o,$<) $@'
