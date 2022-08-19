/*
 * WriteMulti.h
 *
 *  Created on: Apr 30, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_WRITEMULTI_H_
#define COUCHBASE_WRITEMULTI_H_

#include "Decl.h"
#include "Command.h"
#include "DataProvider.h"
#include "Write.h"

namespace Couchbase {

class WriteMulti: public Command {
public:
    WriteMulti(const DataMap& dataMap);

    void schedule(lcb_INSTANCE *connection) override;
    void reject(const std::string& reason) override;

    void wait();
private:
    std::vector<WritePtr> m_commands;
};

} /* namespace Couchbase */

#endif /* COUCHBASE_WRITEMULTI_H_ */
