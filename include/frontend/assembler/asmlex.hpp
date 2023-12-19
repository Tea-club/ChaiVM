#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>

namespace front::assembler {

class AsmLex final : public yyFlexLexer {
public:
    enum LexemType { INTEGER, FLOAT, IDENTIFIER, COMMA, UNKNOWN };

    class Lexem {
    public:
        Lexem(LexemType t) : type(t) {}
        LexemType type;
        virtual ~Lexem() {}
    };
    class Int final : public Lexem {
    public:
        Int(LexemType t, int64_t v) : Lexem(t), value(v) {}
        int64_t value;
        ~Int() override {}
    };
    class Float final : public Lexem {
    public:
        Float(LexemType t, double v) : Lexem(t), value(v) {}
        double value;
        ~Float() override {}
    };
    class Identifier final : public Lexem {
    public:
        Identifier(LexemType t, std::string v) : Lexem(t), value(v) {}
        std::string value;
        ~Identifier() override {}
    };
    class Coma final : public Lexem {
    public:
        Coma(LexemType t) : Lexem(t) {}
        ~Coma() override {}
    };
    class Unknown final : public Lexem {
    public:
        Unknown(LexemType t) : Lexem(t) {}
        ~Unknown() override {}
    };

    int yylex() override;

    std::unique_ptr<Lexem> &nextLexem() {
        this->yylex();
        return currentLexem_;
    }
    std::unique_ptr<Lexem> &currentLexem() { return currentLexem_; }

private:
    std::unique_ptr<Lexem> currentLexem_;

    int processInt() {
        currentLexem_ =
            std::make_unique<Int>(LexemType::INTEGER, std::atol(yytext));
        std::string a = yytext;
        return 0;
    }
    int processFloat() {
        currentLexem_ =
            std::make_unique<Float>(LexemType::FLOAT, std::atof(yytext));
        std::string a = yytext;
        return 0;
    }
    int processIdentifier() {
        currentLexem_ =
            std::make_unique<Identifier>(LexemType::IDENTIFIER, yytext);
        std::string a = yytext;
        return 0;
    }
    int processComma() {
        currentLexem_ = std::make_unique<Coma>(LexemType::COMMA);
        std::string a = yytext;
        return 0;
    }
    int processUnknown() {
        currentLexem_ = std::make_unique<Unknown>(LexemType::UNKNOWN);
        std::string a = yytext;
        return 1;
    }
};

} // namespace front::assembler
