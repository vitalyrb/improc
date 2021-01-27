#include <repository/repository.hpp>
#include <functors/functors.hpp>

#include <application/application.hpp>

#include <iostream>
#include <filesystem>

using namespace std;
using namespace argus::bvi;
using namespace cv;

namespace fs = std::filesystem;

int main(void)
{
    try{
        cout << "Argus improc 2021.\n";

        auto repository = std::make_shared<MemoryImageRepository>();

        Application app;

        //add functors of required commands

        app.add({"blur"}, BlurFunctor(repository));
        app.add({"resize"}, ResizeFunctor(repository));
        app.add({"exit", "q", "quit"}, [](Tokens const&){ return false; /*exit from command loop*/});
        app.add({"help", "h"}, help);
        app.add({"load", "ld"}, [&repository](Tokens const& tokens){
            check_tokens(tokens, 3, "load");
            auto [name, filename] = pair{tokens[1], tokens[2]};
            auto image = imread(filename);
            if(image.empty()) throw user_exception(" can't load file \'" + filename + "\'. error: " + strerror(errno));
            repository->insert(name, image);
            return true;//continue to run of command loop
        });
        app.add({"stor", "s"}, [&repository](Tokens const& tokens){
            check_tokens(tokens, 3, "stor");
            auto [name, filename] = pair{tokens[1], tokens[2]};
            auto image = repository->find(name);
            if(false == imwrite(filename, image)) throw user_exception("can't save file \'" + filename + "\'. error: " + strerror(errno));
            return true;//continue to run of command loop
        });

        //add functors of optional commands

        app.add({"rlist", "rl"}, [&repository](Tokens const&){ repository->print(cout); return true; }); //output image list of repositories
        app.add({"pwd"}, pwd);//print current directory
        app.add({"cd"}, cd);  //change directory
        app.add({"ls"}, ls);  //print file list (ls [path])
        app.add({"show", "vi"}, ShowFunctor(repository)); //show image in window
        
        app.run(repository, cin); //we can use stringstream instead of cin. it let run application with command script file.

        return 0;
    }
    catch (std::exception const& ex)
    {
        std::cout << "exception im main: " << ex.what() << std::endl;   
    }
}