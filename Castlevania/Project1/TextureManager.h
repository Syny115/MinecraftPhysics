#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
using namespace std;

class TextureManager {
private:
	static unordered_map<string, Texture2D>& getCache() {
		static unordered_map<string, Texture2D> cache;
	return cache;
	}
public:
	static TextureManager& Get() {
		static TextureManager instance;
		return instance;
	}

	Texture2D& Load(const string& path) {
		auto& cache = getCache();
		if (cache.find(path) == cache.end()) {
			Image raw = LoadImage(path.c_str());
			Color target = { 119, 51, 153, 255 };
			ImageColorReplace(&raw, target, BLANK);
			cache[path] = LoadTextureFromImage(raw);
			UnloadImage(raw);
		}
		return cache[path];
	}

	void UnloadAll() {
		auto& cache = getCache();
		for (pair<string, Texture2D> p : cache)
		{
			UnloadTexture(p.second);
		}
		cache.clear();
	}
};