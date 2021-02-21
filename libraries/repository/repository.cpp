#include "repository.hpp"

namespace argus::bvi
{

void MemoryImageRepository::insert(std::string const& name, cv::Mat const &image) 
{
    if(image.empty()) throw user_exception("Image \'" + name + "\' is empty. The image did not load to repository.");
    if(is_name_exist(name))
    {
        throw user_exception("Image \'" + name + "\' already exists. The image did not load to repository.");
    }
    try{
        repository_[name] = image.clone();
    }
    catch(std::exception const& ex)
    {
        throw user_exception("Image \'" + name + "\' is not load to repository. exeption: " + ex.what());
    }        
}
void MemoryImageRepository::erase(std::string const& name)   
{
    try{
        auto it = repository_.find(name);
        if(it == repository_.end()) throw user_exception("Image \'" + name + "\' is not erase from repository. Thre image is not found.");
        repository_.erase(it);
    }
    catch(std::exception const& ex)
    {
        throw user_exception("Image \'" + name + "\' is not erase. exeption: " + ex.what());
    }         
}
cv::Mat MemoryImageRepository::find(std::string const& name)   
{
    if(!is_name_exist(name))
    {
        throw user_exception("Image \'" + name + "\' is not found in repository.");
    }        
    try{
        return repository_[name].clone();
    }
    catch(std::exception const& ex)
    {
        throw user_exception("Image \'" + name + "\' is not found. exeption: " + ex.what());
    }   
}  

void MemoryImageRepository::print(std::ostream &os) const
{
    for(auto const& [name, image]: repository_)
    {
        os << name << ": " << image.size() << std::endl;
    }        
}

bool MemoryImageRepository::is_name_exist(std::string const& name) const
{
    return repository_.find(name) != repository_.end();
}      

std::size_t MemoryImageRepository::count() const 
{
    return repository_.size();
}

}//namespace argus::bvi