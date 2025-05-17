#include <string>
#include <vector>
#include <fstream>

class Settings
{
    public:
        void Save(std::string fileName);
        void Load(std::string fileName);
        float wallColor[3] = {1, 1, 1};
        bool verticalSync = true;
        bool showFPS = true;
};
extern Settings settings;
