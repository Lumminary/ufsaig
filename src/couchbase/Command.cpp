/*
 * Command.cpp
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#include "Command.h"
#include "Read.h"
#include "Write.h"

namespace Couchbase {

//------------------------------------------------------------------------------

void Command::onRead(lcb_INSTANCE *instance,
        int type,
        const lcb_RESPGET *response) {
    Read *command = nullptr;
    lcb_respget_cookie(response, (void**) &command);
    command->complete(response);
}

//------------------------------------------------------------------------------

void Command::onWrite(lcb_INSTANCE *instance,
        int type,
        const lcb_RESPSTORE *response) {
    Write *command = nullptr;
    lcb_respstore_cookie(response, (void**) &command);
    command->complete(response);
}

} /* namespace couchbase */
