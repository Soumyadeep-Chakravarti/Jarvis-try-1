#include <stdio.h>
#include <string.h>

#include "io/console.h"

#define DEFAULT_MODEL_PATH "models/default-model.gguf"

void print_usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s -c [-m model_path]\n", prog);
    printf("    -c                Run console (CLI) mode\n");
    printf("    -m model_path     Specify model path (optional, uses default if omitted)\n");
}

int main(int argc, char *argv[]) {
    const char *model_path = DEFAULT_MODEL_PATH;
    int console_mode = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            console_mode = 1;
        } else if (strcmp(argv[i], "-m") == 0) {
            if (i + 1 < argc) {
                model_path = argv[++i];
            } else {
                fprintf(stderr, "Error: -m requires a model path argument\n");
                print_usage(argv[0]);
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0) {
	    print_usage
	} else{
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    if (console_mode) {
        start_console_interface(model_path);
    } else {
        print_usage(argv[0]);
        return 1;
    }

    return 0;
}

