// ~/src/include/llm/llama_interface.hy

#ifndef LLM_LLAMA_INTERFACE_H
#define LLM_LLAMA_INTERFACE_H

#include <stddef.h>

// Initialize the LLaMA model
int llama_init(const char *model_path);

// Generate a response given a prompt
const char *llama_generate(const char *prompt);

// Free memory used internally
void llama_cleanup(void);

#endif // LLM_LLAMA_INTERFACE_H

