#ifndef IMAGE_STEGANOGRAPHY_ENCRYPT_H
#define IMAGE_STEGANOGRAPHY_ENCRYPT_H


class encrypt {
    static void printBits(char c);

public:
    static void encryptAsciiPpm(const std::string &myMessage, std::ofstream &newFile, std::ifstream &file, size_t offset);

    static void encryptRawPpm(const std::string &myMessage, std::ofstream &newFile, std::ifstream &file, size_t offset);

    static void encryptBmp(const std::string &myMessage, std::ofstream &newFile, std::ifstream &file, size_t offset,
                    size_t bitPerPixel);

    static char* decryptAsciiPpm(std::ifstream &file, size_t width, size_t height, long offset);

    static char* decryptRawPpm(std::ifstream &file, size_t width, size_t height, long offset);

    static char* decryptBmp(std::ifstream &file, size_t width, size_t height, long offset, int bitPerPixel);
};

#endif //IMAGE_STEGANOGRAPHY_ENCRYPT_H
