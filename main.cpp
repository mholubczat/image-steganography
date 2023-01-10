#include <iostream>
#include <cstring>
#include <sys/stat.h>

using namespace std;

void info(const char *&path);

void encrypt(const char *&path, const char *&message);

void decrypt(const char *&path);

void check(const char *&path, const char *&message);

void help();

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
    if (!path || !message) throw invalid_argument(missingParameters);
    string extension = tryGetExtension(path);
    cout << "Encrypt, path: " << path << " message: " << message << endl;
}

void decrypt(const char *&path) {
    if (!path) throw invalid_argument(missingParameters);
    const string extension = tryGetExtension(path);
    cout << "Decrypt, path:" << path << endl;
}

void check(const char *&path, const char *&message) {
    if (!path || !message) throw invalid_argument(missingParameters);
    string extension = tryGetExtension(path);
    cout << "Check, path: " << path << " message: " << message << endl;
}

void help() {
    cout << "Help" << endl;
}
