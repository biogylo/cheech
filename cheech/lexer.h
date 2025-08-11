#pragma once
// parse lexemes
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum TokenType{
    TokenType_UNSET = 0,
    TokenType_OPENING_BRACKET,
    TokenType_CLOSING_BRACKET,
    Tokentype_SEMICOLON,
    TokenType_WORD
};

struct Slice {
    char const* data;
    uint64_t len;
};

struct Token{
    enum TokenType type;
    union {
        struct Slice WORD_state;
    } state;
};
static inline bool Token_equal(struct Token a, struct Token b) {
    if (a.type == TokenType_WORD) {
        return (
            a.type == b.type  
                && a.state.WORD_state.data == b.state.WORD_state.data
                && a.state.WORD_state.len == b.state.WORD_state.len
        );
    };
    return a.type == b.type;

};

struct TokenBuf {
    struct Token* buffer;
    uint32_t size;
    const uint32_t capacity;
};

static inline bool is_whitespace(const char the_char) {
    return (
        the_char == ' ' || the_char == '\t' || the_char == '\n'
    );
};

static inline enum TokenType char_as_token(const char the_char) {
    switch (the_char){
        case ' ': case '\t': case '\n': return TokenType_UNSET;
        case '{': return TokenType_OPENING_BRACKET;
        case '}': return TokenType_CLOSING_BRACKET;
        case ';': return Tokentype_SEMICOLON;
        default:  return TokenType_WORD;
    }
};

// will scan text and populate outbuff with all tokens found
// NOTE: Slice has to outlive TokenBuf (zero-copy tokenizer)
static inline void scan(const struct Slice text, struct TokenBuf* outbuff){
    assert(text.data);
    assert(outbuff);
    assert(outbuff->buffer);
    assert(outbuff->capacity);
    assert(outbuff->size == 0);
    assert(text.len < 10000);

    #define LEXER_add_token(outbuff, token) {\
        int32_t token_count = outbuff->size;\
        assert(token_count < outbuff->capacity);\
        outbuff->buffer[token_count] = token;\
        outbuff->size++;\
    }
    #define LEXER_add_word_token(outbuff, word_state) {\
        assert(word_state.len);\
        struct Token token = {\
            .type=TokenType_WORD\
        };\
        token.state.WORD_state = word_state;\
        word_state.data = NULL;\
        word_state.len = 0;\
        LEXER_add_token(outbuff, token);\
    };

    struct Slice WORD_state = {
        .data=NULL,
        .len=0
    };
    for (uint64_t i = 0; i < text.len; i++) {
        assert(i < text.len);
        const char current = text.data[i];
        enum TokenType state = char_as_token(current);
        if (state != TokenType_WORD) {
            if (WORD_state.data) {
                LEXER_add_word_token(outbuff, WORD_state);
            };
            if (state != TokenType_UNSET) {
                struct Token token = {.type=state};
                LEXER_add_token(outbuff, token);
            }
            continue;
        };
        assert(state == TokenType_WORD);
        if (WORD_state.data) {
            WORD_state.len++;
            continue;
        };
        WORD_state.data = &text.data[i];
        WORD_state.len = 1;
    };
    if (WORD_state.data) {
        LEXER_add_word_token(outbuff, WORD_state);
    }
};