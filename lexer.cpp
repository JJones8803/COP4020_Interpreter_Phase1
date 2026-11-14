Enum class TokenType {
// single char tokens 
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

// single or two operator tokens 
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, 
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

// literals tokens 
    IDENTIFIER, NUMBER,

// keywords 
    AND, ELSE, FALSE, IF, LET, NIL, OR, TRUE, WHILE, PRINT,

// end of file token
    END_OF_FILE
};    

struct Token {
TokenType type; 
string lex;
int num;

// struct constructor 
Token(TokenType type, const string& lex, int num) : type(type), lex(lex), num(num) {}
};

int main() {

};
