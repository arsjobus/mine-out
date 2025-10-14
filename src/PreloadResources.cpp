// PreloadResources.cpp

#include "PreloadResources.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <limits.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#endif

#include <iostream>
#include <filesystem>   // C++17 — for checking path exists etc

namespace fs = std::filesystem;

// Static member definitions
std::vector<sf::SoundBuffer> PreloadResources::sound;
std::vector<sf::Font> PreloadResources::fonts;
std::vector<sf::Texture> PreloadResources::txtAnimation;
std::vector<sf::Texture> PreloadResources::txtBackground;
std::vector<sf::Texture> PreloadResources::txtBlock;
std::vector<sf::Texture> PreloadResources::txtLevel;
std::vector<sf::Texture> PreloadResources::txtPaddle;
std::vector<sf::Texture> PreloadResources::txtPowerup;

PreloadResources::PreloadResources() {
    loadDefaultSettings();
    // You might want to initialize other state here
}

PreloadResources::~PreloadResources() {
    // cleanup if needed
}

void PreloadResources::loadDefaultSettings() {
    this->setAnimationDirectoryName("animations");
    this->setBackgroundDirectoryName("backgrounds");
    this->setBlockDirectoryName("blocks");
    this->setPaddleDirectoryName("paddles");
    this->setLevelDirectoryName("levels");
    this->setPowerupDirectoryName("powerups");
    this->setTextureDirectoryName("images");
    this->setFontDirectoryName("fonts");
    this->setSoundDirectoryName("sfx");
    this->setSoundFileType(".wav");
    this->setTextureFileType(".png");
    this->setLoadPercentile(0.f);
    this->setLoadedResourceCount(0);
    this->setResourceCount(0);
}

// resolvePath and getExecutableBasePath as before

std::string PreloadResources::resolvePath(const std::string &relativePath) {
#ifdef __APPLE__
    static std::string bundleRes;
    if (bundleRes.empty()) {
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        if (mainBundle != nullptr) {
            CFURLRef resURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
            if (resURL != nullptr) {
                UInt8 pathBuf[PATH_MAX];
                Boolean ok = CFURLGetFileSystemRepresentation(resURL, true, pathBuf, sizeof(pathBuf));
                CFRelease(resURL);
                if (ok) {
                    bundleRes = std::string(reinterpret_cast<char*>(pathBuf));
                    if (!bundleRes.empty() && bundleRes.back() != '/') {
                        bundleRes.push_back('/');
                    }
                }
            }
        }
    }
    if (!bundleRes.empty()) {
        std::string candidate = bundleRes + relativePath;
        if (fs::exists(candidate)) {
            return candidate;
        }
    }
#endif

    std::string base = getExecutableBasePath();
    if (!base.empty()) {
        std::string candidate2 = base + relativePath;
        if (fs::exists(candidate2)) {
            return candidate2;
        }
    }

    return relativePath;
}

std::string PreloadResources::getExecutableBasePath() {
#ifdef __APPLE__
    char buf[PATH_MAX];
    uint32_t bufsize = sizeof(buf);
    if (_NSGetExecutablePath(buf, &bufsize) == 0) {
        fs::path exePath(buf);
        fs::path contentDir = exePath.parent_path().parent_path();  // go up from MacOS to Contents
        fs::path resourcesDir = contentDir / "Resources";
        std::string s = resourcesDir.string();
        if (!s.empty() && s.back() != '/') {
            s.push_back('/');
        }
        return s;
    }
#endif
    return std::string();
}

// Relative paths for directories

std::string PreloadResources::soundFilePath() {
    return this->getSoundDirectoryName() + "/";
}

std::string PreloadResources::animationTextureFilePath() {
    return this->getTextureDirectoryName() + "/" + this->getAnimationDirectoryName() + "/";
}

std::string PreloadResources::backgroundTextureFilePath() {
    return this->getTextureDirectoryName() + "/" + this->getBackgroundDirectoryName() + "/";
}

std::string PreloadResources::blockTextureFilePath() {
    return this->getTextureDirectoryName() + "/" + this->getBlockDirectoryName() + "/";
}

std::string PreloadResources::levelTextureFilePath() {
    return this->getTextureDirectoryName() + "/" + this->getLevelDirectoryName() + "/";
}

std::string PreloadResources::paddleTextureFilePath() {
    return this->getTextureDirectoryName() + "/" + this->getPaddleDirectoryName() + "/";
}

std::string PreloadResources::powerupTextureFilePath() {
    return this->getTextureDirectoryName() + "/" + this->getPowerupDirectoryName() + "/";
}

std::string PreloadResources::fontsFilePath() {
    return this->getFontDirectoryName() + "/";
}

// Loading routines (sound, textures) — existing from your code…

void PreloadResources::loadSound(const char *filename) {
    std::string rel = this->soundFilePath() + filename + this->getSoundFileType();
    std::string fullpath = resolvePath(rel);
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(fullpath)) {
        std::cerr << "Could not load sound: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded sound: " << fullpath << std::endl;
    }
    sound.push_back(soundBuffer);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

void PreloadResources::loadAnimationTexture(const char *filename) {
    std::string rel = this->animationTextureFilePath() + filename + this->getTextureFileType();
    std::string fullpath = resolvePath(rel);
    sf::Texture texture;
    if (!texture.loadFromFile(fullpath)) {
        std::cerr << "Could not load animation texture: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded animation texture: " << fullpath << std::endl;
    }
    txtAnimation.push_back(texture);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

void PreloadResources::loadBackgroundTexture(const char *filename) {
    std::string rel = this->backgroundTextureFilePath() + filename + this->getTextureFileType();
    std::string fullpath = resolvePath(rel);
    sf::Texture texture;
    if (!texture.loadFromFile(fullpath)) {
        std::cerr << "Could not load background texture: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded background texture: " << fullpath << std::endl;
    }
    txtBackground.push_back(texture);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

void PreloadResources::loadBlockTexture(const char *filename) {
    std::string rel = this->blockTextureFilePath() + filename + this->getTextureFileType();
    std::string fullpath = resolvePath(rel);
    sf::Texture texture;
    if (!texture.loadFromFile(fullpath)) {
        std::cerr << "Could not load block texture: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded block texture: " << fullpath << std::endl;
    }
    txtBlock.push_back(texture);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

void PreloadResources::loadLevelTexture(const char *filename) {
    std::string rel = this->levelTextureFilePath() + filename + this->getTextureFileType();
    std::string fullpath = resolvePath(rel);
    sf::Texture texture;
    if (!texture.loadFromFile(fullpath)) {
        std::cerr << "Could not load level texture: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded level texture: " << fullpath << std::endl;
    }
    txtLevel.push_back(texture);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

void PreloadResources::loadPaddleTexture(const char *filename) {
    std::string rel = this->paddleTextureFilePath() + filename + this->getTextureFileType();
    std::string fullpath = resolvePath(rel);
    sf::Texture texture;
    if (!texture.loadFromFile(fullpath)) {
        std::cerr << "Could not load paddle texture: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded paddle texture: " << fullpath << std::endl;
    }
    txtPaddle.push_back(texture);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

void PreloadResources::loadPowerupTexture(const char *filename) {
    std::string rel = this->powerupTextureFilePath() + filename + this->getTextureFileType();
    std::string fullpath = resolvePath(rel);
    sf::Texture texture;
    if (!texture.loadFromFile(fullpath)) {
        std::cerr << "Could not load powerup texture: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded powerup texture: " << fullpath << std::endl;
    }
    txtPowerup.push_back(texture);
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

// **Font loading method**

void PreloadResources::loadFont(const char *filename) {
    std::string rel = this->fontsFilePath() + filename + ".ttf";
    std::string fullpath = resolvePath(rel);
    sf::Font font;
    if (!font.openFromFile(fullpath)) {
        std::cerr << "Could not load font: " << fullpath << std::endl;
    } else {
        std::cerr << "Loaded font: " << fullpath << std::endl;
        fonts.push_back(font);
    }
    this->setLoadedResourceCount(this->getLoadedResourceCount() + 1);
    this->calculateLoadPercentile();
}

// calculateLoadPercentile, unloadResources, etc.

void PreloadResources::calculateLoadPercentile() {
    size_t loaded = this->getLoadedResourceCount();
    size_t total = this->getResourceCount();
    if (total > 0) {
        float pct = (static_cast<float>(loaded) * 100.f) / static_cast<float>(total);
        this->setLoadPercentile(pct);
    } else {
        this->setLoadPercentile(0.f);
    }
}

void PreloadResources::unloadResources() {
    sound.clear();
    fonts.clear();
    txtAnimation.clear();
    txtBackground.clear();
    txtBlock.clear();
    txtLevel.clear();
    txtPaddle.clear();
    txtPowerup.clear();
}

// Getters

sf::SoundBuffer &PreloadResources::getBufferedSound(int index) {
    return sound.at(index);
}

sf::Font &PreloadResources::getFont(int index) {
    return fonts.at(index);
}

sf::Texture &PreloadResources::getAnimationTexture(int index) {
    return txtAnimation.at(index);
}

sf::Texture &PreloadResources::getBackgroundTexture(int index) {
    return txtBackground.at(index);
}

sf::Texture &PreloadResources::getBlockTexture(int index) {
    return txtBlock.at(index);
}

sf::Texture &PreloadResources::getLevelTexture(int index) {
    return txtLevel.at(index);
}

sf::Texture &PreloadResources::getPaddleTexture(int index) {
    return txtPaddle.at(index);
}

sf::Texture &PreloadResources::getPowerupTexture(int index) {
    return txtPowerup.at(index);
}

// Accessors / Mutators

float PreloadResources::getLoadPercentile() const {
    return loadPercentile;
}

void PreloadResources::setLoadPercentile(float lp) {
    loadPercentile = lp;
}

size_t PreloadResources::getLoadedResourceCount() const {
    return loadedResourceCount;
}

void PreloadResources::setLoadedResourceCount(size_t lrc) {
    loadedResourceCount = lrc;
}

size_t PreloadResources::getResourceCount() const {
    return resourceCount;
}

void PreloadResources::setResourceCount(size_t rc) {
    resourceCount = rc;
}

std::string &PreloadResources::getSoundFileType() {
    return soundFileType;
}

void PreloadResources::setSoundFileType(const char *sft) {
    soundFileType = sft;
}

std::string &PreloadResources::getTextureFileType() {
    return textureFileType;
}

void PreloadResources::setTextureFileType(const char *tft) {
    textureFileType = tft;
}

std::string &PreloadResources::getAnimationDirectoryName() {
    return animationDirectoryName;
}

void PreloadResources::setAnimationDirectoryName(const char *s) {
    animationDirectoryName = s;
}

std::string &PreloadResources::getBackgroundDirectoryName() {
    return backgroundDirectoryName;
}

void PreloadResources::setBackgroundDirectoryName(const char *s) {
    backgroundDirectoryName = s;
}

std::string &PreloadResources::getBlockDirectoryName() {
    return blockDirectoryName;
}

void PreloadResources::setBlockDirectoryName(const char *s) {
    blockDirectoryName = s;
}

std::string &PreloadResources::getLevelDirectoryName() {
    return levelDirectoryName;
}

void PreloadResources::setLevelDirectoryName(const char *s) {
    levelDirectoryName = s;
}

std::string &PreloadResources::getPaddleDirectoryName() {
    return paddleDirectoryName;
}

void PreloadResources::setPaddleDirectoryName(const char *s) {
    paddleDirectoryName = s;
}

std::string &PreloadResources::getPowerupDirectoryName() {
    return powerupDirectoryName;
}

void PreloadResources::setPowerupDirectoryName(const char *s) {
    powerupDirectoryName = s;
}

std::string &PreloadResources::getSoundDirectoryName() {
    return soundDirectoryName;
}

void PreloadResources::setSoundDirectoryName(const char *s) {
    soundDirectoryName = s;
}

std::string &PreloadResources::getTextureDirectoryName() {
    return textureDirectoryName;
}

void PreloadResources::setTextureDirectoryName(const char *s) {
    textureDirectoryName = s;
}

std::string &PreloadResources::getFontDirectoryName() {
    return fontsDirectoryName;
}

void PreloadResources::setFontDirectoryName(const char *s) {
    fontsDirectoryName = s;
}

