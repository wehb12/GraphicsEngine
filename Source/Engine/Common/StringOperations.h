#pragma once

#include <string>

// Concatenate
#define SHADER_PATH(PATH) SHADER_DIR PATH
#define CONTENT_PATH(PATH) CONTENT_DIR PATH
#define TEXTURE_PATH(PATH) CONTENT_DIR "Textures/" PATH

// String Compare
enum class EStringCase
{
	INSENSITIVE = 0,
	SENSITIVE
};

inline bool CompareCharacter(const char& CharacterA, const char& CharacterB, const EStringCase& CaseSensitivity = EStringCase::INSENSITIVE)
{
	return CharacterA == CharacterB || (CaseSensitivity == EStringCase::INSENSITIVE && std::toupper(CharacterA) == std::toupper(CharacterB));
}

inline bool CompareString(const std::string& StringA, const std::string& StringB, const EStringCase& CaseSensitivity = EStringCase::INSENSITIVE)
{
	return ((StringA.size() == StringB.size()) &&
		std::equal(StringA.begin(), StringA.end(), StringB.begin(), 
		[&CaseSensitivity](const char& CharacterA, const char& CharacterB)
		{
			return CompareCharacter(CharacterA, CharacterB, CaseSensitivity);
		}
	));
}
