#include "repository.hpp"

#include <catch2/catch.hpp>

#include <exception>

using namespace argus::bvi;

TEST_CASE("MemoryImageRepository::iprint_list", "[repository]")
{
    auto repository = std::make_shared<MemoryImageRepository>();
    repository->insert("im1", cv::Mat(600, 400, CV_8UC1));
    repository->insert("im2", cv::Mat(800, 400, CV_8UC1));
    repository->insert("tmp", cv::Mat(800, 400, CV_8UC1));
    repository->insert("im3", cv::Mat(600, 300, CV_8UC1));

    repository->erase("tmp");

    std::ostringstream os;
    repository->print(os);
    std::string s = os.str();

    REQUIRE(s == "im2: [400 x 800]\nim3: [300 x 600]\nim1: [400 x 600]\n");
}

TEST_CASE("MemoryImageRepository::insert/find/erase", "[repository]")
{
    auto repository = std::make_shared<MemoryImageRepository>();

    cv::Scalar color_pattern(64, 128, 196);
    cv::Mat image(574, 480, CV_8UC3);
    image = color_pattern;

    repository->insert("image1", image);

    REQUIRE(repository->count() == 1);

    bool f = false;
    try{
        auto image = repository->find("image2");
    }
    catch(user_exception const& ex)
    {
        f = true;
    }

    REQUIRE(f);

    repository->insert("image2", image.clone());

    REQUIRE(repository->count() == 2);

    f = false;
    try{
        auto image = repository->find("image2");
        REQUIRE(!image.empty());
    }
    catch(user_exception const& ex)
    {
        f = true;
    }

    REQUIRE(f == false);

    repository->erase("image1");
    REQUIRE(repository->count() == 1);
    auto image2 = repository->find("image2");
    REQUIRE(!image2.empty());    

    bool isEqual = (cv::sum(image2 != image) == cv::Scalar(0,0,0,0));

    REQUIRE(isEqual);


    f = false;
    try{
        repository->insert("image3", cv::Mat());
    }
    catch(user_exception const& ex)
    {
        f = true;
    }

    REQUIRE(f == true);
    
}