#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <libfswatch/c++/monitor_factory.hpp>
#include <libfswatch/c++/monitor.hpp>
#include <libfswatch/c++/event.hpp>
#include <libfswatch/c/cevent.h>
using fsw::event;
using fsw::monitor_factory;
using fsw::monitor;

void callback(const std::vector<event> &evts, void *ctx)
{
    for (auto evt : evts)
    {
        auto flags = evt.get_flags();
        if (flags.empty()){
                std::cout << "get event empty" << std::endl;
                continue;
        }
        for (auto flag :flags)
        {
            std::cout << "get event" << event::get_event_flag_name(flag) << "@" << evt.get_path() << " at "<< evt.get_time() << std::endl;
        }
        std::cout << "get event ok" << std::endl;
        
    }
}

int main()
{
std:
    std::vector<std::string> paths = {"./"};
    // Create the default platform monitor
    monitor *active_monitor = monitor_factory::create_monitor(
        fsw_monitor_type::system_default_monitor_type,
        paths,
        callback);

    // Configure the monitor
    active_monitor->add_event_type_filter({Updated});
    active_monitor->set_watch_access(false);

    // Start the monitor
    active_monitor->start();
}