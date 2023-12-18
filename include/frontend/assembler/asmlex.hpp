#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <iostream>
#include <filesystem>
#include <fstream>

namespace front::assembler {

class AsmLex final : public yyFlexLexer {
public:
    enum LexemType {
        NUMBER,
        IDENTIFIER,
        COMA,
        UNKNOWN
    };

    class Lexem {
    public:
        LexemType type;
        virtual ~Lexem() = 0;
    };
    class Number : Lexem {
    public:
        uint64_t value;
        ~Number() override {}
    };
    class Identifier : Lexem {
    public:
        std::string value;
        ~Identifier() override {}
    };
    class Coma : Lexem {
    public:
        ~Coma() override {}
    };
    class Unknown : Lexem {
    public:
        ~Unknown() override {}
    };

    AsmLex(const std::filesystem::path &paht);
    int yylex() override;

private:
    int processDigit() {
        currentValue_ = yytext;
        return 1;
    }
    int processIdentifier() {
        currentValue_ = yytext;
        return 1;
    }
    int processUnknown() {
        return 1;
    }

    std::string currentValue_;
    std::ifstream inputFile_;
};

AsmLex::AsmLex(const std::filesystem::path &path) : inputFile_(path) {}

}
