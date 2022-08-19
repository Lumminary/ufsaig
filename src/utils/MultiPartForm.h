/*
 * MultiPartForm.h
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_MULTIPARTFORM_H_
#define UTILS_MULTIPARTFORM_H_

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <httpserver.hpp>
#include <algorithm>
#include <iostream>

namespace ImageServer {

namespace Utils {

struct MultiPartForm {

    struct File {
        std::string name;
        std::vector<char> data;
        std::string md5;
    };

    using Arg = std::variant<File, std::string>;
    using Args = std::unordered_map<std::string, Arg>;

    Args args;

    void parse(const httpserver::http_request& request);

    template<typename I1, typename I2>
    void parse(I1 contentStart, I1 contentEnd,
            I2 boundaryStart, I2 boundaryEnd);

    template<typename Iter>
    void parseArg(Iter begin, Iter end);
};

//------------------------------------------------------------------------------

template<typename I1, typename I2>
void MultiPartForm::parse(I1 contentStart, I1 contentEnd,
        I2 boundaryStart, I2 boundaryEnd) {
    I1 argEnd = std::search(contentStart, contentEnd,
                                        boundaryStart, boundaryEnd);

    parseArg(contentStart, argEnd);

    I1 rest = argEnd + std::distance(boundaryStart, boundaryEnd);
    if (rest < contentEnd) {
        parse(rest, contentEnd, boundaryStart, boundaryEnd);
    }
}

//------------------------------------------------------------------------------

template<typename Iter>
void MultiPartForm::parseArg(Iter begin, Iter end) {
    static const char *entityName = "name=\"";
    static const char *quotationMark = "\"";
    static const char *fileName = "filename=\"";
    static const char *CRLF = "\r\n\r\n";

    auto nameStart = std::search(begin, end, entityName, entityName + 6);
    if (nameStart == end)
        return;

    nameStart += 6;
    auto nameEnd = std::search(nameStart,
            end, quotationMark, quotationMark + 1);
    if (nameEnd == end)
        return;

    std::string name{nameStart, nameEnd};

    auto valueEnd = end;
    if (((valueEnd - begin) >= 4)
            && (*(valueEnd - 4) == '\r')
            && (*(valueEnd - 3) == '\n')
            && (*(valueEnd - 2) == '-')
            && (*(valueEnd - 1) == '-'))
        valueEnd -= 4;

    auto fileNameStart = std::search(nameEnd, end, fileName, fileName + 10);
    if (fileNameStart == end)
    {
        auto valueStart = std::search(nameEnd, end, CRLF, CRLF + 4);
        if (valueStart == end)
            return;

        valueStart += 4;
        args.emplace(name, std::string{valueStart, valueEnd});
        return;
    }
    else
    {
        fileNameStart += 10;
        auto fileNameEnd = std::search(fileNameStart, end,
                                        quotationMark, quotationMark + 1);
        if (fileNameEnd == end)
            return;

        File file;
        file.name = std::string{ fileNameStart, fileNameEnd };

        auto dataStart = std::search(fileNameEnd, end, CRLF, CRLF + 4);
        if (dataStart == end)
            return;

        dataStart += 4;
        file.data.assign(dataStart, valueEnd);
        args.emplace(name, std::move(file));
    }
}



}

} /* namespace ImageServer */

#endif /* UTILS_MULTIPARTFORM_H_ */
