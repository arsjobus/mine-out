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

std::vector<sf::SoundBuffer> PreloadResources::sound;
std::vector<sf::Texture> PreloadResources::txtAnimation;
std::vector<sf::Texture> PreloadResources::txtBackground;
std::vector<sf::Texture> PreloadResources::txtBlock;
std::vector<sf::Texture> PreloadResources::txtLevel;
std::vector<sf::Texture> PreloadResources::txtPaddle;
std::vector<sf::Texture> PreloadResources::txtPowerup;

PreloadResources::PreloadResources(void) {
    loadDefaultSettings();
    // Override default settings here if needed
}

PreloadResources::~PreloadResources(void) {
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
    this->setSoundDirectoryName("sfx");
    this->setSoundFileType(".wav");
    this->setTextureFileType(".png");
    this->setLoadPercentile(0.f);
    this->setLoadedResourceCount(0);
    this->setResourceCount(0);
}

/**
 * Returns a full path by resolving inside the .app’s Resources folder first (on macOS),
 * or falling back to executable base path or working directory.
 */
std::string PreloadResources::resolvePath(const std::string &relativePath) {
#ifdef __APPLE__
    // get the bundle’s Resources directory
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
                    // make sure bundleRes ends with slash
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
        // else fall through to fallback
    }
#endif

    // fallback: assume relativePath is correct against “base path”
    // You must implement getExecutableBasePath to return path to resources folder or binary folder
    std::string base = getExecutableBasePath();
    if (!base.empty()) {
        std::string candidate2 = base + relativePath;
        if (fs::exists(candidate2)) {
            return candidate2;
        }
    }

    // last fallback: just try relative path as given
    return relativePath;
}

/**
 * On macOS, you might compute the base path (e.g. binary’s folder or Resources) for fallback.
 * You’ll need to adapt this depending on how you package & run the app.
 */
std::string PreloadResources::getExecutableBasePath() {
#ifdef __APPLE__
    // We can get the path of the running executable
    char buf[PATH_MAX];
    uint32_t bufsize = sizeof(buf);
    if (_NSGetExecutablePath(buf, &bufsize) == 0) {
        // buf is like: /.../YourApp.app/Contents/MacOS/YourAppExecutable
        fs::path exePath(buf);
        // e.g. go up to Resources folder:
        // exePath.parent_path() is Contents/MacOS
        // so go up one to Contents, then into Resources
        fs::path contentDir = exePath.parent_path().parent_path();  // Contents
        fs::path resourcesDir = contentDir / "Resources";
        std::string s = resourcesDir.string();
        if (!s.empty() && s.back() != '/') {
            s.push_back('/');
        }
        return s;
    }
#endif
    // fallback: empty, or working directory
    return std::string();
}

// Helpers to compute the relative sub‑directory paths:

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

// Loading routines (using resolvePath) —

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

void PreloadResources::calculateLoadPercentile() {
    int loaded = this->getLoadedResourceCount();
    int total = this->getResourceCount();
    if (total > 0) {
        float pct = (static_cast<float>(loaded) * 100.f) / static_cast<float>(total);
        this->setLoadPercentile(pct);
    } else {
        this->setLoadPercentile(0.f);
    }
}

void PreloadResources::unloadResources() {
    sound.clear();
    txtAnimation.clear();
    txtBackground.clear();
    txtBlock.clear();
    txtLevel.clear();
    txtPaddle.clear();
    txtPowerup.clear();
}

sf::SoundBuffer &PreloadResources::getBufferedSound(int index) {
    return this->sound[index];
}

sf::Texture &PreloadResources::getAnimationTexture(int index) {
    return this->txtAnimation[index];
}

sf::Texture &PreloadResources::getBackgroundTexture(int index) {
    return this->txtBackground[index];
}

sf::Texture &PreloadResources::getBlockTexture(int index) {
    return this->txtBlock[index];
}

sf::Texture &PreloadResources::getLevelTexture(int index) {
    return this->txtLevel[index];
}

sf::Texture &PreloadResources::getPaddleTexture(int index) {
    return this->txtPaddle[index];
}

sf::Texture &PreloadResources::getPowerupTexture(int index) {
    return this->txtPowerup[index];
}

float PreloadResources::getLoadPercentile() const {
    return this->loadPercentile;
}

void PreloadResources::setLoadPercentile(float lp) {
    this->loadPercentile = lp;
}

size_t PreloadResources::getLoadedResourceCount() const {
    return this->loadedResourceCount;
}

void PreloadResources::setLoadedResourceCount(size_t lrc) {
    this->loadedResourceCount = lrc;
}

size_t PreloadResources::getResourceCount() const {
    return this->resourceCount;
}

void PreloadResources::setResourceCount(size_t rc) {
    this->resourceCount = rc;
}

std::string &PreloadResources::getSoundFileType() {
    return this->soundFileType;
}

void PreloadResources::setSoundFileType(const char *sft) {
    this->soundFileType = sft;
}

std::string &PreloadResources::getTextureFileType() {
    return this->textureFileType;
}

void PreloadResources::setTextureFileType(const char *tft) {
    this->textureFileType = tft;
}

std::string &PreloadResources::getAnimationDirectoryName() {
    return this->animationDirectoryName;
}

void PreloadResources::setAnimationDirectoryName(const char *s) {
    this->animationDirectoryName = s;
}

std::string &PreloadResources::getBackgroundDirectoryName() {
    return this->backgroundDirectoryName;
}

void PreloadResources::setBackgroundDirectoryName(const char *s) {
    this->backgroundDirectoryName = s;
}

std::string &PreloadResources::getBlockDirectoryName() {
    return this->blockDirectoryName;
}

void PreloadResources::setBlockDirectoryName(const char *s) {
    this->blockDirectoryName = s;
}

std::string &PreloadResources::getLevelDirectoryName() {
    return this->levelDirectoryName;
}

void PreloadResources::setLevelDirectoryName(const char *s) {
    this->levelDirectoryName = s;
}

std::string &PreloadResources::getPaddleDirectoryName() {
    return this->paddleDirectoryName;
}

void PreloadResources::setPaddleDirectoryName(const char *s) {
    this->paddleDirectoryName = s;
}

std::string &PreloadResources::getPowerupDirectoryName() {
    return this->powerupDirectoryName;
}

void PreloadResources::setPowerupDirectoryName(const char *s) {
    this->powerupDirectoryName = s;
}

std::string &PreloadResources::getSoundDirectoryName() {
    return this->soundDirectoryName;
}

void PreloadResources::setSoundDirectoryName(const char *s) {
    this->soundDirectoryName = s;
}

std::string &PreloadResources::getTextureDirectoryName(){
    return this->textureDirectoryName;
}

void PreloadResources::setTextureDirectoryName(const char *s) {
    this->textureDirectoryName = s;
}
