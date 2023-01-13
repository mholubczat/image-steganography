#ifndef IMAGE_STEGANOGRAPHY_UTIL_H
#define IMAGE_STEGANOGRAPHY_UTIL_H

using namespace std;

class util {
public:
    static string tryGetExtension(const char *&path);

    static void readBmpHeader(ifstream &file, long &offset, size_t &width, size_t &height, int &bitPerPixel);

    static void readPpmHeader(ifstream &file, long &offset, size_t &width, size_t &height, bool &isAscii);

    static size_t readAsciiInt(ifstream &file);

    static void reWriteAsciiInt(ifstream &file, ofstream &newFile);
};

#endif //IMAGE_STEGANOGRAPHY_UTIL_H
