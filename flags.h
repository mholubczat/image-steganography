#ifndef IMAGE_STEGANOGRAPHY_FLAGS_H
#define IMAGE_STEGANOGRAPHY_FLAGS_H

class flags {

public:
    static bool check(const char *&path, const char *&message, bool verbose);

    static void help();

    static void info(const char *&path, bool verbose);

    static void encrypt(const char *&path, const char *&message);

    static void decrypt(const char *&path);
};

#endif //IMAGE_STEGANOGRAPHY_FLAGS_H
