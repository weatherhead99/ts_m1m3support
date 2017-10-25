/*
 * ILC.h
 *
 *  Created on: Oct 1, 2017
 *      Author: ccontaxis
 */

#ifndef ILC_H_
#define ILC_H_

#include <IILC.h>
#include <ILCDataTypes.h>
#include <ILCSubnetData.h>
#include <ModbusBuffer.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <ILCMessageFactory.h>
#include <SetADCChanneOffsetAndSensitivityBusList.h>
#include <ChangeILCModeBusList.h>
#include <ReadBoostValveDCAGainBusList.h>
#include <ReadCalibrationBusList.h>
#include <ReportADCScanRateBusList.h>
#include <ReportDCAIDBusList.h>
#include <ReportDCAStatusBusList.h>
#include <ReportServerIDBusList.h>
#include <ReportServerStatusBusList.h>
#include <ResetBustList.h>
#include <SetADCScanRateBusList.h>
#include <SetBoostValveDCAGainBusList.h>
#include <FreezeSensorBusList.h>
#include <RaisedBusList.h>
#include <ILCResponseParser.h>

struct m1m3_logevent_HardpointActuatorInfoC;
struct m1m3_HardpointStatusC;
struct m1m3_HardpointDataC;

struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_ForceActuatorStatusC;
struct m1m3_ForceActuatorDataC;

struct m1m3_logevent_ILCWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;
class IBusList;

/*!
 * The ILC class used to communicate with the M1M3's 5 subnets.
 */
class ILC: public IILC {
private:
	IPublisher* publisher;
	IFPGA* fpga;

	ILCApplicationSettings* ilcApplicationSettings;

	ILCSubnetData subnetData;
	ILCMessageFactory ilcMessageFactory;

	SetADCChanneOffsetAndSensitivityBusList busListSetADCChannelOffsetAndSensitivity;
	SetADCScanRateBusList busListSetADCScanRate;
	SetBoostValveDCAGainBusList busListSetBoostValveDCAGains;
	ResetBustList busListReset;
	ReportServerIDBusList busListReportServerID;
	ReportServerStatusBusList busListReportServerStatus;
	ReportADCScanRateBusList busListReportADCScanRate;
	ReadCalibrationBusList busListReadCalibration;
	ReadBoostValveDCAGainBusList busListReadBoostValveDCAGains;
	ReportDCAIDBusList busListReportDCAID;
	ReportDCAStatusBusList busListReportDCAStatus;
	ChangeILCModeBusList busListChangeILCModeDisabled;
	ChangeILCModeBusList busListChangeILCModeEnabled;
	ChangeILCModeBusList busListChangeILCModeStandby;
	FreezeSensorBusList busListFreezeSensor;
	RaisedBusList busListRaised;

	int8_t hpStepCommand[6];
	int32_t hpSteps[6];

	uint16_t u16Buffer[1];
	ModbusBuffer rxBuffer;

	m1m3_logevent_HardpointActuatorInfoC* hardpointInfo;
	m1m3_logevent_ForceActuatorInfoC* forceInfo;

	ILCResponseParser responseParser;


public:
	ILC(IPublisher* publisher, IFPGA* fpga, ILCApplicationSettings* ilcApplicationSettings, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings);
	virtual ~ILC();

	void writeCalibrationDataBuffer();
	void writeSetADCScanRateBuffer();
	void writeSetBoostValveDCAGainBuffer();
	void writeResetBuffer();
	void writeReportServerIDBuffer();
	void writeReportServerStatusBuffer();
	void writeReportADCScanRateBuffer();
	void writeReadCalibrationDataBuffer();
	void writeReadBoostValveDCAGainBuffer();
	void writeReportDCAIDBuffer();
	void writeReportDCAStatusBuffer();
	void writeSetModeDisableBuffer();
	void writeSetModeEnableBuffer();
	void writeSetModeStandbyBuffer();
	void writeFreezeSensorListBuffer();
	void writeRaisedListBuffer();

	void triggerModbus();

	void waitForSubnet(int32_t subnet, int32_t timeout);
	void waitForAllSubnets(int32_t timeout);

	void read(uint8_t subnet);
	void readAll();

	void verifyResponses();

	void publishForceActuatorStatus();
	void publishForceActuatorData();
	void publishHardpointStatus();
	void publishHardpointData();

private:
	uint8_t subnetToRxAddress(uint8_t subnet);

	void writeBusList(IBusList* busList);

	void updateHPSteps();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILC_H_ */
