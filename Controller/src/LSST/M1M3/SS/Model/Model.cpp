/*
 * Model.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <Model.h>
#include <ISettingReader.h>
#include <IPublisher.h>
#include <RS232.h>
#include <ILC.h>
#include <Timestamp.h>
#include <ILCApplicationSettings.h>
#include <U16ArrayUtilities.h>
#include <iostream>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <ForceCalculator.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga) {
	this->settingReader = settingReader;
	this->publisher = publisher;
	this->fpga = fpga;
	this->rs232 = 0;
	this->ilc = 0;
	this->airController = 0;
	this->forceCalculator = 0;
	pthread_mutex_init(&this->mutex, NULL);
	pthread_mutex_lock(&this->mutex);
}

Model::~Model() {
	pthread_mutex_unlock(&this->mutex);
	pthread_mutex_destroy(&this->mutex);
	if (this->rs232) {
		delete this->rs232;
	}
	if (this->ilc) {
		delete this->ilc;
	}
	if (this->airController) {
		delete this->airController;
	}
	if (this->forceCalculator) {
		delete this->forceCalculator;
	}
}

void Model::loadSettings(std::string settingsToApply) {
	this->settingReader->configure(settingsToApply);
	if (this->rs232) {
		delete this->rs232;
	}
	this->rs232 = new RS232(this->fpga, this->publisher);
	ILCApplicationSettings* ilcApplicationSettings = this->settingReader->loadILCApplicationSettings();
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings = this->settingReader->loadForceActuatorApplicationSettings();
	HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings = this->settingReader->loadHardpointActuatorApplicationSettings();
	if (this->ilc) {
		delete this->ilc;
	}
	this->ilc = new ILC(this->publisher, this->fpga, ilcApplicationSettings, forceActuatorApplicationSettings, hardpointActuatorApplicationSettings);
	if (this->airController) {
		delete this->airController;
	}
	this->airController = new AirController(this->publisher, this->fpga);
	this->forceCalculator = new ForceCalculator(forceActuatorApplicationSettings, this->publisher->getEventForceActuatorInfo(), this->publisher->getInclinometerData(), this->publisher->getForceActuatorData());
}

void Model::queryFPGAData() {

}

void Model::publishFPGAData() {
	uint16_t response[512];
	this->fpga->writeRequestFIFO(FPGAAddresses::HealthAndStatus, 0);
	this->fpga->readU16ResponseFIFO(response, 64*4, 20);
	for(int i = 0; i < 25; i++) {
		cout << U16ArrayUtilities::u64(response, i * 4) << " ";
	}
	cout << endl;
}

void Model::publishStateChange(States::Type newState) {
	m1m3_logevent_SummaryStateC* data = this->publisher->getEventSummaryState();
	data->Timestamp = Timestamp::currentTime();
	data->SummaryState = newState;
	this->publisher->logSummaryState();
}

void Model::publishRecommendedSettings() {
	RecommendedApplicationSettings* recommendedApplicationSettings = this->settingReader->loadRecommendedApplicationSettings();
	m1m3_logevent_SettingVersionsC* data = this->publisher->getEventSettingVersions();
	data->Timestamp = Timestamp::currentTime();
	data->RecommendedSettingsVersion = "";
	for(uint32_t i = 0; i < recommendedApplicationSettings->RecommendedSettings.size(); i++) {
		data->RecommendedSettingsVersion += recommendedApplicationSettings->RecommendedSettings[i] + ",";
	}
	this->publisher->logSettingVersions();
}

void Model::shutdown() {
	pthread_mutex_unlock(&this->mutex);
}

void Model::waitForShutdown() {
	pthread_mutex_lock(&this->mutex);
	pthread_mutex_unlock(&this->mutex);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
