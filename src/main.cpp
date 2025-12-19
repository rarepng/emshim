#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>

std::string get_env_var(const char* name) {
#if defined(_MSC_VER)
    char *buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, name) == 0 && buf != nullptr) {
        std::string val = buf;
        free(buf);
        return val;
    }
    return "";
#else
    char* val = getenv(name);
    return val ? std::string(val) : "";
#endif
}

int main(int argc, char **argv) {
    std::string emsdk = get_env_var("EMSDK");

    // using py because .ps1 & .bat always fail because of having to escape all the " ' . : etc
    // and just for easier x-platform compatibility
    std::string emcmake_script;
    if (!emsdk.empty()) {
        emcmake_script = emsdk + "/upstream/emscripten/emcmake.py";
    } else {
        emcmake_script = "emcmake.py"; 
    }

    std::string python_exe = get_env_var("EMSDK_PYTHON");
    if (python_exe.empty()) {
        python_exe = "python";
    }

    bool probe = false;
    for (int i = 1; i < argc; i++) {
        std::string_view a = argv[i];
        if (a == "--version" || a == "-E" || a == "--build") {
            probe = true;
            break;
        }
    }

    std::vector<std::string> cmdParts;

    if (probe) {
        cmdParts.push_back("cmake");
        for (int i = 1; i < argc; ++i) cmdParts.push_back(argv[i]);
    } else {
        cmdParts.push_back(python_exe);
        cmdParts.push_back("\"" + emcmake_script + "\"");
        cmdParts.push_back("cmake");

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg.find(' ') != std::string::npos) {
                cmdParts.push_back("\"" + arg + "\"");
            } else {
                cmdParts.push_back(arg);
            }
        }
    }

    std::string cmd;
    for (const auto &part : cmdParts) {
        if (!cmd.empty()) cmd += " ";
        cmd += part;
    }

#ifdef DBG
    std::cout << "[emshim] full cmd: " << cmd << std::endl;
#endif

    return system(cmd.c_str());
}