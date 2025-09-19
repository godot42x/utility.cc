#pragma once


#include <iostream>
#include <iterator>
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

// A custom iterator that moves through the vector, updating the index.
template <typename T>
class enumerating_iterator
{
  public:
    // Required to make the iterator usable.
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = indexed_value<T>;
    using pointer           = value_type *;
    using reference         = value_type;

    enumerating_iterator(typename std::vector<T>::iterator it, size_t index)
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
    typename std::vector<T>::iterator m_it;
    size_t                            m_index;
};

template <typename Value, template <typename> typename Container>
    requires requires(Container<Value> c) {
        typename Container<Value>::iterator;
        typename Container<Value>::size_type;
    }
struct enumerate_range
{
    Container<Value> &m_data; // requires forward iterator

    enumerate_range(Container<Value> &data) : m_data(data) {}

    enumerating_iterator<Value> begin() { return enumerating_iterator<Value>(m_data.begin(), 0); }
    enumerating_iterator<Value> end() { return enumerating_iterator<Value>(m_data.end(), m_data.size()); }
};


struct enumerate_adaptor
{
    template <typename Range>
    auto operator()(Range &&range) const
    {
        return enumerate_range(std::forward<Range>(range));
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
    }
    ut::enumerate(fruits);


    for (auto &&[index, fruit] : ut::enumerate(fruits)) {
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
