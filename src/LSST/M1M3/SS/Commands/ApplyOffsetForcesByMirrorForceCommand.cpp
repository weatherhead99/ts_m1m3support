/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyOffsetForcesByMirrorForceCommand::ApplyOffsetForcesByMirrorForceCommand(
        int32_t commandID, MTM1M3_command_applyOffsetForcesByMirrorForceC* data) {
    this->commandID = commandID;
    _data.xForce = data->xForce;
    _data.yForce = data->yForce;
    _data.zForce = data->zForce;
    _data.xMoment = data->xMoment;
    _data.yMoment = data->yMoment;
    _data.zMoment = data->zMoment;
}

void ApplyOffsetForcesByMirrorForceCommand::execute() { Context::get().applyOffsetForcesByMirrorForce(this); }

void ApplyOffsetForcesByMirrorForceCommand::ackInProgress() {
    M1M3SSPublisher::get().ackCommandapplyOffsetForcesByMirrorForce(this->commandID, ACK_INPROGRESS,
                                                                    "In-Progress");
}

void ApplyOffsetForcesByMirrorForceCommand::ackComplete() {
    M1M3SSPublisher::get().ackCommandapplyOffsetForcesByMirrorForce(this->commandID, ACK_COMPLETE,
                                                                    "Complete");
}

void ApplyOffsetForcesByMirrorForceCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandapplyOffsetForcesByMirrorForce(this->commandID, ACK_FAILED,
                                                                    "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
