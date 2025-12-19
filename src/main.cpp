#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    std::string emsdk;

    #if defined(_MSC_VER)
        char* tmp = nullptr;
        size_t len = 0;
        _dupenv_s(&tmp, &len, "EMSDK");
        if(tmp) { emsdk = tmp; free(tmp); }
    #else
        char* tmp = getenv("EMSDK");
        if(tmp) emsdk = tmp;
    #endif

    std::string emcmake;
    if(!emsdk.empty()) {
        emcmake = emsdk + "/upstream/emscripten/emcmake.ps1";
    } else {
        emcmake = "emcmake";
    }

    std::string pwsh;
    #if defined(_MSC_VER)
        char* tmp2 = nullptr; size_t len2 = 0;
        _dupenv_s(&tmp2, &len2, "PWSH");
        if(tmp2) { pwsh = tmp2; free(tmp2); }
    #endif
        if(pwsh.empty()) pwsh = "pwsh";

    // build command safely
    std::vector<std::string> cmdParts;
    if(!emsdk.empty()) {
        cmdParts.push_back(pwsh);
        cmdParts.push_back("-NoProfile");
        cmdParts.push_back("-ExecutionPolicy Bypass");
        cmdParts.push_back("-File \"" + emcmake + '"');
    } else {
        cmdParts.push_back(emcmake);
    }

    for(int i=1;i<argc;i++)
        cmdParts.push_back('"' + std::string(argv[i]) + '"');

    for(int i = 1; i < argc; ++i)
        cmdParts.push_back('"' + std::string(argv[i]) + '"');

    std::string cmd;
    for(const auto& part : cmdParts) {
        if(!cmd.empty()) cmd += " ";
        cmd += part;
    }

    std::cout << "DEBUG: " << cmd << "\n";
    return system(cmd.c_str());
}
