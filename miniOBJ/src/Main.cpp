#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iterator>

#include "OBJ.hpp"

void PrintHeader() {
    std::cout << "==================" <<
    std::endl << "   OBJ Minifier" <<
    std::endl << "           v0.1" <<
    std::endl << "==================" <<
    std::endl;
}

std::vector<std::string> Split(std::string &str, char delimiter = ' ') {
    std::vector<std::string> out;
    std::stringstream sstream {str};
    std::string buffer;
    while(std::getline(sstream, buffer, delimiter))
        out.emplace_back(buffer);
    
    return out;
}

void ParseVertex(std::string &str, std::vector<std::float_t> &vertices) {
    auto c = Split(str);
    for(auto i = 1; i < c.size(); ++i) {
        vertices.emplace_back(std::stof(c[i]));
    }
}

void ParseFace(std::string &str, std::vector<uint16_t> &indices) {
    auto c = Split(str);
    for (auto i = 1; i < c.size(); ++i) {
        auto v = c[i].find('/');
        if (v != std::string::npos) {
            auto s = std::string(c[i].begin(), c[i].begin() + v);
            indices.emplace_back(static_cast<uint16_t>(std::stoi(s)));
        }
    }
}

std::vector<uint8_t> SplitFloat(float_t f) {
    auto v = *reinterpret_cast<uint32_t*>(&f);
    return { 
        static_cast<uint8_t>(v >>  0),
        static_cast<uint8_t>(v >>  8),
        static_cast<uint8_t>(v >> 16),
        static_cast<uint8_t>(v >> 24) 
    };
}

std::vector<uint8_t> SplitUint16(uint16_t u) {
    return { static_cast<uint8_t>(u), static_cast<uint8_t>(u >> 8) };
}

void ConvertObj(std::filesystem::path filepath) {
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "File " << filepath.stem() << " does not exist" << std::endl;
        return;
    }

    std::vector<float_t> vertices;
    std::vector<float_t> normals;
    std::vector<float_t> uvs;
    std::vector<uint16_t> faces;

    std::ifstream reader(filepath);
    std::string str;
    while (reader.good()) {
        std::getline(reader, str);
        switch (str[0]) {
            case 'f':
                ParseFace(str, faces);
                break;
            case 'v':
                switch(str[1]) {
                    case ' ':
                        ParseVertex(str, vertices);
                        break;
                    case 't':
                        ParseVertex(str, uvs);
                        break;
                    case 'n':
                        ParseVertex(str, normals);
                        break;
                    case 'p':
                        // Not handled
                        break;
                }
                break;
            default:
                continue;
        }   
    }

    reader.close();
    std::ofstream writer(filepath.parent_path() / filepath.stem() += ".mobj", std::ios::binary);
    // Write header
    writer.write("MOBJ", 4);

    auto vx = SplitUint16(vertices.size());
    auto nx = SplitUint16(normals.size());
    auto tx = SplitUint16(uvs.size());
    auto fx = SplitUint16(faces.size());

    writer << vx[0] << vx[1];
    writer << tx[0] << tx[1];
    writer << nx[0] << nx[1];
    writer << fx[0] << fx[1];

    for (auto v : vertices) { auto u = SplitFloat(v); writer << u[0] << u[1] << u[2] << u[3]; }
    for (auto v : uvs) { auto u = SplitFloat(v); writer << u[0] << u[1] << u[2] << u[3]; }
    for (auto v : normals) { auto u = SplitFloat(v); writer << u[0] << u[1] << u[2] << u[3]; }
    for (auto v : faces) {auto u = SplitFloat(v); writer << u[0] << u[1] << u[2] << u[3]; }
    writer.close();
    std::cout << "Converted '" << filepath.filename().string() << "'" << std::endl;
}

int main(int argc, char **argv) {
    PrintHeader();
    if (argc == 1) {
        std::cout << "No files specified" << std::endl;
        return 0;
    }
    std::cout << "Converting " << argc - 1 << " file" << ((argc==2)?"":"s") << std::endl;;
    for (auto i = 1; i < argc; ++i) {
        std::cout << "File " << i << " of " << argc - 1 << std::endl;
        ConvertObj(std::filesystem::absolute(argv[i]));
    }
    return 0;
}