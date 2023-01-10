#include <iostream>
#include <fstream>
#include <cstring>

#include <sys/stat.h>
#include <vector>

#define BIT_ONE 0x01
#define BIT_TWO 0x02
#define BIT_THREE 0x04
#define BIT_FOUR 0x08
#define BIT_FIVE 0x10
#define BIT_SIX 0x20
#define BIT_SEVEN 0x40
#define BIT_EIGHT 0x80

using namespace std;

void info(const char *&path);

void encrypt(const char *&path, const char *&message);

void decrypt(const char *&path);

void check(const char *&path, const char *&message);

void help();

void printBits(char c) {
    cout << ((c & BIT_EIGHT) >> 7) << ((c & BIT_SEVEN) >> 6) << ((c & BIT_SIX) >> 5)
         << ((c & BIT_FIVE) >> 4) << ((c & BIT_FOUR) >> 3) << ((c & BIT_THREE) >> 2)
         << ((c & BIT_TWO) >> 1) << ((c & BIT_ONE)) << endl;
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; i++) {
        if (i == 0) {
            if (argc == 1) help();
            continue;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
            continue;
        }
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0) {
            const char *path = argv[++i];
            info(path);
            continue;
        }
        if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encrypt") == 0) {
            const char *path = argv[++i];
            const char *message = argv[++i];
            encrypt(path, message);
            continue;
        }
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decrypt") == 0) {
            const char *path = argv[++i];
            decrypt(path);
            continue;
        }
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--check") == 0) {
            const char *path = argv[++i];
            const char *message = argv[++i];
            check(path, message);
            continue;
        }
        string invalidOption = "Invalid option \"" + string(argv[i]) + "\". Enter -h for more information";
        throw runtime_error(invalidOption);
    }
    return 0;
}

const string missingParameters = "Missing parameter(s). Enter -h for more information.";

#ifdef WIN32
#define stat _stat
#endif

string tryGetExtension(const char *path) {
    string extension;
    if (strlen(path) < 4 || (extension = string(path).substr(strlen(path) - 4, 4)) != ".bmp") {
        throw invalid_argument("File not supported.");
    }
    return extension;
}

void info(const char *&path) {
    if (!path) throw invalid_argument(missingParameters);

    struct stat fileStats{};
    if (stat(path, &fileStats) != 0) throw invalid_argument("File not found.");

    cout << "Extension:" << '\t' << tryGetExtension(path) << endl;
    cout << "Size:\t" << '\t' << fileStats.st_size << endl;
    cout << "Size on disk: " << '\t' << fileStats.st_blocks * 512 << endl;
    cout << "Last modified: " << '\t' << fileStats.st_mtime << endl;
}

void encrypt(const char *&path, const char *&message) {
    clock_t start = clock();
    if (!path || !message) throw invalid_argument(missingParameters);
    string extension = tryGetExtension(path);
    string myMessage = string(message).append("/END");
    cout << "Encrypt, path: " << path << " message: " << message << endl;
    if (extension == ".bmp") {
        fstream file(path, ios::in | ios::out | ios::binary);
        file.seekg(0, ios::end);
        long size = file.tellg();
        int offset = 0;
        int bitPerPixel = 0;
        int biWidth = 0;
        int biHeight = 0;
        // READ HEADER DATA
        file.seekg(10, ios::beg);
        for (int i = 0; i < 4; i++) {
            offset += (file.get() << 8 * i);
        }
        file.seekg(28, ios::beg);
        for (int i = 0; i < 2; i++) {
            bitPerPixel += (file.get() << 8 * i);
        }
        file.seekg(18, ios::beg);
        for (int i = 0; i < 4; i++) {
            biWidth += (file.get() << 8 * i);
        }
        file.seekg(22, ios::beg);
        for (int i = 0; i < 4; i++) {
            biHeight += (file.get() << 8 * i);
        }
        string newFilePath = string(path).substr(0, strlen(path) - 4).append(" encrypted.bmp");
        ofstream newFile(newFilePath, ios::binary);
        file.seekg(0, ios::beg);
        vector<int> bits = {BIT_ONE, BIT_TWO, BIT_THREE, BIT_FOUR, BIT_FIVE, BIT_SIX, BIT_SEVEN, BIT_EIGHT};
        size_t msgCounter = 0;
        char c;
        file.seekg(0, ios::beg);
        for (int i = 0; i < size; i++) {
            if (msgCounter >= myMessage.length() || i < offset) {
                c = file.get();
                newFile.put(c);
            } else {
                for (; msgCounter < myMessage.length(); msgCounter++) {
                    for (uint8_t b: bits) {
                        c = ((file.get() & (~b)) + (myMessage[msgCounter] & b));
                        newFile.put(c);
                        for (int j = 1; j < bitPerPixel / 8; j++) {
                            newFile.put(file.get());
                            i++;
                        }
                    }
                }
            }
        }
        newFile.close();
        file.close();
        clock_t stop = clock();
        double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
        printf("\nTime elapsed: %.5f\n", elapsed);
    }
    if (extension == ".png") {

    }
}

void decrypt(const char *&path) {
    clock_t start = clock();
    if (!path) throw invalid_argument(missingParameters);
    const string extension = tryGetExtension(path);
    cout << "Decrypt, path:" << path << endl;
    if (extension == ".bmp") {
        fstream file(path, ios::in | ios::binary);
        file.seekg(0, ios::end);
        long size = file.tellg();
        int offset = 0;
        int bitPerPixel = 0;
        int biWidth = 0;
        int biHeight = 0;
        // READ HEADER DATA
        file.seekg(10, ios::beg);
        for (int i = 0; i < 4; i++) {
            offset += (file.get() << 8 * i);
        }
        file.seekg(28, ios::beg);
        for (int i = 0; i < 2; i++) {
            bitPerPixel += (file.get() << 8 * i);
        }
        file.seekg(18, ios::beg);
        for (int i = 0; i < 4; i++) {
            biWidth += (file.get() << 8 * i);
        }
        file.seekg(22, ios::beg);
        for (int i = 0; i < 4; i++) {
            biHeight += (file.get() << 8 * i);
        }
        char c = 0;
        char *message = new char[size];
        int currOffset = offset - 3;
        vector<char8_t> bits = {BIT_ONE, BIT_TWO, BIT_THREE, BIT_FOUR, BIT_FIVE, BIT_SIX, BIT_SEVEN,
                                BIT_EIGHT};

        for (int i = 0; i < size / bitPerPixel; i++) {
            for (char8_t b: bits) {
                file.seekg(currOffset += bitPerPixel / 8, ios::beg);
                c = file.get();
                message[i] += (c & b);
            }
            if (message[i] == 'D') {
                if (string(message).substr(strlen(message)-4,4) == "/END") {
                    break;
                }
            }
        }
        cout << "Decrypted message: " << string(message).erase(strlen(message) - 4) << endl;
        delete[] message;
        file.close();
        clock_t stop = clock();
        double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
        printf("\nTime elapsed: %.5f\n", elapsed);
    }
}

void check(const char *&path, const char *&message) {
    if (!path || !message) throw invalid_argument(missingParameters);
    string extension = tryGetExtension(path);
    cout << "Check, path: " << path << " message: " << message << endl;
}

void help() {
    cout << "Help" << endl;
}

