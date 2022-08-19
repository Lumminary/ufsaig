/*
 * injectable.hpp
 *
 *  Created on: Feb 1, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_INJECTABLE_HPP_
#define UTILS_INJECTABLE_HPP_

#include <functional>

namespace ImageServer {

template<typename Target>
class Injected {
public:
    using Getter = std::function<Target* ()>;

    Target* operator->() const {
        return m_getter();
    }

    Target& operator*() const {
        return *m_getter();
    }

    template<typename GetBlock>
    void inject(GetBlock block) {
        m_getter = Getter(block);
    }
private:
    Getter m_getter;
};

}



#endif /* UTILS_INJECTABLE_HPP_ */
