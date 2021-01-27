#pragma once

#include <common.hpp>

#include <string>
#include <initializer_list>
#include <memory>
#include <functional>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <ostream>


namespace argus::bvi
{
typedef std::function<bool (Tokens &)> Functor;

std::string trim_command_line(std::string const& raw_line);
Tokens split_tokens(std::string const& trimmed_line);
std::ostream& operator<<(std::ostream& os, Tokens const& tokens);

class Application
{
public:
	bool add(std::initializer_list<std::string> const& names, Functor &&func);
    
	Application();
	explicit Application(std::ostream &output);
	explicit Application(std::string const& filename);
	int run(std::shared_ptr<IImageRepository> repository, std::istream &input);
protected:    
    Tokens read_command_line(std::istream &input_stream, std::string &raw_line);
    bool is_name_exist(std::initializer_list<std::string> const& names);
protected:    
    std::shared_ptr<IImageRepository> repository_;
    std::unordered_map<std::string, Functor> functors_;    
	std::ostream &output_;
};

}//namespace argus::bvi
