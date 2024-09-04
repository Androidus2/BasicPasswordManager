#include "PasswordManager.h"
#include <fstream>

PasswordManager::PasswordManager(const std::string& masterKey)
{
	this->masterKey = new unsigned char[32];
	for (int i = 0; i < 32; i++)
		this->masterKey[i] = masterKey[i % masterKey.size()]; // Repeat master key if it's shorter than 32 bytes

	key = nullptr;
}
PasswordManager::~PasswordManager()
{
	if(masterKey)
		delete[] masterKey;
	if(key)
		delete[] key;
}

void PasswordManager::AddPassword(const std::string& password, const std::string& description)
{
	passwords.push_back(password);
	descriptions.push_back(description);
}
void PasswordManager::ChangePassword(int index, const std::string& password)
{
	if (index >= 0 && index < passwords.size())
		passwords[index] = password;
}
void PasswordManager::RemovePassword(int index)
{
	if (index >= 0 && index < passwords.size())
		passwords.erase(passwords.begin() + index);
}
void PasswordManager::RemoveAllPasswords()
{
	passwords.clear();
	descriptions.clear();
}
const std::string& PasswordManager::GetPassword(int index) const
{
	if (index >= 0 && index < passwords.size())
		return passwords[index];
}
const std::string& PasswordManager::GetDescription(int index) const
{
	if (index >= 0 && index < descriptions.size())
		return descriptions[index];
}
int PasswordManager::GetPasswordsCount() const
{
	return passwords.size();
}

bool PasswordManager::SavePasswords(const std::string& path) const
{
	if (key)
	{
		std::ofstream outputFile(path, std::ios::binary);
		if (outputFile.is_open())
		{
			// Encrypt the key with the master key
			unsigned char* encryptedKey = aes.EncryptECB(key, 32, masterKey);
			outputFile.write((char*)encryptedKey, 32);
			delete[] encryptedKey;

			// Write the number of passwords
			unsigned int numPasswords = passwords.size();
			outputFile.write((char*)&numPasswords, sizeof(unsigned int));

			std::cout << "GOT TO FOR LOOP\n";
			// Encrypt the passwords with the key
			for (int i=0; i<passwords.size(); i++)
			{
				const std::string& password = passwords[i];
				unsigned int length = password.length();
				unsigned int padding = 16 - (length % 16);
				unsigned char* data = new unsigned char[length + padding];
				for (unsigned int j = 0; j < length; j++)
					data[j] = password[j];
				for (unsigned int j = 0; j < padding; j++)
					data[length + j] = (unsigned char)65; // Padding with 'A'
				unsigned char* encryptedPassword = aes.EncryptECB(data, length + padding, key);

				// Write the length of the password
				outputFile.write((char*)&length, sizeof(unsigned int));
				// Write the encrypted password
				outputFile.write((char*)encryptedPassword, length + padding);

				delete[] data;
				delete[] encryptedPassword;

				std::cout<<"GOT TO DESCRIPTIONS LENGTH\n";

				// Write the length of the description
				length = descriptions[i].length();
				outputFile.write((char*)&length, sizeof(unsigned int));

				std::cout<<"GOT TO DESCRIPTIONS\n";

				// Encrypt the description with the key
				length = descriptions[i].length();
				padding = 16 - (length % 16);
				data = new unsigned char[length + padding];
				for (unsigned int j = 0; j < length; j++)
					data[j] = descriptions[i][j];
				for (unsigned int j = 0; j < padding; j++)
					data[length + j] = (unsigned char)65; // Padding with 'A'
				unsigned char* encryptedDescription = aes.EncryptECB(data, length + padding, key);

				// Write the encrypted description
				outputFile.write((char*)encryptedDescription, length + padding);

				delete[] data;
				delete[] encryptedDescription;

				std::cout<<"GOT TO END OF FOR LOOP\n";
			}
			return true;
		}
	}
	return false;
}
bool PasswordManager::LoadPasswords(const std::string& path)
{
	std::ifstream inputFile(path, std::ios::binary);
	if (inputFile.is_open())
	{
		// Decrypt the key with the master key
		unsigned char* encryptedKey = new unsigned char[32];
		inputFile.read((char*)encryptedKey, 32);
		key = aes.DecryptECB(encryptedKey, 32, masterKey);
		delete[] encryptedKey;

		// Read the number of passwords
		unsigned int numPasswords;
		inputFile.read((char*)&numPasswords, sizeof(unsigned int));

		// Decrypt the passwords with the key
		for (int i = 0; i < numPasswords; i++)
		{
			// Read the length of the password
			unsigned int length;
			inputFile.read((char*)&length, sizeof(unsigned int));
			unsigned int padding = 16 - (length % 16);
			unsigned char* encryptedPassword = new unsigned char[length + padding];
			inputFile.read((char*)encryptedPassword, length + padding);
			unsigned char* data = aes.DecryptECB(encryptedPassword, length + padding, key);

			std::string password;
			for (unsigned int j = 0; j < length; j++)
				password += data[j];
			passwords.push_back(password);

			delete[] data;
			delete[] encryptedPassword;

			// Read the length of the description
			inputFile.read((char*)&length, sizeof(unsigned int));
			padding = 16 - (length % 16);
			encryptedPassword = new unsigned char[length + padding];
			inputFile.read((char*)encryptedPassword, length + padding);
			data = aes.DecryptECB(encryptedPassword, length + padding, key);

			std::string description;
			for (unsigned int j = 0; j < length; j++)
				description += data[j];
			descriptions.push_back(description);

			delete[] data;
			delete[] encryptedPassword;
		}
		return true;
	}
	return false;
}
void PasswordManager::GenerateKey()
{
	if (key)
		delete[] key;
	key = new unsigned char[32];
	for (int i = 0; i < 32; i++)
		key[i] = rand() % 256;
}