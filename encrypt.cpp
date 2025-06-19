#include "encrypt.h"

namespace encrypt{
    std::string trsp_encrypt(const char *text, const std::vector<int> &key){
        int blockSize = key.size();
        std::string cipher_text = text;
    
        while(cipher_text.length() % blockSize != 0){
            cipher_text += 'x';
        }
    
        std::string encrypted_text = cipher_text;
        for(size_t i = 0; i < cipher_text.length(); i += blockSize){
            for(size_t j = 0; j < blockSize; j++){
                encrypted_text[i + j] = cipher_text[i + key[j] - 1];
            }
        }
        return encrypted_text;
    }

    std::string caesar_encrypt(const std::string &encrypted_text, const std::vector<char> &key2){
        std::string cipher = encrypted_text;
        for(size_t i = 0; i < cipher.length(); i++){
            if (cipher[i] >= 'a' && cipher[i] <= 'z') {
                cipher[i] += (3 % 26);
                if (cipher[i] > 'z'){
                    cipher[i] -= 26;
                }
            }
            if (cipher[i] >= 'A' && cipher[i] <= 'Z') {
                cipher[i] += (3 % 26);
                if (cipher[i] > 'Z'){
                    cipher[i] -= 26;
                }
            }
        }
        return cipher;
    }
}