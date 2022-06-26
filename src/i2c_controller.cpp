#include "./i2c_controller.h"


I2CController::I2CController(const unsigned int& _channel_num)
    :status(Status::NOT_OPENED)
{
    std::stringstream ss;
    ss << channel_name_form << _channel_num;
    channel = ss.str();
    open_port();
}
I2CController::~I2CController()
{
    close_port();
}
int I2CController::open_port()
{
    port_handle = open(channel.c_str(),O_RDWR);
    if (port_handle < 0)
    {
        std::cout<<"Failed to open\n";
        return -1;
    }
    status = Status::IDLE;
    std::cout<<"Successed to open the port\n";
    return 1;   
}

int I2CController::close_port()
{
    if (status == Status::NOT_OPENED)
    {
        std::cout << "No need to close\n";
        return -1;
    }
    wait_for_being_idle();
    std::cout << "Closing the port...\n";
    close(port_handle);
    status = Status::NOT_OPENED;
    return 1;
}

void I2CController::wait_for_being_idle()
{
    if (status != Status::BUSY)
    {
        return;
    }
    std::cout << "Wait for work be finished\n";
    while (status == Status::BUSY)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

int I2CController::set_slave_address(const std::uint8_t& _slave_address)
{
    int rc;
    rc = ioctl(port_handle, I2C_SLAVE, _slave_address);
    if(rc < 0)
    {
        std::cout<<"Failed to set slave_address\n";
        std::cout<<"Recommended: i2cdetect\n";
        return -1;
    }
    std::cout<<"Successed to slave address\n";
    return 1;   
}

int I2CController::write_1_byte(const uint8_t& _a_byte)
{
    if(status == Status::NOT_OPENED)
    {
        std::cout<<"Port is not opened\n";
        return -1;
    }
    int result;
    wait_for_being_idle();
    status = Status::BUSY;
    result = write(port_handle, &_a_byte, 1);
    status = Status::IDLE;
    return result;
}

