#include <iostream>
#include <cstring>
#include "flags.h"

using namespace std;

int main(int argc, char *argv[]) {
    clock_t start = clock();

    for (int i = 0; i < argc; i++) {
        if (i == 0) {
            if (argc == 1) flags::help();
            continue;
        }
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--info") == 0) {
            const char *path = argv[++i];
            flags::info(path, true);
            continue;
        }
        if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encrypt") == 0) {
            const char *path = argv[++i];
            const char *message = argv[++i];
            flags::encrypt(path, message);
            continue;
        }
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decrypt") == 0) {
            const char *path = argv[++i];
            flags::decrypt(path);
            continue;
        }
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--check") == 0) {
            const char *path = argv[++i];
            const char *message = argv[++i];
            flags::check(path, message, true);
            continue;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            flags::help();
            continue;
        }
        const string invalidOption = "Invalid option \"" + string(argv[i]) + "\". Enter -h for more information";
        throw runtime_error(invalidOption);
    }

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}











