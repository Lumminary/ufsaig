/*
 * DataProvider.h
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#ifndef TRANSFORMER_DATAPROVIDER_H_
#define TRANSFORMER_DATAPROVIDER_H_

#include <memory>
#include <unordered_map>
#include <string>

class DataProvider {
public:
    virtual const void* bytes() const = 0;
    virtual size_t size() const = 0;
    virtual ~DataProvider() = default;
};

using DataProviderPtr = std::shared_ptr<DataProvider>;
using DataMap = std::unordered_map<std::string, DataProviderPtr>;

#endif /* TRANSFORMER_DATAPROVIDER_H_ */
