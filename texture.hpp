#include "commons.hpp"

class Texture
{
    private:
    std::string filepath;
    public:
    int TextureID;
    Texture();
    ~Texture();
    void Load(const std::string &filepath);
};