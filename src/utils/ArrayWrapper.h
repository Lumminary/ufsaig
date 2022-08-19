/*
 * ArrayWrapper.h
 *
 *  Created on: Apr 23, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_ARRAYWRAPPER_H_
#define UTILS_ARRAYWRAPPER_H_

#include "DataProvider.h"

namespace ImageServer {

namespace Utils {

template<typename Item>
struct ArrayWrapper : public DataProvider {
    std::vector<Item> data;

    const void* bytes() const override { return data.data(); }
    size_t size() const override { return data.size() * sizeof(Item); };
};

} /* namespace Utils */

} /* namespace ImageServer */

#endif /* UTILS_ARRAYWRAPPER_H_ */
