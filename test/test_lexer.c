// Copyright 2025 Juan Navarro

#include <assert.h>

#include <cheech/lexer.h>

#define SCENARIO_COUNT (6)
#define BUFFER_SIZE (16)

static inline void single_token(void) {
    struct Token buffer[BUFFER_SIZE];
    struct TokenBuf token_buf = {.buffer = buffer, .capacity = BUFFER_SIZE, .size = 0};
    struct Scenario {
        char the_char;
        enum TokenType expected_token;
        uint32_t expected_token_count;
    };
    struct Scenario scenarios[SCENARIO_COUNT] = {
        {.the_char = '{', .expected_token = TokenType_OPENING_BRACKET, .expected_token_count = 1},
        {.the_char = '}', .expected_token = TokenType_CLOSING_BRACKET, .expected_token_count = 1},
        {.the_char = ' ', .expected_token = TokenType_UNSET, .expected_token_count = 0},
        {.the_char = 'A', .expected_token = TokenType_WORD, .expected_token_count = 1},
        {.the_char = 'b', .expected_token = TokenType_WORD, .expected_token_count = 1},
        {.the_char = '7', .expected_token = TokenType_WORD, .expected_token_count = 1},
    };
    assert(sizeof(scenarios) / sizeof(struct Scenario) == SCENARIO_COUNT);
    for (uint32_t i = 0; i < sizeof(scenarios) / sizeof(struct Scenario); i++) {
        struct Scenario scenario = scenarios[i];
        token_buf.buffer[0].type = TokenType_UNSET;
        token_buf.size = 0;
        scan((struct Slice){.data = &scenario.the_char, .len = 1}, &token_buf);
        assert(token_buf.size == scenario.expected_token_count);
        assert(token_buf.buffer[0].type == scenario.expected_token);
    }
}

static inline void multi_token(void) {
    struct Token expected_buffer[BUFFER_SIZE];
    struct TokenBuf expected_token_buf;

    expected_token_buf = TokenBuf(expected_buffer, BUFFER_SIZE);

    TokenBuf_append(&expected_token_buf, Token_OB());
    TokenBuf_append(&expected_token_buf, Token_Word_Static("w"));
    TokenBuf_append(&expected_token_buf, Token_CB());

    struct Token buffer[BUFFER_SIZE];
    struct TokenBuf token_buf;
    token_buf = TokenBuf(buffer, BUFFER_SIZE);
    scan(Slice("{w}"), &token_buf);
    TokenBuf_assert_equal(token_buf, expected_token_buf);

    token_buf = TokenBuf(buffer, BUFFER_SIZE);
    scan(Slice("{w }"), &token_buf);
    TokenBuf_assert_equal(token_buf, expected_token_buf);

    token_buf = TokenBuf(buffer, BUFFER_SIZE);
    scan(Slice("{ w}"), &token_buf);
    TokenBuf_assert_equal(token_buf, expected_token_buf);

    token_buf = TokenBuf(buffer, BUFFER_SIZE);
    scan(Slice("{ w }"), &token_buf);
    TokenBuf_assert_equal(token_buf, expected_token_buf);

    token_buf = TokenBuf(buffer, BUFFER_SIZE);
    scan(Slice(" {  w  } "), &token_buf);
    TokenBuf_assert_equal(token_buf, expected_token_buf);

    token_buf = TokenBuf(buffer, BUFFER_SIZE);
    scan(Slice(" \t\n{  w \n}\n"), &token_buf);
    TokenBuf_assert_equal(token_buf, expected_token_buf);
}

int main(void) {
    single_token();
    multi_token();
}
