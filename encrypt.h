#ifndef ENCRYPT_H
#define ENCRYPT_H
#include <vector>
#include <string>

namespace encrypt{
    std::string trsp_encrypt(const char *text, const std::vector<int> &key);
    std::string caesar_encrypt(const std::string &encrypted_text, const std::vector<char> &key2);
}
#endif //ENCRYPT_H