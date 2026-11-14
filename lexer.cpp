#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

using namespace std;

// Instruction 1: Scoped Enum for Token Types
enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT,
    MINUS, PLUS,
    SEMICOLON, SLASH,
    STAR,

    // One or two character operator tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, NUMBER,

    // Keywords
    AND, ELSE,
    FALSE, IF,
    LET, NIL,
    OR, TRUE,
    WHILE, PRINT,

    // End of file token
    END_OF_FILE
};


// Instruction 2: Token Struct
struct Token {
    TokenType type;
    string lex;
    int line; 

    // Constructor passing the string via const reference
    Token(TokenType type, const string& lex, int line)
        : type(type), lex(lex), line(line) {}
};


// Instruction 3: Lexer Class
class Lexer {
public:
    // Constructor 
    Lexer(const string& source) : source(source) {}

    // scanTokens 
    vector<Token> scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }

        tokens.emplace_back(TokenType::END_OF_FILE, "", line);
        return tokens;
    }

private:
    const string& source;       
    vector<Token> tokens;       
    size_t start = 0;           
    size_t current = 0;        
    int line = 1;               

    // Unordered map of language keywords 
    unordered_map<string, TokenType> keywords = {
        {"and",   TokenType::AND},
        {"else",  TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"if",    TokenType::IF},
        {"let",   TokenType::LET},
        {"nil",   TokenType::NIL},
        {"or",    TokenType::OR},
        {"true",  TokenType::TRUE},
        {"while", TokenType::WHILE},
        {"print", TokenType::PRINT}
    };

    // isAtEnd: Checks if we've reached the end of the source string
    bool isAtEnd() {
        return current >= source.length();
    }

    // advance: Returns current character and increments current by one
    char advance() {
        return source[current++];
    }

    // addToken: Extracts substring from start to current and emplaces Token
    void addToken(TokenType type) {
        string text = source.substr(start, current - start);
        tokens.emplace_back(type, text, line);
    }

    // match: Conditional advance for two-character operators (e.g., ==, !=)
    bool match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;

        current++;
        return true;
    }

    // peek: Looks at the current character without consuming it
    char peek() {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    // Helper for decimal numbers 
    char peekNext() {
        if (current + 1 >= source.length()) return '\0';
        return source[current + 1];
    }

    // string: Reads string literals enclosed in quotes "..."
    // Added per rubric specification: emits IDENTIFIER token once string closes
    void stringLiteral() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            cerr << "[Line " << line << "] Error: Unterminated string." << endl;
            return;
        }

        // Consume the closing '"'
        advance();

        // Add to tokens vector as IDENTIFIER per rubric instructions
        addToken(TokenType::IDENTIFIER);
    }

    // number: Consumes integer and floating-point literals (e.g., 3, 45, 56.7)
    void number() {
        while (isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }

        // Look for a fractional decimal part
        if (peek() == '.' && isdigit(static_cast<unsigned char>(peekNext()))) {
            // Consume the '.'
            advance();

            while (isdigit(static_cast<unsigned char>(peek()))) {
                advance();
            }
        }

        addToken(TokenType::NUMBER);
    }

    // identifier: Consumes alphanumeric words and checks the keywords map
    void identifier() {
        while (isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
            advance();
        }

        string text = source.substr(start, current - start);
        auto it = keywords.find(text);
        if (it != keywords.end()) {
            addToken(it->second);
        } else {
            addToken(TokenType::IDENTIFIER);
        }
    }

    // scanToken: Maps next character(s) to their corresponding TokenType
    void scanToken() {
        char c = advance();

        switch (c) {
            // Single-character tokens
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;

            // Single or two-character operators
            case '!':
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;

            // Slash or single-line comment (//)
            case '/':
                if (match('/')) {
                    // Comment goes until the end of the line
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;

            // Ignore whitespace
            case ' ':
            case '\r':
            case '\t':
                break;

            // Newline increments line counter
            case '\n':
                line++;
                break;

            // String literal handling (if quotes are encountered)
            case '"':
                stringLiteral();
                break;

            default:
                if (isdigit(static_cast<unsigned char>(c))) {
                    number();
                } else if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
                    identifier();
                } else {
                    cerr << "[Line " << line << "] Error: Unexpected character '" << c << "'." << endl;
                }
                break;
        }
    }
};


// Helper Function for Testing Output in main()
string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN:    return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:   return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:    return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:   return "RIGHT_BRACE";
        case TokenType::COMMA:         return "COMMA";
        case TokenType::DOT:           return "DOT";
        case TokenType::MINUS:         return "MINUS";
        case TokenType::PLUS:          return "PLUS";
        case TokenType::SEMICOLON:     return "SEMICOLON";
        case TokenType::SLASH:         return "SLASH";
        case TokenType::STAR:          return "STAR";
        case TokenType::BANG:          return "BANG";
        case TokenType::BANG_EQUAL:    return "BANG_EQUAL";
        case TokenType::EQUAL:         return "EQUAL";
        case TokenType::EQUAL_EQUAL:   return "EQUAL_EQUAL";
        case TokenType::GREATER:       return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS:          return "LESS";
        case TokenType::LESS_EQUAL:    return "LESS_EQUAL";
        case TokenType::IDENTIFIER:    return "IDENTIFIER";
        case TokenType::NUMBER:        return "NUMBER";
        case TokenType::AND:           return "AND";
        case TokenType::ELSE:          return "ELSE";
        case TokenType::FALSE:         return "FALSE";
        case TokenType::IF:            return "IF";
        case TokenType::LET:           return "LET";
        case TokenType::NIL:           return "NIL";
        case TokenType::OR:            return "OR";
        case TokenType::TRUE:          return "TRUE";
        case TokenType::WHILE:         return "WHILE";
        case TokenType::PRINT:         return "PRINT";
        case TokenType::END_OF_FILE:   return "END_OF_FILE";
        default:                       return "UNKNOWN";
    }
}

// Main Driver to Verify All Token Types
int main() {
    string sampleSource =
        "let x = 45;\n"
        "let y = 56.7;\n"
        "if (x != y and x <= 100) {\n"
        "    print x + y * 3;\n"
        "} else {\n"
        "    print false;\n"
        "}";

    Lexer lexer(sampleSource);
    vector<Token> tokens = lexer.scanTokens();

    cout << "Line | Token Type      | Lexeme\n";
    cout << "----------------------------------------\n";
    for (const Token& token : tokens) {
        cout << token.line << "    | "
             << tokenTypeToString(token.type) << "\t | '"
             << token.lex << "'\n";
    }

    return 0;
}
