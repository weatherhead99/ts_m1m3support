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

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * @brief Parent class for all commands.
 *
 * Follows Command Pattern from Design Patterns. Encapsulates command executed
 * in M1M3 SS. Commands are created from SAL messages by
 * CommandFactory::create() and M1M3SSSubscriber in SubscriberThread::run().
 */
class Command {
public:
    Command(int32_t commandID) : _commandID(commandID) {}
    virtual ~Command();

    /*!
     * Gets the command ID.
     */
    inline int32_t getCommandID() { return _commandID; }

    /*!
     * Validates the command.
     */
    virtual bool validate();
    /*!
     * Executes the command.
     */
    virtual void execute();
    /*!
     * Acknowledges the command is in progress.
     */
    virtual void ackInProgress();
    /*!
     * Acknowledges the command has completed successfully.
     */
    virtual void ackComplete();
    /*!
     * Acknowledges the command has failed.
     * @param[in] reason The reason why the command has failed.
     */
    virtual void ackFailed(std::string reason);

protected:
    int32_t _commandID;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMAND_H_ */
