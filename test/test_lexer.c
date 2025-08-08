#include <assert.h>
#include <string.h>

#include <cheech/lexer.h>

static inline void single_token() {
    struct Token buffer[16];
    struct TokenBuf token_buf = {
        .buffer=buffer,
        .capacity=sizeof(buffer)/sizeof(struct Token),
        .size=0
    };
    struct Scenario {
        char the_char;
        enum TokenType expected_token;
        int expected_token_count;
    };
    struct Scenario scenarios[] = {
        {.the_char='{', .expected_token = TokenType_OPENING_BRACKET, .expected_token_count=1},
        {.the_char='}', .expected_token = TokenType_CLOSING_BRACKET, .expected_token_count=1},
        {.the_char=' ',  .expected_token = TokenType_UNSET, .expected_token_count=0},
        {.the_char='A',  .expected_token = TokenType_WORD, .expected_token_count=1},
        {.the_char='b',  .expected_token = TokenType_WORD, .expected_token_count=1},
        {.the_char='7',  .expected_token = TokenType_WORD, .expected_token_count=1},
    };
    for (int i = 0; i<sizeof(scenarios)/sizeof(struct Scenario); i++) {
        buffer[0].type = TokenType_UNSET;
        struct Scenario scenario = scenarios[i];
        token_buf.size = 0;
        scan((struct Slice){.data=&scenario.the_char,.len=1}, &token_buf);
        assert(token_buf.size == scenario.expected_token_count);
        assert(token_buf.buffer[0].type == scenario.expected_token);
    };
};

static inline void assert_TokenBuf_equal(const struct TokenBuf a, const struct  TokenBuf b){
    assert(a.size == b.size);
    for (int i = 0; i < a.size; i++) {
        assert(Token_equal(a.buffer[i], b.buffer[i]));
    };
};

static inline void multi_token() {
    struct Token buffer[16];
    struct TokenBuf token_buf = {
        .buffer=buffer,
        .capacity=sizeof(buffer)/sizeof(struct Token),
        .size=0
    };
    struct Scenario {
        char* the_chars;
        enum TokenType expected_token;
    };
    
    struct TokenBuf expected_token_buf = {
        .buffer=buffer,
        .capacity=sizeof(buffer)/sizeof(struct Token),
        .size=0
    };
    // TODO!

    assert_TokenBuf_equal(token_buf, expected_token_buf);
};

int main() {
    single_token();
    multi_token();
};