#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <type_traits>
#include <vector>
#include <array>

/*It may possible be deleted...*/
#include "./metafunctions.h"

const std::string channel_name_form = "/dev/i2c-";

class I2CController
{
protected:
    std::string channel;
    int port_handle;
    std::uint8_t slave_address;
    enum class Status
    {
        NOT_OPENED,
        IDLE,
        BUSY,
    };
    Status status;
public:
    I2CController(const unsigned int& _channel_num);
    virtual ~I2CController();
    int open_port();
    int close_port();
    void wait_for_being_idle();
    int set_slave_address(const std::uint8_t& _slave_address);
    int write_1_byte(const uint8_t& _a_byte);

    template <typename Pointer, std::enable_if_t<   std::is_pointer_v<Pointer>  , bool> = false>
    int write_bytes(Pointer _a_pointer, size_t _len);
    template <typename Container, std::enable_if_t<has_size<Container>::value, bool> = false>
    int write_bytes(const Container& _a_data_container);
};


template <typename Pointer, std::enable_if_t<   std::is_pointer_v<Pointer>  , bool> = false>
int I2CController::write_bytes(Pointer _a_pointer, size_t _len)
{
    wait_for_being_idle();
    status = Status::BUSY;
    int result = write(port_handle, _a_pointer, _len);
    status = Status::IDLE;
    if(result <0)
    {
        std::cout<<"Write failed...\n";
    }
    return result;
}

template <typename Container, std::enable_if_t<has_size<Container>::value, bool> = false>
int I2CController::write_bytes(const Container& _a_data_container)
{
    wait_for_being_idle();
    status = Status::BUSY;
    int result = write(port_handle, &(_a_data_container[0]), _a_data_container.size());
    status = Status::IDLE;
    if(result <0)
    {
        std::cout<<"Write failed...\n";
    }
    return result;
}