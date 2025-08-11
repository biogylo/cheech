#pragma once
// parse lexemes
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

enum TokenType {
    TokenType_UNSET = 0,
    TokenType_OPENING_BRACKET,
    TokenType_CLOSING_BRACKET,
    Tokentype_SEMICOLON,
    TokenType_WORD,
    TokenType_INTEGRAL
};

struct Slice {
    char const* data;
    uint64_t len;
};

struct Integral {
    uint64_t number;
    bool sign;
};

struct Token {
    enum TokenType type;
    union {
        struct Slice WORD_state;
        struct Integral INTEGRAL_state;
    } state;
};

static struct Slice Slice_from_char(const char* the_char) {
    return (struct Slice){.data = the_char, .len = 1};
}

static inline bool Slice_equal(struct Slice a, struct Slice b) {
    if (a.len != b.len) {
        return false;
    }
    if (a.len == 0) {
        return true;
    }
    for (uint32_t i = 0; i < a.len; i++) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }
    return true;
}

static inline bool Token_equal(struct Token a, struct Token b) {
    if (a.type == TokenType_WORD) {
        return (a.type == b.type && Slice_equal(a.state.WORD_state, b.state.WORD_state));
    };
    return a.type == b.type;
}
static inline uint32_t str_len(const char* str) {
    uint32_t len = 0;
    while (true) {
        if (str[len] == 0) {
            return len;
        }
        len++;
    }
}

struct TokenBuf {
    struct Token* buffer;
    uint32_t size;
    uint32_t capacity;
};

static inline void TokenBuf_assert_equal(const struct TokenBuf a, const struct TokenBuf b) {
    assert(a.size == b.size);
    for (uint32_t i = 0; i < a.size; i++) {
        struct Token token_a = a.buffer[i];
        struct Token token_b = b.buffer[i];
        assert(Token_equal(token_a, token_b));
    }
}

static inline struct Slice Slice(const char* inline_str) {
    uint32_t len = str_len(inline_str);
    struct Slice slice = {.data = inline_str, .len = len};
    return slice;
}

static inline struct Token Token_NonWord(enum TokenType type) {
    assert(type != TokenType_WORD);
    struct Token token = {.type = type};
    return token;
}

static inline struct Token Token_Unset(void) {
    return Token_NonWord(TokenType_UNSET);
}
static inline struct Token Token_CB(void) {
    return Token_NonWord(TokenType_CLOSING_BRACKET);
}
static inline struct Token Token_OB(void) {
    return Token_NonWord(TokenType_OPENING_BRACKET);
}
static inline struct Token Token_Word(struct Slice slice) {
    assert(slice.len);
    assert(slice.data);
    struct Token token = {.type = TokenType_WORD, .state = {slice}};
    return token;
}
static inline struct Token Token_Word_Static(const char* str) {
    struct Token token = {.type = TokenType_WORD, .state = {Slice(str)}};
    return token;
}

static inline struct TokenBuf TokenBuf(struct Token* buffer, uint32_t capacity) {
    assert(capacity >= 1);
    for (uint32_t i = 0; i < capacity; i++) {
        buffer[i] = Token_Unset();
    }
    struct TokenBuf token_buf = {.buffer = buffer, .size = 0, .capacity = capacity};
    return token_buf;
}

static inline void TokenBuf_append(struct TokenBuf* token_buf, struct Token token) {
    assert(token_buf->capacity > token_buf->size);
    token_buf->buffer[token_buf->size] = token;
    token_buf->size++;
}

static inline enum TokenType char_as_token(const char the_char) {
    switch (the_char) {
    case ' ':
    case '\t':
    case '\n':
        return TokenType_UNSET;
    case '{':
        return TokenType_OPENING_BRACKET;
    case '}':
        return TokenType_CLOSING_BRACKET;
    case ';':
        return Tokentype_SEMICOLON;
    default:
        return TokenType_WORD;
    }
}

// will scan text and populate outbuff with all tokens found
// NOTE: Slice has to outlive TokenBuf (zero-copy tokenizer)
static inline void scan(const struct Slice text, struct TokenBuf* outbuff) {
    assert(text.data);
    assert(outbuff);
    assert(outbuff->buffer);
    assert(outbuff->capacity);
    assert(outbuff->size == 0);
    assert(text.len < 10000);

#define LEXER_add_word_token(outbuff, word_state)         \
    {                                                     \
        TokenBuf_append(outbuff, Token_Word(word_state)); \
        word_state.data = NULL;                           \
        word_state.len = 0;                               \
    };

    struct Slice WORD_state = {.data = NULL, .len = 0};
    for (uint64_t i = 0; i < text.len; i++) {
        assert(i < text.len);
        const char current = text.data[i];
        enum TokenType state = char_as_token(current);
        if (state != TokenType_WORD) {
            if (WORD_state.data) {
                LEXER_add_word_token(outbuff, WORD_state);
            };
            if (state != TokenType_UNSET) {
                TokenBuf_append(outbuff, Token_NonWord(state));
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
        LEXER_add_word_token(outbuff, WORD_state); // NOLINT
    }
}
