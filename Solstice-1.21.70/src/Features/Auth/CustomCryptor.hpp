//
// Created by alteik on 31/10/2024.
//

class CustomCryptor {
public:
    CustomCryptor(unsigned long publicKey, unsigned long privateKey, unsigned long modulus)
        : e(publicKey), d(privateKey), n(modulus) {}

    std::string encrypt(const std::string& text) const {
        std::string encrypted;
        for (char c : text) {
            unsigned long encryptedChar = modular_pow(c, e, n);
            encrypted += std::to_string(encryptedChar) + " ";
        }
        return encrypted;
    }

    std::string decrypt(const std::string& text) const {
        std::istringstream iss(text);
        std::string decrypted;
        unsigned long encryptedChar;
        while (iss >> encryptedChar) {
            char decryptedChar = static_cast<char>(modular_pow(encryptedChar, d, n));
            decrypted += decryptedChar;
        }
        return decrypted;
    }

private:
    unsigned long e;
    unsigned long d;
    unsigned long n;

    unsigned long modular_pow(unsigned long base, unsigned long exponent, unsigned long mod) const {
        unsigned long result = 1;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exponent /= 2;
        }
        return result;
    }
};