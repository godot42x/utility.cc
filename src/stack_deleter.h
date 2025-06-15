#pragma once

#include <functional>
#include <string>
#include <vector>

// 是否需要一份整个声明周期都存在的内存，，来维护一个基于栈的清理操作？？
// 花一小部分时间来手动管理资源的生命周期，是不是更加合理？？

namespace ut
{

/**
 * @brief 自动按栈顺序清理持有指针的管理器
 *
 * 这个类解决了类型擦除时析构函数不被正确调用的问题。
 * 它使用 std::function 来存储类型化的删除器，确保正确的析构函数被调用。
 * 
 * 启用调试日志：
 * - 编译时：使用 xmake config --utility_debug=true 来启用调试日志
 * - 这将定义 UTILITY_DEBUG_ENABLED 宏，从而输出清理和添加操作的日志
 */
class StackDeleter
{
  public:
    struct DeleterItem
    {
        std::string           name;
        std::function<void()> deleter;

        DeleterItem(const std::string &n, std::function<void()> d)
            : name(n), deleter(std::move(d)) {}
    };

  private:
    std::vector<DeleterItem> _items;

  public:
    /**
     * @brief 添加一个需要管理的指针
     * @tparam T 指针指向的类型
     * @param name 用于调试的名称
     * @param ptr 要管理的原始指针
     */    template <typename T>
    void push(const std::string &name, T *ptr)
    {
        if (ptr) {
#ifdef UTILITY_DEBUG_ENABLED
            fprintf(stderr, "[StackDeleter] Adding %s for management\n", name.c_str());
#endif
            _items.emplace_back(name, [ptr, name]() {
                delete ptr;
            });
        }
    }

    /**
     * @brief 添加一个需要管理的指针（使用类型名作为名称）
     * @tparam T 指针指向的类型
     * @param ptr 要管理的原始指针
     */
    template <typename T>
    void push(T *ptr)
    {
        if (ptr) {
            push(typeid(T).name(), ptr);
        }
    }

    /**
     * @brief 添加一个自定义的清理函数
     * @param name 用于调试的名称
     * @param deleter 清理函数
     */    void push_custom(const std::string &name, std::function<void()> deleter)
    {
#ifdef UTILITY_DEBUG_ENABLED
        fprintf(stderr, "[StackDeleter] Adding custom deleter %s for management\n", name.c_str());
#endif
        _items.emplace_back(name, std::move(deleter));
    }

    /**
     * @brief 手动清理所有管理的资源
     */
    void clear()
    {        // 按相反顺序删除（LIFO - 后进先出）
        for (auto it = _items.rbegin(); it != _items.rend(); ++it) {
            try {
#ifdef UTILITY_DEBUG_ENABLED
                fprintf(stderr, "[StackDeleter] Deleting %s\n", it->name.c_str());
#endif
                it->deleter();
            }
            catch (...) {
                // 在析构过程中不应该抛出异常
                // 可以在这里记录错误日志
#ifdef UTILITY_DEBUG_ENABLED
                fprintf(stderr, "[StackDeleter] Error during deletion of %s\n", it->name.c_str());
#else
                fprintf(stderr, "[StackDeleter] Error during deletion\n");
#endif
            }
        }
        _items.clear();
    }

    /**
     * @brief 获取当前管理的项目数量
     */
    size_t size() const { return _items.size(); }

    /**
     * @brief 检查是否为空
     */
    bool empty() const { return _items.empty(); }

    /**
     * @brief 析构函数 - 自动清理所有资源
     */
    ~StackDeleter()
    {
        clear();
    }

    // 禁止拷贝，但允许移动
    StackDeleter()                                = default;
    StackDeleter(const StackDeleter &)            = delete;
    StackDeleter &operator=(const StackDeleter &) = delete;
    StackDeleter(StackDeleter &&)                 = default;
    StackDeleter &operator=(StackDeleter &&)      = default;
};

} // namespace ut
