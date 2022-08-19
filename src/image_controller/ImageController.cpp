/*
 * ImageController.cpp
 *
 *  Created on: Apr 23, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageController.h"
#include "utils/mime.h"
#include <iostream>
#include "utils/base64.h"
#include "utils/ArrayWrapper.h"
#include <algorithm>

namespace ImageServer {

//------------------------------------------------------------------------------

static DataProviderPtr
makeResult(const ImageDescriptor &desc,
        const DataProviderPtr& rawData,
        const std::string& mimeType) {
    if (!desc.base64)
        return rawData;

    std::shared_ptr<Utils::ArrayWrapper<char>> result =
            std::make_shared<Utils::ArrayWrapper<char>>();

    std::string base64 = "data:" + mimeType + ";base64, " +
            Utils::Base64::encode(
                    (const unsigned char *)rawData->bytes(), rawData->size());

    result->data.assign(base64.begin(), base64.end());

    return result;
}

//------------------------------------------------------------------------------

DataMap
ImageController::getImagesData(
        const std::vector<ImageDescriptor>& descriptors) {
    std::vector<std::string> metaKeys;
    metaKeys.reserve(descriptors.size());
    
    for (const ImageDescriptor& desc : descriptors) {
        metaKeys.push_back(desc.imageKey);
    }

    ImageStore::Images images = imageStore->findImages(metaKeys);
    std::unordered_map<std::string, VersionRequest> versionRequests;
    versionRequests.reserve(descriptors.size());
    std::vector<std::string> rawDataKeys;
    rawDataKeys.reserve(descriptors.size());
    for (const ImageDescriptor& desc : descriptors) {
        versionRequests.emplace(desc.imageKey,
                VersionRequest{ *images[desc.imageKey], desc });
        rawDataKeys.push_back(versionRequests[desc.imageKey].src.key);
    }

    Couchbase::GetResults rawDataMap =
            imageStore->dataConnector->findEntries(rawDataKeys);

    DataMap output;
    output.reserve(descriptors.size());

    DataMap updatedImagesMeta;
    DataMap updatedImagesData;

    for (const ImageDescriptor& desc : descriptors) {
        VersionRequest& versionRequest = versionRequests[desc.imageKey];

        std::string mimeType = Utils::mimeType(versionRequest.type);

        DataProviderPtr origData = rawDataMap[versionRequest.src.key];
        if (versionRequest.isExact) {
            output[desc.imageKey] =
                    makeResult(desc,
                            origData,
                            mimeType);
            continue;
        }

        Size box{ desc.width, desc.height };

        DataProviderPtr newData =
                renderer->aspectFit(versionRequest.src,
                        origData, box, versionRequest.type);

        if (desc.save) {
            ImageVersion newVersion;
            newVersion.extension = versionRequest.type;
            newVersion.width = desc.width;
            newVersion.height = desc.height;
            newVersion.fileName = versionRequest.src.fileName;
            newVersion.mimeType = mimeType;
            newVersion.sizeBytes = newData->size();

            Image& image = *images[desc.imageKey];
            ImageVersion& version = image.addVersion(newVersion);
            updatedImagesData[version.key] = newData;

            updatedImagesMeta[image.key] = image.data();
        }

        output[desc.imageKey] = makeResult(desc, newData, mimeType);
    }

    imageStore->dataConnector->saveEntries(updatedImagesData);
    imageStore->metaConnector->saveEntries(updatedImagesMeta);

    return output;
}

//------------------------------------------------------------------------------

DataProviderPtr
ImageController::getImage(const ImageDescriptor &desc) {
    ImagePtr image = imageStore->findImage(desc.imageKey);
    std::vector<ImageVersion>& versions = image->versions;

    auto original = std::find_if(versions.begin(),
            versions.end(),
            [](const ImageVersion& ver) -> bool {
                return ver.original;
            });

    const std::string& type =
            desc.type.empty() ? original->extension : desc.type;
    std::string mimeType = Utils::mimeType(type);

    if (!desc.original) {
        auto exact = std::find_if(versions.begin(),
                versions.end(),
                [&](const ImageVersion& ver) -> bool {
                    return (ver.width == desc.width)
                            && (ver.height == desc.height)
                            && (ver.extension == type);
                });

        if (exact != versions.end())
            return makeResult(desc,
                    imageStore->dataConnector->findEntry(exact->key),
                    mimeType);
    }

    if (original == versions.end())
        throw std::runtime_error{
                "Original image not found for key " + desc.imageKey};

    Couchbase::EntryPtr origData =
            imageStore->dataConnector->findEntry(original->key);

    if (desc.original)
        return makeResult(desc, origData, mimeType);

    Size box{ desc.width, desc.height };

    DataProviderPtr newData =
            renderer->aspectFit(*original, origData, box, type);

    if (desc.save) {
        ImageVersion newVersion;
        newVersion.extension = type;
        newVersion.width = desc.width;
        newVersion.height = desc.height;
        newVersion.fileName = original->fileName;
        newVersion.mimeType = Utils::mimeType(type);
        newVersion.sizeBytes = newData->size();

        imageStore->addVersion(*image,
                newVersion,
                (char*)newData->bytes());
    }

    return makeResult(desc, newData, mimeType);
}

//------------------------------------------------------------------------------

ImageController::VersionRequest::VersionRequest(
        const Image &image,
        const ImageDescriptor &desc) {
    auto original = std::find_if(image.versions.begin(),
            image.versions.end(),
            [](const ImageVersion& ver) -> bool {
                return ver.original;
            });

    type = desc.type.empty() ? original->extension : desc.type;

    if (!desc.original) {
        auto exact = std::find_if(image.versions.begin(),
                image.versions.end(),
                [&](const ImageVersion& ver) -> bool {
                    return (ver.width == desc.width)
                            && (ver.height == desc.height)
                            && (ver.extension == type);
                });

        if (exact != image.versions.end()) {
            isExact = true;
            src = *exact;
            return;
        }
    }

    if (original == image.versions.end())
        throw std::runtime_error{
                "Original image not found for key " + desc.imageKey};

    src = *original;
    isExact = desc.original;
}

} /* namespace ImageServer */

