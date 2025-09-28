#pragma once

#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>


namespace ut
{

namespace detail
{
// A struct to hold the index and a reference to the element.
template <typename T>
struct indexed_value
{
    size_t index;
    T     &value;
};

// A custom iterator that moves through the container, updating the index.
template <typename Iterator>
class enumerating_iterator
{
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = indexed_value<typename std::iterator_traits<Iterator>::value_type>;
    using pointer           = value_type *;
    using reference         = value_type;

    enumerating_iterator(Iterator it, size_t index)
        : m_it(it), m_index(index) {}

    // Dereference operator.
    reference operator*() { return {m_index, *m_it}; }

    // Prefix increment.
    enumerating_iterator &operator++()
    {
        ++m_it;
        ++m_index;
        return *this;
    }

    // Postfix increment.
    enumerating_iterator operator++(int)
    {
        enumerating_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    // Comparison operators.
    friend bool operator==(const enumerating_iterator &a, const enumerating_iterator &b)
    {
        return a.m_it == b.m_it;
    }
    friend bool operator!=(const enumerating_iterator &a, const enumerating_iterator &b)
    {
        return a.m_it != b.m_it;
    }

  private:
    Iterator m_it;
    size_t   m_index;
};

template <typename Range>
struct enumerate_range
{
    Range &m_range;

    enumerate_range(Range &range) : m_range(range) {}

    auto begin()
    {
        return enumerating_iterator(m_range.begin(), 0);
    }

    auto end()
    {
        return enumerating_iterator(m_range.end(), std::distance(m_range.begin(), m_range.end()));
    }
};

struct enumerate_adaptor
{
    template <typename Range>
    auto operator()(Range &&range) const
    {
        return enumerate_range<std::remove_reference_t<Range>>(std::forward<Range>(range));
    }
};

} // namespace detail

inline constexpr detail::enumerate_adaptor enumerate;

template <typename Range>
auto operator|(Range &&range, const detail::enumerate_adaptor &adapter)
{
    return adapter(std::forward<Range>(range));
}

namespace test
{
// Example usage
inline int test()
{
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};

    for (auto &&[index, fruit] : fruits | enumerate) {
        std::cout << "Index: " << index << ", Value: " << fruit << std::endl;
    }

    for (auto &&[index, fruit] : enumerate(fruits)) {
        std::cout << "Index: " << index << ", Value: " << fruit << std::endl;
    }

    // You can also use it to modify elements.
    for (auto &&[index, fruit] : enumerate(fruits)) {
        fruit += " pie";
    }

    for (const auto &fruit : fruits) {
        std::cout << fruit << std::endl;
    }

    return 0;
}
} // namespace test

} // namespace ut
