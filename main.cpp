#include <lcmware/topic.hpp>
#include <custom_package/PointArray.hpp>
#include <lcmware/types/core/StringMsg.hpp>
#include <iostream>

using namespace lcmware;


/*
LCMware CPP Template Example
Date: 2025-7-3
Author: Nathan George

Description:
This is a simple example of how to use LCMWare to publish and subscribe to messages.
You can replace this with your own message types and logic.

You can also import actions and services and other standard types:

#include <lcmware/action.hpp>
#include <lcmware/service.hpp>
#include <lcmware/types/core/...>

Your custom types are directly available under the package name you give them.
There is a default custom package which you can replace with your own giving the file or package any name you like.
Custom messages are built automatically when you build your project assuming you have the lcm-gen package installed.
*/


void callback(const core::StringMsg& msg)
{
    std::cout << "Received message: " << msg.value << std::endl;
}

int main(int argc, char** argv)
{
    TopicPublisher<custom_package::PointArray> points_pub("/points");
    TopicPublisher<core::StringMsg> talker_pub("/talker");
    TopicSubscriber<core::StringMsg> talker_sub("/talker", callback);

    while (true)
    {
        custom_package::PointArray msg;
        msg.points.resize(10);
        
        for (int i = 0; i < 10; ++i)
        {
            msg.points[i].x = i * 0.1f;
            msg.points[i].y = i * 0.2f;
        }

        points_pub.publish(msg);
        talker_pub.publish({value: "Hello, LCM!"});
    }
}