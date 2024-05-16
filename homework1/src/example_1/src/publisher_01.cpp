#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int32.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class TopicPublisher01 : public rclcpp::Node
{
public:
    // 构造函数,有一个参数为节点名称
    TopicPublisher01(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        // 创建发布者
        pub_num = this->create_publisher<std_msgs::msg::UInt32>("num", 10);
        // 使用定时器来控制发布频率
        // timer_1 = create_wall_timer(2000ms, std::bind(&TopicPublisher01::timer_callback, this));

        // sizen
        timer_1 = create_wall_timer(2000ms, std::bind(&TopicPublisher01::timer_callback, this),callback_group1_);

        callback_group2_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
        
        auto sub_options = rclcpp::SubscriptionOptions();
        sub_options.callback_group = callback_group2_;

        sub_num = this->create_subscription<std_msgs::msg::UInt32>(
            "result",
            10,
            std::bind(&TopicPublisher01::num_callback, this, std::placeholders::_1),
            sub_options);
    }

private:
    // 声明一个订阅者
    rclcpp::Subscription<std_msgs::msg::UInt32>::SharedPtr sub_num;
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_num;
    rclcpp::TimerBase::SharedPtr timer_1;
    rclcpp::CallbackGroup::SharedPtr callback_group2_;

    // sizen
    rclcpp::CallbackGroup::SharedPtr callback_group1_; // for publisher timer

    uint32_t number = 1;
    
    void timer_callback()
    {
        if (number <= 100)
        {
            std_msgs::msg::UInt32 msg;
            msg.data = number;
            pub_num->publish(msg);
            RCLCPP_INFO(this->get_logger(), "发布数字：%d", msg.data);
            ++number;
        }
    }
    // 创建回调函数
    void num_callback(const std_msgs::msg::UInt32::SharedPtr num)
    {

        RCLCPP_INFO(this->get_logger(), "得到结果：%d", num->data);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TopicPublisher01>("topic_publisher_01");
    // rclcpp::executors::SingleThreadedExecutor executor;
    // sizen 
    rclcpp::executors::MultiThreadedExecutor executor; //用single thread还是单线程运行

    executor.add_node(node);
    executor.spin();
    rclcpp::shutdown();
    return 0;
}