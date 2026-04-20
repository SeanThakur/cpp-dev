#include <iostream>
#include <memory>
#include <utility>
#include <string>

class Image
{

public:
    std::string name;
    int size;
    Image(std::string n, int s) : name(n), size(s)
    {
        std::cout << "[+] Image Created [+]\n";
    }
    ~Image()
    {
        std::cout << "[-] Image Deleted [-]\n";
    }
};

std::unique_ptr<Image> downloadingImage(std::string filename)
{
    std::cout << "--->Downloading Image " << filename << "\n";
    return std::make_unique<Image>(filename, 1024);
}

std::unique_ptr<Image> applyFilter(std::unique_ptr<Image> img)
{
    std::cout << "--> Applying Black & White filter to " << img->name << "...\n";
    img->name = img->name + "-filtered!";
    return img;
}

void saveImage(std::unique_ptr<Image> img)
{
    std::cout << "--> Saving the image " << img->name << "...\n";
}

int main()
{
    std::cout << "----Main Programm---- \n";

    std::unique_ptr<Image> img = downloadingImage("jacob_ladder.jepg");
    auto filteredImg = applyFilter(std::move(img));
    saveImage(std::move(filteredImg));

    std::cout << "---- END of the program---- \n";
    return 0;
}