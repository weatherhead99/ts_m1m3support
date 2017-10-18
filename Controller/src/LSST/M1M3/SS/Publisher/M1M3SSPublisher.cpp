/*
 * M1M3SSPublisher.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSPublisher.h>
#include <SAL_m1m3.h>
#include <iostream>
#include <string>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSPublisher::M1M3SSPublisher(SAL_m1m3* m1m3SAL) {
	this->m1m3SAL = m1m3SAL;
	this->m1m3SAL->salTelemetryPub("m1m3_InclinometerData");
	this->m1m3SAL->salTelemetryPub("m1m3_IMSData");
	this->m1m3SAL->salEvent("m1m3_logevent_SummaryState");
	this->m1m3SAL->salEvent("m1m3_logevent_SettingVersions");
	this->m1m3SAL->salEvent("m1m3_logevent_InclinometerSensorWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_DisplacementSensorWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_ILCWarning");
	this->m1m3SAL->salEvent("m1m3_logevent_AirSupplyStatus");
	this->m1m3SAL->salEvent("m1m3_logevent_AirSupplyWarning");
	this->m1m3SAL->salTelemetryPub("m1m3_ForceActuatorStatus");
	this->m1m3SAL->salTelemetryPub("m1m3_ForceActuatorData");
	this->m1m3SAL->salTelemetryPub("m1m3_HardpointStatus");
	this->m1m3SAL->salTelemetryPub("m1m3_HardpointData");
}

double M1M3SSPublisher::getTimestamp() {
	return this->m1m3SAL->getCurrentTime();
}

void M1M3SSPublisher::putInclinometerData(m1m3_InclinometerDataC* data) {
	this->m1m3SAL->putSample_InclinometerData(data);
}

void M1M3SSPublisher::putIMSData(m1m3_IMSDataC* data) {
	this->m1m3SAL->putSample_IMSData(data);
}

void M1M3SSPublisher::logStateChange(m1m3_logevent_SummaryStateC* data) {
	this->m1m3SAL->logEvent_SummaryState(data, 0);
}

void M1M3SSPublisher::logSettingVersions(m1m3_logevent_SettingVersionsC* data) {
	this->m1m3SAL->logEvent_SettingVersions(data, 0);
}

void M1M3SSPublisher::logInclinometerSensorWarning(m1m3_logevent_InclinometerSensorWarningC* data) {
	this->m1m3SAL->logEvent_InclinometerSensorWarning(data, 0);
}

void M1M3SSPublisher::logDisplacementSensorWarning(m1m3_logevent_DisplacementSensorWarningC* data) {
	this->m1m3SAL->logEvent_DisplacementSensorWarning(data, 0);
}

void M1M3SSPublisher::logILCWarning(m1m3_logevent_ILCWarningC* data) {
	this->m1m3SAL->logEvent_ILCWarning(data, 0);
}

void M1M3SSPublisher::logAirSupplyStatus(m1m3_logevent_AirSupplyStatusC* data) {
	this->m1m3SAL->logEvent_AirSupplyStatus(data, 0);
}

void M1M3SSPublisher::logAirSupplyWarning(m1m3_logevent_AirSupplyWarningC* data) {
	this->m1m3SAL->logEvent_AirSupplyWarning(data, 0);
}

void M1M3SSPublisher::putForceActuatorStatus(m1m3_ForceActuatorStatusC* data) {
	this->m1m3SAL->putSample_ForceActuatorStatus(data);
}

void M1M3SSPublisher::putForceActuatorData(m1m3_ForceActuatorDataC* data) {
	this->m1m3SAL->putSample_ForceActuatorData(data);
}

void M1M3SSPublisher::putHardpointStatus(m1m3_HardpointStatusC* data) {
	this->m1m3SAL->putSample_HardpointStatus(data);
}

void M1M3SSPublisher::putHardpointData(m1m3_HardpointDataC* data) {
	this->m1m3SAL->putSample_HardpointData(data);
}

void M1M3SSPublisher::ackCommandStart(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Start(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandEnable(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Enable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandDisable(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Disable(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandStandby(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Standby(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandShutdown(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_Shutdown(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnAirOn(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnAirOn(commandID, ackCode, 0, (char*)description.c_str());
}

void M1M3SSPublisher::ackCommandTurnAirOff(int32_t commandID, int32_t ackCode, std::string description) {
	this->m1m3SAL->ackCommand_TurnAirOff(commandID, ackCode, 0, (char*)description.c_str());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
