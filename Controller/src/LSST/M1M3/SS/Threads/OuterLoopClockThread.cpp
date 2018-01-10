/*
 * OuterLoopClockThread.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <OuterLoopClockThread.h>
#include <ICommandFactory.h>
#include <IController.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <ICommand.h>
#include <IPublisher.h>
#include <Timestamp.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OuterLoopClockThread::OuterLoopClockThread(ICommandFactory* commandFactory, IController* controller, IFPGA* fpga, IPublisher* publisher) {
	this->commandFactory = commandFactory;
	this->controller = controller;
	this->fpga = fpga;
	this->publisher = publisher;
	this->keepRunning = true;
	pthread_mutex_init(&this->updateMutex, NULL);
}

OuterLoopClockThread::~OuterLoopClockThread() {
	pthread_mutex_destroy(&this->updateMutex);
}

void OuterLoopClockThread::run() {
	while(this->keepRunning) {
		this->fpga->waitForOuterLoopClock(-1);
		uint64_t timestamp = Timestamp::toRaw(this->publisher->getTimestamp());
		this->controller->lock();
		if (this->keepRunning) {
			this->timestampUpdateBuffer[0] = FPGAAddresses::Timestamp;
			this->timestampUpdateBuffer[1] = (timestamp >> 48) & 0xFFFF;
			this->timestampUpdateBuffer[2] = (timestamp >> 32) & 0xFFFF;
			this->timestampUpdateBuffer[3] = (timestamp >> 16) & 0xFFFF;
			this->timestampUpdateBuffer[4] = (timestamp >> 0) & 0xFFFF;
			this->fpga->writeCommandFIFO(this->timestampUpdateBuffer, 5, 0);
			this->controller->enqueue(this->commandFactory->create(Commands::UpdateCommand, &this->updateMutex));
		}
		this->controller->unlock();
		pthread_mutex_lock(&this->updateMutex);
		pthread_mutex_unlock(&this->updateMutex);
		this->fpga->ackOuterLoopClock();
	}
}

void OuterLoopClockThread::stop() {
	this->keepRunning = false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
