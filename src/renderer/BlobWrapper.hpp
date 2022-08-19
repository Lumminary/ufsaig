/*
 * BlobWrapper.hpp
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#ifndef TRANSFORMER_BLOBWRAPPER_HPP_
#define TRANSFORMER_BLOBWRAPPER_HPP_

#include <Magick++.h>
#include "DataProvider.h"
#include <memory>

namespace ImageServer {

class BlobWrapper : public DataProvider {
public:
    BlobWrapper(const Magick::Blob& blob) : m_blob{ blob } {}
    BlobWrapper(const BlobWrapper& other) = default;
    BlobWrapper(BlobWrapper&& other) = default;
    BlobWrapper() = default;

    const void* bytes() const override { return m_blob.data(); }
    size_t size() const override { return m_blob.length(); }

    const Magick::Blob& blob() const { return m_blob; }
    Magick::Blob& blob() { return m_blob; }

    virtual ~BlobWrapper() = default;
private:
    Magick::Blob m_blob;
};

using BlobWrapperPtr = std::shared_ptr<BlobWrapper>;

}


#endif /* TRANSFORMER_BLOBWRAPPER_HPP_ */
