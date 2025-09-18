#pragma once


#include <iostream>
#include <iterator>
#include <vector>


namespace ut
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

// A wrapper struct to create the "range".
template <typename T>
class enumerate_range
{
  public:
    enumerate_range(std::vector<T> &data) : m_data(data) {}

    // Methods that return the custom iterators.
    ut::enumerating_iterator<T> begin() { return enumerating_iterator<T>(m_data.begin(), 0); }
    ut::enumerating_iterator<T> end() { return enumerating_iterator<T>(m_data.end(), m_data.size()); }

  private:
    std::vector<T> &m_data;
};



// A helper function to make it easy to create the range.
template <typename T>
enumerate_range<T> enumerate(std::vector<T> &data)
{
    return enumerate_range<T>(data);
}



namespace test
{
// Example usage
inline int test()
{
    std::vector<std::string> fruits = {"apple", "banana", "cherry"};

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
