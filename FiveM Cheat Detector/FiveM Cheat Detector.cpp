#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <Windows.h>

class CheatScanner {
public:
    CheatScanner(const std::vector<std::string>& cheatNames) : cheatNames(cheatNames) {}

    void ScanDirectories(const std::string& rootDirectory) {
        try {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDirectory)) {
                if (entry.is_regular_file()) {
                    std::string fileName = entry.path().filename().string();
                    for (const auto& cheat : cheatNames) {
                        if (fileName.find(cheat) != std::string::npos) {
                            std::cout << "Cheat detected: " << fileName << " in " << entry.path() << std::endl;
                        }
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error accessing directory: " << e.what() << std::endl;
        }
    }

    void ScanRegistry() {
        HKEY hKey;
        const std::vector<std::string> registryPaths = {
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
            "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
        };

        for (const auto& path : registryPaths) {
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                char valueName[255];
                DWORD valueNameSize = sizeof(valueName);
                DWORD index = 0;

                while (RegEnumValueA(hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
                    for (const auto& cheat : cheatNames) {
                        if (std::string(valueName).find(cheat) != std::string::npos) {
                            std::cout << "Cheat found in registry: " << valueName << std::endl;
                        }
                    }
                    valueNameSize = sizeof(valueName);
                    index++;
                }
                RegCloseKey(hKey);
            }
        }
    }

private:
    std::vector<std::string> cheatNames;
};

int main() {
    std::vector<std::string> cheatNames = {
        "eulen",
        "redengine",
        "fivem_fps_boost",
        "fivem_norecoil",
        "darkmatter"
    };

    CheatScanner scanner(cheatNames);

    std::cout << "Searching through all directories..." << std::endl;
    scanner.ScanDirectories("C:\\"); // Starting from root directory (C:)

    std::cout << "Checking the registry..." << std::endl;
    scanner.ScanRegistry();

    std::cout << "Scan complete." << std::endl;
    return 0;
}
