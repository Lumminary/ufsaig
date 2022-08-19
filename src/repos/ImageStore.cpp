/*
 * ImageStore.cpp
 *
 *  Created on: Jan 28, 2021
 *      Author: Sergii Kutnii
 */

#include "ImageStore.h"
#include <uuid/uuid.h>
#include <filesystem>
#include "utils/hex.h"
#include <ctime>
#include <json/json.h>
#include <sstream>

namespace ImageServer {

using namespace Couchbase;

//------------------------------------------------------------------------------

ImagePtr ImageStore::newImage(const std::string &name, const Params &params,
        const char *data, size_t dataLength) {
    auto iWidth = params.find("width");
    if (iWidth == params.end())
        throw ValidationError{ "Width missing." };

    auto iHeight = params.find("height");
    if (iHeight == params.end())
        throw ValidationError{ "Height missing." };

    auto iMD5 = params.find("md5");
    if (iMD5 == params.end())
        throw ValidationError{ "MD5 hash missing." };

    auto iMime = params.find("mime_type");
    if (iMime == params.end())
        throw ValidationError{ "Mime type missing." };

    std::string extension = std::filesystem::path{ name }.extension().string();
    if ((extension.size() > 0) && (extension[0] == '.'))
        extension.replace(0, 1, "");

    uuid_t uuid;
    uuid_generate(uuid);

    std::string mainKey = "img_" + Utils::hexify(uuid);

    ImageVersion instance;
    instance.fileName = name;
    instance.sizeBytes = dataLength;
    instance.extension = extension;
    instance.width = stoi(iWidth->second);
    instance.height = stoi(iHeight->second);
    instance.original = true;
    instance.md5 = iMD5->second;
    instance.mimeType = iMime->second;

    ImagePtr image = std::make_shared<Image>();
    image->key = mainKey;
    image->createdTimestamp = std::time(nullptr);
    image->updatedTimestamp = image->createdTimestamp;
    image->addVersion(instance);

    saveImageInstance(*image, instance, data);

    return image;
}

//------------------------------------------------------------------------------

ImagePtr ImageStore::findImage(const std::string &key) {
    EntryPtr entry = metaConnector->findEntry(key);
    return std::make_shared<Image>(entry->json());
}

//------------------------------------------------------------------------------

void ImageStore::addVersion(const Image &image,
        const ImageVersion &versionTemplate,
        const char *data) {
    Image copy{ image };
    ImageVersion& version = copy.addVersion(versionTemplate);
    saveImageInstance(image, version, data);
}

//------------------------------------------------------------------------------

ImageStore::Images ImageStore::findImages(
        const std::vector<std::string> &keys) {
    GetResults metaEntries = metaConnector->findEntries(keys);
    Images images;
    images.reserve(keys.size());

    auto metaEnd = metaEntries.end();
    for (const std::string& key : keys) {
        auto it = metaEntries.find(key);
        if (it != metaEnd)
            images[key] = std::make_shared<Image>(it->second->json());
    }

    return images;
}

//------------------------------------------------------------------------------

void ImageStore::saveImageInstance(const Image &image,
        const ImageVersion &instance, const char *data) {
    std::shared_ptr<Write> saveBlob =
            std::make_shared<Write>(instance.key);
    saveBlob->setValue(data, instance.sizeBytes);
    dataConnector->schedule(saveBlob);
    saveBlob->result().get();

    Image copy{ image };
    copy.versions.push_back(instance);

    Json::StreamWriterBuilder builder;
    std::string meta = Json::writeString(builder, copy.json());

    std::shared_ptr<Write> saveMeta =
            std::make_shared<Write>(copy.key);
    saveMeta->setValue(meta.c_str(), meta.size());
    metaConnector->schedule(saveMeta);
    saveMeta->result().get();
}

} /* namespace ImageServer */
