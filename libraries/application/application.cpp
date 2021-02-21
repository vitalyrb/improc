#include "application.hpp"
#include "colors.hpp"


namespace argus::bvi
{

bool Application::add(std::initializer_list<std::string> const& names, Functor &&func)
{
    if(!is_name_exist(names))
    {
        for(const auto& name: names)
        {
            functors_[name] = func;
        }
        return true;
    }

    return false;
}    

std::ostream& operator<<(std::ostream& os, Tokens const& tokens)
{
    for(auto && t: tokens)
    {
        os << t << " ";
    }
    return os;
}

Application::Application(): output_(std::cout){};
Application::Application(std::ostream &output): output_(output){}
Application::Application(std::string const&): output_(std::cout){}; //TODO: for loading command script from file. Is not implemented in current version.
int Application::run(std::shared_ptr<IImageRepository> repository, std::istream &input)
{
    repository_ = repository;
    bool continue_flag{true};
    std::string raw_line;
    std::string command_name;
    while(continue_flag){
        try{
            auto tokens = read_command_line(input, raw_line);
            if(tokens.empty()) continue;
            command_name = tokens[0];
            continue_flag = functors_[command_name](tokens);
        }
        catch(std::bad_function_call const& ex)
        {
            output_ << FYEL("warning: ") << SFYEL("fail command: " + command_name) << FYEL(". exception: ") << SFYEL(ex.what()) << std::endl;
            continue_flag = true;
        }
        catch(user_exception const& ex){
            output_ << FYEL("warning: ") << SFYEL(ex.what()) << std::endl;
            continue_flag = true;
        }
        catch(std::exception const& ex){
            output_ << FRED("error: ") << SFRED(ex.what()) << std::endl;
            output_ << BOLD("press 0 for exit.\npress 1 for continue.\n") << std::endl;
            int choice = 0;
            if(command_name == "testbreak") //need for unit tests
                choice = 1;
            else 
                std::cin >> choice;
            if(0 == choice) return -2;
            if(1 == choice) { continue_flag = true; continue;}
        }
        output_ << std::endl;
    }; 

    return 0;
}

Tokens Application::read_command_line(std::istream &input_stream, std::string &raw_line)
{
    output_ << "> ";
    std::getline(input_stream, raw_line);
    auto trimmed_line = trim_command_line(raw_line);
    auto tokens = split_tokens(trimmed_line);
    return tokens;
}

bool Application::is_name_exist(std::initializer_list<std::string> const& names)
{
    for(const auto& name: names)
    {
        if(functors_.find(name) != functors_.end())
        {
            output_ << "warning: functor with name:  " << name << " already exists" << std::endl;
            return true;
        }
    }

    return false;
}

std::string trim_command_line(std::string const& raw_line)
{
    std::string s = raw_line;
    replace(s.begin(), s.end(), '\t', ' ');
    auto new_end = unique(s.begin(), s.end(), [](const char l, const char &r){
        return std::isspace(l) && std::isspace(r);
    });
    s.erase(new_end, s.end()); 
    if(s[0] == ' ') s.erase(0, 1);   

    return s; 
}

Tokens split_tokens(std::string const& trimmed_line)
{
    Tokens tokens;
    std::stringstream ss(trimmed_line);
    std::string s;
    while (std::getline(ss, s, ' '))
    {
        if(!s.empty ()){
            tokens.push_back(s);
        } 
    }      

    return tokens;      
}


}//namespace argus::bvi