// ~/src/io/console.c

#include "llm/llama_interface.h"
#include "llama.h"  // llama.cpp C API
#include "core/jarvis.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Static context so we can reuse it
static struct llama_context *ctx = NULL;
static struct llama_model *model = NULL;

// You can tune these
#define N_CTX 512
#define MAX_TOKENS 128
#define SEED 1234

int llama_init(const char *model_path) {
    struct llama_model_params model_params = llama_model_default_params();
    model = llama_load_model_from_file(model_path, model_params);
    if (!model) {
        fprintf(stderr, "Failed to load model from: %s\n", model_path);
        return -1;
    }

    struct llama_context_params ctx_params = llama_context_default_params();
    ctx_params.seed = SEED;
    ctx_params.n_ctx = N_CTX;
    ctx = llama_new_context_with_model(model, ctx_params);
    if (!ctx) {
        fprintf(stderr, "Failed to create llama context\n");
        return -1;
    }

    return 0;
}

const char *llama_generate(const char *prompt) {
    static char output[2048]; // not thread-safe; static buffer for return
    output[0] = '\0';

    // Tokenize prompt
    int n_prompt_tokens;
    llama_token prompt_tokens[512];
    n_prompt_tokens = llama_tokenize(ctx, prompt, prompt_tokens, 512, true);

    if (llama_eval(ctx, prompt_tokens, n_prompt_tokens, 0, 1) != 0) {
        fprintf(stderr, "Failed to evaluate prompt\n");
        return NULL;
    }

    for (int i = 0; i < MAX_TOKENS; ++i) {
        llama_token token = llama_sample_token(ctx);
        if (token == llama_token_eos(ctx)) {
            break;
        }

        const char *piece = llama_token_to_str(ctx, token);
        strncat(output, piece, sizeof(output) - strlen(output) - 1);

        if (llama_eval(ctx, &token, 1, n_prompt_tokens + i, 1) != 0) {
            break;
        }
    }

    return output;
}

void llama_cleanup(void) {
    if (ctx) {
        llama_free(ctx);
        ctx = NULL;
    }
    if (model) {
        llama_free_model(model);
        model = NULL;
    }
}
