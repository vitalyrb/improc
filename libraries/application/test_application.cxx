#include "application.hpp"

#include <catch2/catch.hpp>

#include <exception>
#include <ostream>

using namespace argus::bvi;

class MockApplication: public Application
{
public:
    MockApplication(std::ostream& output): Application(output){};
    bool run_functor(const std::string& name, const std::string& arg1 = "image1", const std::string& arg2 = "image1.jpg")
    {
        try{
            auto fun = functors_[name];

            Tokens tokens{name, arg1, arg2};
            return fun(tokens);
        }
        catch(std::bad_function_call const& ex)
        {
            output_ << "warning: detected to trying run not exist command or functor: " << name << std::endl;            
            return false;
        }
    }
 
};

class Mockrepository : public IImageRepository
{
public:
    void insert(std::string const&, cv::Mat const&) override {}
    void erase(std::string const&)  override {}
    cv::Mat find(std::string const&) override {return cv::Mat();}
    std::size_t count() const override {return 0;};
    void print(std::ostream &) const override
    {
    }
};

class MockCommand
{
public:
    MockCommand(std::shared_ptr<IImageRepository> repository, int *count = nullptr): repository_(repository), count_(count){}
    bool operator() (Tokens const& tokens) 
    {
        if(nullptr != count_) ++(*count_);
        return (tokens[0] == "help") && (tokens[1] == "image3") && (tokens[2] == "image3.jpg");
    }
protected:
    std::shared_ptr<IImageRepository> repository_;
    int *count_;
};

TEST_CASE("Aplication::add(name, Functor)", "[application]")
{
    std::stringstream eos;
    MockApplication app(eos);

    app.add({"load", "ld"}, [](Tokens const& tokens){
        return ((tokens[0] == "load") || (tokens[0] == "ld")) && (tokens[1] == "image1") && (tokens[2] == "image1.jpg");
    });

    REQUIRE(app.run_functor("load"));
    REQUIRE(app.run_functor("ld"));

    auto repository = std::make_shared<Mockrepository>();
    app.add({"help", "h"}, MockCommand(repository));
    REQUIRE(app.run_functor("help", "image3", "image3.jpg"));

    bool r1 = app.add({"stor", "ld"}, [](Tokens const& tokens){
        return (tokens[0] == "load") && (tokens[1] == "image1") && (tokens[2] == "image1.jpg");
    });    

    REQUIRE(r1 == false);

    bool r2 = app.add({"stor", "s"}, [](Tokens const& tokens){
        return ((tokens[0] == "stor")||(tokens[0] == "s")) && (tokens[1] == "image1") && (tokens[2] == "image1.jpg");
    });     

    REQUIRE(app.run_functor("s"));

    REQUIRE(r2 == true);

    bool r3 = app.add({"help", "ld"}, [](Tokens const& tokens){
        return (tokens[0] == "load") && (tokens[1] == "image1") && (tokens[2] == "image1.jpg");
    });    

    REQUIRE(r3 == false);

    REQUIRE(false == app.run_functor("m"));

    REQUIRE(false == app.run_functor("m"));

}

TEST_CASE("Aplication::run(repository, input)", "[application]")
{
    std::stringstream eos;
    MockApplication app1(eos);

    auto repository = std::make_shared<Mockrepository>();

    int count = 0;

    app1.add({"load", "ld"}, [&count](Tokens const& tokens){
        ++count;
        return (tokens[0] == "load") && (tokens[1] == "image1") && (tokens[2] == "image1.jpg");
    });

    app1.add({"stor", "s"}, [&count](Tokens const& tokens){
        ++count;
        return (tokens[0] == "stor") && (tokens[1] == "image2") && (tokens[2] == "image2.jpg");
    });

    app1.add({"exit"}, [&count](Tokens const& tokens){
        ++count;
        if(tokens[0] == "exit") return false;
        throw user_exception("fail command name: " + tokens[0]);
    });

    app1.add({"testbreak"}, [&count](Tokens const&) -> bool {
        ++count;
        throw std::runtime_error("runtime error");
    });    

    app1.add({"help", "h"}, MockCommand(repository, &count));    

    std::stringstream input(
"load image1 image1.jpg\n \
unknown bad_arg1 badarg2\n \
testbreak\n \
stor    \t image2\timage2.jpg\n \
\t\t\t  help image3 image3.jpg\n \
exit\n");
 
    app1.run(repository, input);    

    REQUIRE(count == 5);
}

