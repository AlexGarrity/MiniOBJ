#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <iostream>
#include <cmath>

namespace miniOBJ {
    class Model {
        protected:
            std::vector<float_t> _vertices {};
            std::vector<float_t> _normals {};
            std::vector<float_t> _uvs {};

            std::vector<uint16_t> _indicesV {};
            std::vector<uint16_t> _indicesT {};
            std::vector<uint16_t> _indicesN {};


        public:
            Model(std::vector<uint8_t> data)
            {
                // Check that there's at least enough data for the header
                if (data.size() < 16) {
                    // Data is too small
                    return;
                }

                // Check we have the right data
                auto magicNum = std::string (data.begin(), data.begin() + 4);
                if (magicNum != "MOBJ") {
                    // Wrong data, I guess
                    return;
                }

                uint16_t vertexLength = 0;
                uint16_t textureLength = 0;
                uint16_t normalCount = 0;

                uint16_t iV = 0;
                uint16_t iT = 0;
                uint16_t iN = 0;

                // Extract relevant data from the header
                vertexLength = data[4] | data[5] << 8;
                textureLength = data[6] | data[7] << 8;
                normalCount = data[8] | data[9] << 8;
                
                iV = data[10] | data[11] << 8;
                iT = data[12] | data[13] << 8;
                iN = data[14] | data[15] << 8;

                _vertices.resize(vertexLength);
                _uvs.resize(textureLength);
                _normals.resize(normalCount);

                _indicesV.resize(iV);
                _indicesT.resize(iT);
                _indicesN.resize(iN);

                // Set the offset to the current read position
                // = Where the header ends (after iN)
                auto of = 16;
                for (auto x = 0; x < vertexLength * 4; x += 4) {
                    uint32_t val = data[of+x] | data[of+x+1] << 8 | data[of+x+2] << 16 | data[of+x+3] << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    _vertices[x] = fVal;
                }

                // 3 coords per vertex
                of += 3 * vertexLength;
                for (auto x = 0; x < textureLength * 4; x += 4) {
                    uint32_t val = data[of+x] | data[of+x+1] << 8 | data[of+x+2] << 16 | data[of+x+3] << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    _uvs[x] = fVal;
                }

                // 2 coords per UV
                of += 2 * textureLength;
                for (auto x = 0; x < normalCount * 4; x += 4) {
                    uint32_t val = data[of+x] | data[of+x+1] << 8 | data[of+x+2] << 16 | data[of+x+3] << 24;
                    float_t fVal = *reinterpret_cast<float_t*>(&val);
                    _normals[x] = fVal;
                }

                // 3 coords per normal
                of += 3 * normalCount;
                for (auto x = 0; x < iV * 2; x += 2) {
                    uint16_t val = data[of+x] | data[of+x+1] << 8;
                    _indicesV[x] = val;
                }

                // 3 indices per face
                of += 3 * iV;
                for (auto x = 0; x < iT * 2; x += 2) {
                    uint16_t val = data[of+x] | data[of+x+1] << 8;
                    _indicesV[x] = val;
                }
                
                // 3 indices per face
                of += 3 * iV;
                for (auto x = 0; x < iN * 2; x += 2) {
                    uint16_t val = data[of+x] | data[of+x+1] << 8;
                    _indicesV[x] = val;
                }
            }

            inline std::vector<float_t> Vertices() const { return _vertices; }
            inline std::vector<float_t> Normals() const { return _normals; }
            inline std::vector<float_t> UVs() const { return _uvs; }

            inline std::vector<uint16_t> VertexIndices() const { return _indicesV; }
            inline std::vector<uint16_t> NormalIndices() const { return _indicesN; }
            inline std::vector<uint16_t> UVIndices() const { return _indicesT; }

            size_t Size() const { return 
                (
                    _vertices.size() * sizeof(float_t) + 
                    _normals.size() * sizeof(float_t) +
                    _uvs.size() * sizeof(float_t) +

                    _indicesV.size() * sizeof(uint16_t) +
                    _indicesT.size() * sizeof(uint16_t) +
                    _indicesN.size() * sizeof(uint16_t) +
                    sizeof(Model)
                );
            }
    };

}