#include "decrypt.h"

namespace decrypt{
    
    std::string trsp_decrypt(const std::string &decrypted, const std::vector<int> &key){
        int blockSize = key.size();
        std::string decrypted_text = decrypted;
    
        std::vector<int> inverse_key(blockSize);
        for(size_t i = 0; i < blockSize; i++){
            inverse_key[key[i] - 1] = i + 1;
        }
    
        for(size_t i = 0; i < decrypted.length(); i += blockSize){
            for(size_t j = 0; j < blockSize; j++){
                decrypted_text[i + j] = decrypted[i + inverse_key[j] - 1];
            }
        }
    
        while(!decrypted_text.empty() && decrypted_text.back() == 'x'){
            decrypted_text.pop_back();
        }
        return decrypted_text;
    }
    
    std::string decrypted_caesar(const char *text, const std::vector<char> &key2){
        std::string decrypted = text;
        for (size_t i = 0; i < decrypted.length(); i++) {
            if (decrypted[i] >= 'a' && decrypted[i] <= 'z') {
                decrypted[i] -= (3 % 26);
                if (decrypted[i] < 'a'){
                    decrypted[i] += 26;
                }
            } 
            else if (decrypted[i] >= 'A' && decrypted[i] <= 'Z') {
                decrypted[i] -= (3 % 26);
                if (decrypted[i] < 'A'){
                    decrypted[i] += 26;
                }
            }
        }
        return decrypted;
    }
}