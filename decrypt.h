#ifndef DECRYPT_H
#define DECRYPT_H
#include <string>
#include <vector>

namespace decrypt{
    std::string trsp_decrypt(const std::string &decrypted, const std::vector<int> &key);
    //std::string trsp_decrypt(const char *text, const std::vector<int> &key);
    //std::string decrypted_caesar(const std::string &cipher, const std::vector<char> &key2);
    std::string decrypted_caesar(const char *text, const std::vector<char> &key2);
}
#endif 