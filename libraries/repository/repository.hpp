#pragma once

#include <opencv2/opencv.hpp>

#include <common.hpp>

namespace argus::bvi
{

class MemoryImageRepository : public IImageRepository
{
    typedef std::unordered_map<std::string, cv::Mat> Repository;
public:
    void insert(std::string const& name, cv::Mat const &image) override;    
    void erase(std::string const& name)  override ;
    cv::Mat find(std::string const& name) override ;
    std::size_t count() const override ;
    void print(std::ostream &os) const override;
protected:
    bool is_name_exist(std::string const& name) const;
protected:
    Repository repository_;    
};

}//namespace argus::bvi