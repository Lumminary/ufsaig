/*
 * Client.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_DECL_H_
#define COUCHBASE_DECL_H_

#include <memory>
#include <libcouchbase/couchbase.h>
#include <vector>
#include <unordered_map>
#include <string>

namespace Couchbase {

class CreateOpts;

class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

class Command;
using CommandPtr = std::shared_ptr<Command>;

class Worker;
using WorkerPtr = std::shared_ptr<Worker>;

class Read;
using ReadPtr = std::shared_ptr<Read>;

class Write;
using WritePtr = std::shared_ptr<Write>;

struct Config;

struct Entry;
using EntryPtr = std::shared_ptr<Entry>;

using Buffer = std::vector<char>;

using GetResults = std::unordered_map<std::string, EntryPtr>;

}

#endif /* COUCHBASE_DECL_H_ */
