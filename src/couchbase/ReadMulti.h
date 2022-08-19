/*
 * ReadMulti.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_READMULTI_H_
#define COUCHBASE_READMULTI_H_

#include "Decl.h"
#include "Read.h"
#include <unordered_map>

namespace Couchbase {

class ReadMulti: public Command {
public:
    ReadMulti(const std::vector<std::string>& keys);

    void schedule(lcb_INSTANCE *connection) override;
    void reject(const std::string& reason) override;

    GetResults getResults();
private:
    std::unordered_map<std::string, Read> m_operations;
};


} /* namespace ImageServer */

#endif /* COUCHBASE_READMULTI_H_ */
