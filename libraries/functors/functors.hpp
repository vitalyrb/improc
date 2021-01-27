#pragma once

#include <common.hpp>

namespace argus::bvi
{
bool cd(Tokens const& tokens);
bool pwd(Tokens const&);
bool ls(Tokens const& tokens);
bool help(Tokens const& tokens);

void check_tokens(Tokens const& tokens, size_t count, std::string const& name);

class BlurFunctor
{
public:
    BlurFunctor(std::shared_ptr<IImageRepository> repository): repository_(repository){}
    bool operator() (Tokens const& tokens)
    {
        check_tokens(tokens, 4, "blur");
        auto [prev_name, new_name, size_str] = std::tuple{tokens[1], tokens[2], tokens[3]};
        int wh = atoi(size_str.c_str());
        cv::Size ksize{wh, wh};
        auto image = repository_->find(prev_name);
        cv::blur(image, image, ksize);
        repository_->insert(new_name, image);
        return true;
    }
protected:
    cv::Size to_size(std::string const& s) const
    {
        auto p = s.find('x', 0);
        int w = std::atoi(s.substr(0, p).c_str());
        int h = std::atoi(s.substr(p+1).c_str());
        return {w, h};
    }
protected:
    std::shared_ptr<IImageRepository> repository_;
};

class ResizeFunctor
{
public:
    ResizeFunctor(std::shared_ptr<IImageRepository> repository): repository_(repository){}
    bool operator() (Tokens const& tokens)
    {
        check_tokens(tokens, 5, "resize");
        auto [prev_name, new_name, ws, hs] = std::tuple{tokens[1], tokens[2], tokens[3], tokens[4]};
        cv::Size size(atoi(ws.c_str()), atoi(hs.c_str()));
        auto image = repository_->find(prev_name);
        cv::resize(image, image, size);
        repository_->insert(new_name, image);
        return true;
    }
protected:
    std::shared_ptr<IImageRepository> repository_;
};

class ShowFunctor
{
public:
    ShowFunctor(std::shared_ptr<IImageRepository> repository, int x = -1, int y = -1, int flags = cv::WINDOW_NORMAL): 
        repository_(repository), x_(x), y_(y), flags_(flags) 
    {}
    bool operator()(Tokens const& tokens)
    {
        check_tokens(tokens, 2, "show");
        auto name = tokens[1];
        auto image = repository_->find(name);
        std::string window_name = "Argus improc. " + name;
        cv::namedWindow(window_name, flags_);
        if(x_ != -1) cv::moveWindow(window_name, x_, y_);
        cv::imshow(window_name, image);
        while(cv::getWindowProperty(window_name, cv::WND_PROP_VISIBLE) >= 1){
            if(cv::waitKey(200) == 27) break;
        };
        cv::destroyWindow(window_name);
        return true;
    }  
protected:
    std::shared_ptr<IImageRepository> repository_;  
    int x_;
    int y_;
    int flags_;            
};

}//namespace argus::bvi