#pragma once

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <ostream>

namespace argus::bvi
{
typedef std::vector<std::string> Tokens;

struct IImageRepository 
{
    virtual void insert(std::string const& name, cv::Mat const &image) = 0;
    virtual void erase(std::string const& name) = 0;
    virtual cv::Mat find(std::string const& name) = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual size_t count() const = 0;
    virtual ~IImageRepository(){};
};

class user_exception: public std::exception
{
public:
    explicit user_exception(const char* message)
        : msg_(message) {}
    explicit user_exception(const std::string& message)
        : msg_(message) {}
    virtual ~user_exception() noexcept {}
    virtual const char* what() const noexcept {
       return msg_.c_str();
    }
protected:
    std::string msg_;
};

} // namespace argus::bvi