#include "HeightMapGenerator.hpp"
#include "FastNoiseLite.h"
#include "Texture.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <thread>
#include <vector>

spry::HeightMapGenerator::HeightMapGenerator()
{
    float freq = 1.0f;
    float ampl = 1.0f;
    for (auto& [generator, amplitude] : mGenerators) {
        generator.SetSeed(rand());
        generator.SetFrequency(freq);
        generator.SetFractalType(FastNoiseLite::FractalType_FBm);
        amplitude = ampl;
        ampl /= 2;
        freq *= 2;
    }
}

void spry::HeightMapGenerator::load(uint32_t width, uint32_t height)
{
    mBuffer = std::vector<uint8_t>(height * width * 4);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            const uint32_t offset = (width * i + j) * 4;
            const float nx = i / float(width) - 0.5f;
            const float ny = j / float(height) - 0.5f;
            const float noiseHeight = mFastNoiseLite.GetNoise(nx, ny);
            const uint8_t heightNorm = ((noiseHeight + 1.0f) / 2.0f) * 256.0;

            mBuffer[offset + 0] = heightNorm;
            mBuffer[offset + 1] = heightNorm;
            mBuffer[offset + 2] = heightNorm;
            mBuffer[offset + 3] = heightNorm;
        }
    }

    mWidth = width;
    mHeight = height;
}

void spry::HeightMapGenerator::generate(uint32_t width, uint32_t height)
{
    mBuffer = std::vector<uint8_t>(height * width * 4);

    const uint32_t threadCount = 4;
    const uint32_t resolution = width * height;
    const uint32_t workLen = resolution / threadCount;
    const auto& generators = mGenerators;
    auto& buffer = mBuffer;
    std::vector<std::thread> pool;

    for (uint32_t i = 0; i < threadCount; i++) {
        const uint32_t fillStart = i * workLen;
        pool.emplace_back(
            std::thread([fillStart, workLen, height, width, &generators, &buffer, power = this->mPower]() mutable {
                for (uint32_t idx = fillStart; idx < fillStart + workLen; idx++) {
                    const uint32_t x = idx / width;
                    const uint32_t y = idx % width;
                    const float nx = x / float(width) - 0.5f;
                    const float ny = y / float(height) - 0.5f;
                    float noiseHeight = 0.0f;
                    float amplitudeSum = 0.0f;

                    for (const auto& generator : generators) {
                        const auto& [gen, amplitude] = generator;
                        const float noise = (gen.GetNoise(nx, ny) + 1.0f) / 2.0f;
                        noiseHeight += amplitude * noise;
                        amplitudeSum += amplitude;
                    }

                    noiseHeight /= amplitudeSum;
                    const uint8_t heightNorm = std::pow(noiseHeight, power) * 256;
                    const uint32_t offset = idx * 4;
                    buffer[offset + 0] = heightNorm;
                    buffer[offset + 1] = heightNorm;
                    buffer[offset + 2] = heightNorm;
                    buffer[offset + 3] = heightNorm;
                }
            }));
    }

    for (auto& handle : pool) {
        handle.join();
    }

    /*
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            const float nx = i / float(width) - 0.5f;
            const float ny = j / float(height) - 0.5f;
            float noiseHeight = 0.0f;
            float amplitudeSum = 0.0f;

            for (const auto& generator : mGenerators) {
                const auto& [gen, amplitude] = generator;
                noiseHeight += amplitude * gen.GetNoise(nx, ny);
                amplitudeSum += amplitude;
            }

            noiseHeight /= amplitudeSum;
            const uint8_t heightNorm = std::pow(((noiseHeight + 1.0f) / 2.0f) * 256.0, 1.03);
            const uint32_t offset = (width * i + j) * 4;
            mBuffer[offset + 0] = heightNorm;
            mBuffer[offset + 1] = heightNorm;
            mBuffer[offset + 2] = heightNorm;
            mBuffer[offset + 3] = heightNorm;
        }
    }
    */

    mWidth = width;
    mHeight = height;
}

const std::vector<uint8_t>& spry::HeightMapGenerator::getHeightMap() const
{
    return mBuffer;
}

spry::Texture spry::HeightMapGenerator::createTextureFromHeightMap(
    uint32_t width,
    uint32_t height)
{
    Texture texture;
    texture.create()
        .setWrapMode(GL_REPEAT)
        .setFilterMode(GL_LINEAR)
        .load((void*)(mBuffer.data()), width, height, GL_RGBA);

    return texture;
}
