/*
 * Base64Images.cpp
 *
 *  Created on: Apr 23, 2021
 *      Author: Sergii Kutnii
 */

#include "Base64Images.h"
#include "utils/ImageResponse.h"
#include "utils/base64.h"
#include <iostream>
#include <sstream>

namespace ImageServer {

using namespace httpserver;

const std::shared_ptr<httpserver::http_response>
Base64Images::render(const httpserver::http_request &req) {
    try {
        const std::vector<char>& data = req.get_content();
        const char *dataStart = data.data();
        const char *dataEnd = dataStart + data.size();

        Json::Value descriptorsJson;
        Json::CharReaderBuilder readerBuilder;
        std::string errs;

        std::unique_ptr<Json::CharReader>
        reader{ readerBuilder.newCharReader() };

        reader->parse(dataStart,
                dataEnd, &descriptorsJson, &errs);

        size_t imageCount = descriptorsJson.size();

        std::vector<ImageDescriptor> descriptors;
        descriptors.reserve(imageCount);
        for (size_t i = 0; i < imageCount; ++i) {
            Json::Value& descJson = descriptorsJson[(Json::ArrayIndex)i];
            descriptors.emplace_back();
            ImageDescriptor& desc = descriptors[i];
            desc.imageKey = descJson["key"].asString();
            desc.original = descJson.isMember("original")
                                    && descJson["original"].asBool();
            desc.width = descJson["width"].asUInt();
            desc.height = descJson["height"].asUInt();
            desc.type = descJson["type"].asString();

            desc.save = descJson.isMember("save") && descJson["save"].asBool();
            desc.base64 = true;
        }

        DataMap images =
                imageController->getImagesData(descriptors);

        Json::Value output;
        for (auto& entry : images) {
            DataProviderPtr data = entry.second;
            std::string base64;;
            base64.assign((char*)data->bytes(), data->size());

            output[entry.first] = Json::Value{ base64 };
        }

        Json::StreamWriterBuilder writerBuilder;
        std::unique_ptr<Json::StreamWriter> writer(
           writerBuilder.newStreamWriter());


        std::stringstream outStream;
        writer->write(output, &outStream);
        auto response = std::make_shared<string_response>(
                             outStream.str(),
                             http::http_utils::http_ok,
                             "application/json");

        response->with_header("Access-Control-Allow-Origin", "*");
        return response;
    } catch(std::exception& e) {
        return std::make_shared<string_response>(
                e.what(), 400, "text/plain");
    }
}

} /* namespace ImageServer */
