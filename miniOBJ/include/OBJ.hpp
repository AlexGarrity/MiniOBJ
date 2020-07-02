#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <cmath>

namespace miniOBJ {
    class Model {
        protected:
            std::vector<float_t> _vertices;
            std::vector<float_t> _normalCoordinates;
            std::vector<float_t> _uvCoordinates;
            std::vector<uint16_t> _indices;


        public:
            Model(std::vector<uint8_t> *data) 
            : _vertices {0}, _normalCoordinates {0}, _uvCoordinates {0}, _indices {0} 
            {

                if (!data) {
                    return;
                }

                // Check that there's at least enough data for the header
                if (data->size() < 16) {
                    // Data is too small
                    return;
                }

                // Check we have the right data
                auto magicNum = std::string (data->begin(), data->begin() + 4);
                if (magicNum != "MOBJ") {
                    // Wrong data, I guess
                    return;
                }

                uint16_t vertexLength = 0;
                uint16_t textureLength = 0;
                uint16_t normalCount = 0;
                uint16_t faceCount = 0;

                // Extract relevant data from the header
                vertexLength = data->at(4) | data->at(5) << 8;
                textureLength = data->at(6) | data->at(7) << 8;
                normalCount = data->at(8) | data->at(9) << 8;
                faceCount = data->at(10) | data->at(11) << 8;

                _vertices.resize(vertexLength);
                _uvCoordinates.resize(textureLength);
                _normalCoordinates.resize(normalCount);
                _indices.resize(faceCount);

                // Set the offset to the current read position
                auto of = 12;
                for (auto x = 0; x < vertexLength * 4; x += 4) {
                    uint32_t val = data->at(of+x) | data->at(of+x+1) << 8 | data->at(of+x+2) << 16 | data->at(of+x+3) << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    _vertices[x] = fVal;
                }

                of += 3 * vertexLength;
                for (auto x = 0; x < textureLength * 4; x += 4) {
                    uint32_t val = data->at(of+x) | data->at(of+x+1) << 8 | data->at(of+x+2) << 16 | data->at(of+x+3) << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    _uvCoordinates[x] = fVal;
                }

                of += 2 * textureLength;
                for (auto x = 0; x < normalCount * 4; x += 4) {
                    uint32_t val = data->at(of+x) | data->at(of+x+1) << 8 | data->at(of+x+2) << 16 | data->at(of+x+3) << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    _normalCoordinates[x] = fVal;
                }

                of += 3 * normalCount;
                for (auto x = 0; x < faceCount * 2; x += 2) {
                    uint16_t val = data->at(of+x) | data->at(of+x+1) << 8;
                    _indices[x] = val;
                }
            }

            inline std::vector<float_t> Vertices() const { return _vertices; }
            inline std::vector<float_t> Normals() const { return _normalCoordinates; }
            inline std::vector<float_t> UVs() const { return _uvCoordinates; }
            inline std::vector<uint16_t> Indices() const { return _indices; }

            inline size_t Size() const { return 
                (_vertices.size() * sizeof(float_t) + 
                _normalCoordinates.size() * sizeof(float_t) +
                _uvCoordinates.size() * sizeof(float_t) +
                _indices.size() * sizeof(uint16_t));
            }
    };

}