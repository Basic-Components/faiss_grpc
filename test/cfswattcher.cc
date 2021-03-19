#include <iostream>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <libfswatch/c/cevent.h>
#include <libfswatch/c/libfswatch.h>

void callback(fsw_cevent const *const events,
                 const unsigned int event_num,
                 void *data)
{
  std::cout << "get events num " << event_num << std::endl;
  for (auto i = 0; i < event_num; i++){
    auto event = events[i];
    std::string path(event.path);
    auto evt_time = event.evt_time;
    auto flags_num = event.flags_num;
    for (auto j = 0; i < flags_num; i++){
      auto f = event.flags[i];
      std::string eventname (fsw_get_event_flag_name(f));
      std::cout << "get event" << eventname << "@" << path << " at "<< evt_time << std::endl;
    }
  }
  std::cout << "get event ok" << std::endl;
}

int main()
{
    std::vector<std::string> paths = {"./"};
    // Create the default platform monitor
    FSW_STATUS ret = fsw_init_library();
    const FSW_HANDLE handle = fsw_init_session(system_default_monitor_type);
    for (auto p : paths ){
        fsw_add_path(handle, p.c_str());
    }
    fsw_add_event_type_filter(handle,{Updated});
    fsw_set_callback(handle, callback,nullptr);
    fsw_start_monitor(handle);
    std::thread t (fsw_start_monitor,handle);
    t.detach();
    std::cout << "start watching fs" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    fsw_stop_monitor(handle);
}