#include "functors.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using namespace std;

namespace argus::bvi
{
bool cd(Tokens const& tokens)
{
    check_tokens(tokens, 2, "cd");
    fs::current_path(tokens[1]);
    return true;
};  

bool pwd(Tokens const&)
{
    std::cout << "current path: " << fs::current_path() << std::endl;
    return true;
};

bool ls(Tokens const& tokens)
{
    std::string path{"."};
    if (tokens.size() > 1) path = tokens[1];
    for (const auto & entry : fs::directory_iterator(path))
        std::cout << entry.path() << "\n";      

    std::cout.flush();

    return true;
};  

bool help(Tokens const& tokens)
{
std::unordered_map<std::string, std::string> help_strings = {
{"load", "load, ld <name, path>  -  load image from file to repository.\n\
        name - name of image in repository\n\
        path - path to file in file system.\nexample: ld im1 im1.png\n\n"},
{"stor", "stor, s <name, path>  -  save image from repository  to file.\n\
        name - name of image in repository\n\
        path - path to file in file system.\nexample: s im1 im1.png\n\n"},
{"blur", "blur <from_name, to_name, size> - image bluring.\n\
        from_name - name of original image in repository\n\
        to_name - name new blured image in repository\n\
        size - blurring kernel size.\nexample: blur im1 im2 5\n\n"},
{"resize", "resize <from_name, to_name, new_width, new_height> - resize original image to new image with size(new_width, new_height)\n\
        from_name - name of original image in repository\n\
        to_name - name new blured image in repository\nexample: resize im1 im2 100 100\n\n"},
{"help", "help [command name] - outputs this message\n\
         command name - optional parameter. outputs help message only for specifed command.\nexample: help blur\n\n"},
{"usage", "Usage: command [options...][path]\n\nCommands:\n"}};
 
    if(tokens.size() > 1)
        std::cout << help_strings[tokens[1]] << std::endl;
    else
        for(auto const& [n, s]: help_strings) std::cout << s << "\n";

    std::cout.flush();

    return true;
}

void check_tokens(Tokens const& tokens, std::size_t count, std::string const& name)
{
    if(tokens.size() < count) throw user_exception("not enought parameters for exacution command " + name);
}

}//amespace argus::bvi