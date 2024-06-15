#ifndef VERSION_HH
#define VERSION_HH

#include <string>
#include <iostream>
#include <fstream>

/* Only modify this, no need to modify others. */
#ifndef _VER_NUM
#define _VER_NUM "0.0.1-alpha"
#endif

#ifndef _ARCH
#ifdef __x86_64__
#define _ARCH "amd64"
#elif defined(_M_X64)
#define _ARCH "x86_64"
#elif defined(__aarch64__)
#define _ARCH "arm64"
#elif defined(__powerpc64__)
#define _ARCH "ppc64"
#elif defined(__i686__) || defined(__i586__) || defined(__i486__) || defined(__i386__)
#define _ARCH "i686/i586/i486/i386"
#else
#define _ARCH "Unknown"
#endif
#endif

// Define OS based on compilation flags
#ifndef _OS
#ifdef __linux__

inline std::string get_os_release_name()
{
    std::ifstream file("/etc/os-release");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("NAME=") != std::string::npos)
            {
                std::size_t pos = line.find('=');
                if (pos != std::string::npos)
                {
                    std::string name = line.substr(pos + 1);
                    name.erase(name.find_last_not_of("\r\n") + 1);
                    if (!name.empty() && (name.front() == '"' || name.front() == '\''))
                    {
                        name = name.substr(1, name.size() - 2);
                    }
                    return name;
                }
            }
        }
        file.close();
    }
    return "Unknown Linux";
}

#define _OS "Linux" + get_os_release_name()
#elif defined(__FreeBSD__)
#define _OS "FreeBSD"
#elif defined(__OpenBSD__)
#define _OS "OpenBSD"
#elif defined(__NetBSD__)
#define _OS "NetBSD"
#elif defined(_WIN32) || defined(_WIN64)
#define _OS "Windows"
#elif defined(__unix__) || defined(__unix)
#define _OS "Unix"
#elif defined(__APPLE__) || defined(__MACH__)
#define _OS "MacOS"
#else
#define _OS "Unknown"
#endif
#endif

#ifndef __SET_OS_NAME
#define __SET_OS_NAME
inline std::string get_version()
{
    static std::string VERSION = std::string(_VER_NUM) + " " + _OS;
#ifdef __linux__
    VERSION += " (" + get_os_release_name() + ")";
#endif
    VERSION += " " + std::string(_ARCH);
    return VERSION;
}
#endif
#endif