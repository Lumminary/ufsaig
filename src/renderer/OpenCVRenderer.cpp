/*
 * OpenCVRenderer.cpp
 *
 *  Created on: May 14, 2021
 *      Author: Sergii Kutnii
 */

#include "OpenCVRenderer.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "utils/ArrayWrapper.h"
#include <opencv2/gapi/render/render_types.hpp>
#include <opencv2/gapi/render/render.hpp>
#include "utils/convert.h"
#include <iostream>

namespace ImageServer {

static const double colorMax = 255.0;

static cv::Scalar parseHex(const std::string& hexColor) {
    try {
        if (hexColor.size() >= 8) {
            double red = stoi(hexColor.substr(0, 2), 0, 16);
            double green = stoi(hexColor.substr(2, 2), 0, 16);
            double blue = stoi(hexColor.substr(4, 2), 0, 16);
            double alpha = stoi(hexColor.substr(6), 0, 16);

            return cv::Scalar{ red / colorMax,
                green / colorMax, blue / colorMax, alpha / colorMax };
        } else if (hexColor.size() >= 6) {
            double red = stoi(hexColor.substr(0, 2), 0, 16);
            double green = stoi(hexColor.substr(2, 2), 0, 16);
            double blue = stoi(hexColor.substr(4, 2), 0, 16);

            return cv::Scalar{ blue,
                green, red, colorMax };
        }
    } catch(...) {

    }

    return cv::Scalar{ 0.0, 0.0, 0.0, 0.0 };
}

//------------------------------------------------------------------------------

static cv::Scalar parseRGB(std::string src) {
    try {
        size_t end = src.find_last_of(")");
        if (end != std::string::npos) {
            src.replace(end, 1, "");
        }

        std::vector<std::string> values = Utils::split(src, ',');

        if (values.size() >= 3) {
            double red = std::stoi(values[0]);
            double green = std::stoi(values[1]);
            double blue = std::stoi(values[2]);
            return cv::Scalar{
                blue, green, red, colorMax };
        }
    } catch(...) {
    }

    return cv::Scalar{ 0.0, 0.0, 0.0, 0.0 };
}

//------------------------------------------------------------------------------

static cv::Scalar parseRGBA(std::string src) {
    try {
        size_t end = src.find_last_of(")");
        if (end != std::string::npos) {
            src.replace(end, 1, "");
        }

        std::vector<std::string> values = Utils::split(src, ',');

        if (values.size() >= 4) {
            double red = std::stoi(values[0]);
            double green = std::stoi(values[1]);
            double blue = std::stoi(values[2]);
            double alpha = std::stoi(values[3]);
            return cv::Scalar{
                blue, green,
                red, alpha };
        }
    } catch(...) {
    }

    return cv::Scalar{ 0.0, 0.0, 0.0, 0.0 };
}

//------------------------------------------------------------------------------

static cv::Scalar parseColor(const std::string& color) {
    if (color[0] == '#')
        return parseHex(color.substr(1));
    else if (color.substr(0, 4) == "rgb(")
        return parseRGB(color.substr(4));
    else if (color.substr(0, 5) == "rgba(")
        return parseRGBA(color.substr(5));

    return cv::Scalar{ 0.0, 0.0, 0.0, 0.0 };
}

//------------------------------------------------------------------------------

DataProviderPtr OpenCVRenderer::aspectFit(const Size &origSize,
        const DataProviderPtr &origData,
        const Size &box,
        const std::string &outFormat) {

    auto transform = [&]() -> DataProviderPtr {
        cv::_InputArray in{ (char*)origData->bytes(), (int)origData->size() };
        cv::Mat src = cv::imdecode(in, cv::IMREAD_COLOR);

        Size newSize = origSize.aspectFit(box);
        cv::Mat dest;

        cv::resize(src,
                dest,
                cv::Size{ (int)newSize.width, (int)newSize.height });

        std::shared_ptr<Utils::ArrayWrapper<uchar>> buffer =
                std::make_shared<Utils::ArrayWrapper<uchar>>();
        cv::imencode(encoderExtension(outFormat), dest, buffer->data);

        return buffer;
    };

    return m_executor.sync<DataProviderPtr, decltype(transform)>(transform);
}

//------------------------------------------------------------------------------

DataProviderPtr OpenCVRenderer::renderSprite(const Sprite &sprite,
        const std::string &background,
        const std::string &outFormat,
        double scale) {
    auto transform = [&]() -> DataProviderPtr {
        int spriteXMax =
                (int) (scale * (sprite.bounds.origin.x
                        + sprite.bounds.size.width));
        int spriteYMax =
                (int) (scale * (sprite.bounds.origin.y
                        + sprite.bounds.size.height));

        cv::Mat canvas;
        canvas.create( spriteYMax, spriteXMax, CV_8UC3 );
        canvas = parseColor(background);

        cv::gapi::wip::draw::Prims drawables;
        drawables.reserve(sprite.tiles.size());
        for (const Tile& tile: sprite.tiles) {
            cv::_InputArray in{
                (char*)tile.data->bytes(), (int)tile.data->size() };
            cv::Mat img = cv::imdecode(in, cv::IMREAD_COLOR);
            cv::Mat alpha{
                img.cols, img.rows, CV_32FC1, cv::Scalar{ 1.0, 1.0, 1.0, 1.0 }};
            int x = (int)(scale * tile.bounds.origin.x);
            int y = spriteYMax
                    - (int)(scale * (tile.bounds.origin.y
                            + tile.bounds.size.height));
            cv::Point origin(x, y);
            drawables.emplace_back(
                    cv::gapi::wip::draw::Image{ origin, img, alpha });
        }

        cv::gapi::wip::draw::render(canvas, drawables);

        std::shared_ptr<Utils::ArrayWrapper<uchar>> buffer =
                std::make_shared<Utils::ArrayWrapper<uchar>>();
        cv::imencode(encoderExtension(outFormat), canvas, buffer->data);

        return buffer;
    };

    return m_executor.sync<DataProviderPtr, decltype(transform)>(transform);
}

//------------------------------------------------------------------------------

std::string OpenCVRenderer::encoderExtension(const std::string &format) {
    if (format.empty())
        return format;

    std::string fmt = Utils::toLower(format);
    if (fmt[0] == '.') {
        return fmt;
    }

    return "." + fmt;
}

} /* namespace ImageServer */
