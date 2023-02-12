#ifndef CONSTEXPR_MAP_HPP
#define CONSTEXPR_MAP_HPP

#include<algorithm>
#include<cstddef>
#include<stdexcept>

template<typename K, typename V, std::size_t size>
class ConstexprMap {
  public:
    std::array<std::pair<K, V>, size> data;
    constexpr V operator[](const K& key) const {
        const auto element = std::find_if(std::begin(data), std::end(data),
                                          [&key] (const auto& value) {
                                            return value.first == key;
                                          });
        if (element != std::end(data)) {
            return element->second;
        } else {
            throw std::range_error("Not found.");
        }
    }
};

#endif /* CONSTEXPR_MAP_HPP */