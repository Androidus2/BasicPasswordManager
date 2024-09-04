#pragma once
#include "External/AES/AES.h"
#include <vector>

class PasswordManager
{
private:
	unsigned char* masterKey;
	unsigned char* key;
	mutable AES aes;
	std::vector<std::string> passwords;
	std::vector<std::string> descriptions;
public:
	PasswordManager(const std::string& masterKey); 
	~PasswordManager();

	void AddPassword(const std::string& password, const std::string& description);
	void ChangePassword(int index, const std::string& password);
	void RemovePassword(int index);
	void RemoveAllPasswords();
	const std::string& GetPassword(int index) const;
	const std::string& GetDescription(int index) const;
	int GetPasswordsCount() const;

	bool SavePasswords(const std::string& path) const;
	bool LoadPasswords(const std::string& path);
	void GenerateKey();
};