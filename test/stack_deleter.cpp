
#include <cassert>
#include <iostream>

#include "utility.cc/stack_deleter.h"


// 示例类，用于测试析构函数调用
class TestResource
{
  private:
    std::string name_;
    static int  instance_count_;

  public:
    TestResource(const std::string &name) : name_(name)
    {
        instance_count_++;
        std::cout << "创建资源: " << name_ << " (总数: " << instance_count_ << ")" << std::endl;
    }

    ~TestResource()
    {
        instance_count_--;
        std::cout << "销毁资源: " << name_ << " (剩余: " << instance_count_ << ")" << std::endl;
    }

    const std::string &getName() const { return name_; }
    static int         getInstanceCount() { return instance_count_; }
};

int TestResource::instance_count_ = 0;

// 模拟复杂的资源依赖关系
class DatabaseConnection
{
  public:
    DatabaseConnection()
    {
        std::cout << "建立数据库连接" << std::endl;
    }

    ~DatabaseConnection()
    {
        std::cout << "关闭数据库连接" << std::endl;
    }
};

class NetworkManager
{
  public:
    NetworkManager()
    {
        std::cout << "初始化网络管理器" << std::endl;
    }

    ~NetworkManager()
    {
        std::cout << "关闭网络管理器" << std::endl;
    }
};

void demonstrateStackDeleter()
{
    std::cout << "\n=== StackDeleter 演示开始 ===" << std::endl;

    {
        ut::StackDeleter deleter;

        // 创建资源并注册到删除器
        auto *db = new DatabaseConnection();
        deleter.push("Database", db);

        auto *network = new NetworkManager();
        deleter.push("Network", network);

        auto *resource1 = new TestResource("Resource1");
        deleter.push("TestResource1", resource1);

        auto *resource2 = new TestResource("Resource2");
        deleter.push(resource2); // 使用自动命名

        // 添加自定义清理逻辑
        deleter.push_custom("Custom Cleanup", []() {
            std::cout << "--->执行自定义清理逻辑" << std::endl;
        });

        std::cout << "\n当前管理的资源数量: " << deleter.size() << std::endl;
        std::cout << "当前TestResource实例数: " << TestResource::getInstanceCount() << std::endl;

        std::cout << "\n即将离开作用域, 开始自动清理..." << std::endl;
    } // deleter 在这里析构，按照LIFO顺序清理所有资源

    std::cout << "\n清理完成,剩余TestResource实例数: " << TestResource::getInstanceCount() << std::endl;
    std::cout << "=== StackDeleter 演示结束 ===\n"
              << std::endl;
}

void demonstrateManualClear()
{
    std::cout << "=== 手动清理演示开始 ===" << std::endl;

    ut::StackDeleter deleter;

    auto *resource1 = new TestResource("Manual1");
    auto *resource2 = new TestResource("Manual2");

    deleter.push("Manual Resource 1", resource1);
    deleter.push("Manual Resource 2", resource2);

    std::cout << "添加资源后，实例数: " << TestResource::getInstanceCount() << std::endl;

    // 手动清理
    std::cout << "执行手动清理..." << std::endl;
    deleter.clear();

    std::cout << "手动清理完成，实例数: " << TestResource::getInstanceCount() << std::endl;
    std::cout << "=== 手动清理演示结束 ===\n"
              << std::endl;
}

int main()
{
    std::cout << "栈式指针管理器 (StackDeleter) 测试程序\n"
              << std::endl;

    // 演示自动清理
    demonstrateStackDeleter();

    // 演示手动清理
    demonstrateManualClear();

    // 验证所有资源都被正确清理
    assert(TestResource::getInstanceCount() == 0);
    std::cout << "✅ 所有测试通过！所有资源都被正确清理。" << std::endl;

    return 0;
}
