#include <iostream>
#include <string>
#include <memory>

class Texture
{
public:
    std::string filename;

    Texture(std::string name) : filename(name)
    {
        std::cout << "[+] Loading Texture from disk " << filename << "\n";
    }

    ~Texture()
    {
        std::cout << "[-] Texture is Destroyed " << filename << "\n";
    }
};

class Enemy
{
private:
    std::shared_ptr<Texture> myTexture;

public:
    Enemy(std::shared_ptr<Texture> tex) : myTexture(tex)
    {
        std::cout << "[+] Enemy spawned!\n";
    }

    ~Enemy()
    {
        std::cout << "[+] Enemy Died!\n";
    }
};

int main()
{
    std::cout << "--- Game Starting ---\n";
    std::shared_ptr<Texture> masterTex = std::make_shared<Texture>("orc.jpeg");
    std::cout << "Texture use count (Creation) " << masterTex.use_count() << "\n";
    std::cout << "\n--- Entering Spawn Zone ---\n";
    {
        Enemy e1(masterTex);
        Enemy e2(masterTex);
        Enemy e3(masterTex);

        std::cout << "Texture use count (Inside zone): " << masterTex.use_count() << "\n";
        std::cout << "\n--- Leaving Spawn Zone ---\n";
    }
    std::cout << "Texture use count (Back in main): " << masterTex.use_count() << "\n";
    std::cout << "--- Game Over ---\n";
    return 0;
}