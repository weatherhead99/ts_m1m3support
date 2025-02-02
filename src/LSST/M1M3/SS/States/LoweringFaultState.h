/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef LOWERINGFAULTSTATE_H_
#define LOWERINGFAULTSTATE_H_

#include <FaultState.h>
#include <UpdateCommand.h>
#include <Model.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Lowering mirror during fault.
 */
class LoweringFaultState : public FaultState {
public:
    LoweringFaultState();

    virtual States::Type update(UpdateCommand* command) override;

    /**
     * Perform actions needed to safe mirror on fault.
     */
    static void ensureFaulted();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERINGFAULTSTATE_H_ */
