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
    enum LexemType {
        INTEGER,
        FLOAT,
        IDENTIFIER,
        FUNC,
        STRING,
        OP_CURLY_BRACKET,
        CL_CURLY_BRACKET,
        COMMA,
        UNKNOWN
    };

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
    class Func final : public Lexem {
    public:
        Func(LexemType t) : Lexem(t) {}
        ~Func() override {}
    };
    class String final : public Lexem {
    public:
        String(LexemType t, std::string str) : Lexem(t), value(str) {}
        std::string value;
        ~String() override {}
    };
    class OpCurlyBracket final : public Lexem {
    public:
        OpCurlyBracket(LexemType t) : Lexem(t) {}
        ~OpCurlyBracket() override {}
    };
    class ClCurlyBracket final : public Lexem {
    public:
        ClCurlyBracket(LexemType t) : Lexem(t) {}
        ~ClCurlyBracket() override {}
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
        return 0;
    }
    int processFloat() {
        currentLexem_ =
            std::make_unique<Float>(LexemType::FLOAT, std::atof(yytext));
        return 0;
    }
    int processIdentifier() {
        currentLexem_ =
            std::make_unique<Identifier>(LexemType::IDENTIFIER, yytext);
        return 0;
    }
    int processFunc() {
        currentLexem_ = std::make_unique<Coma>(LexemType::FUNC);
        return 0;
    }
    int processOpCurlyBracket() {
        currentLexem_ = std::make_unique<Coma>(LexemType::OP_CURLY_BRACKET);
        return 0;
    }
    int processClCurlyBracket() {
        currentLexem_ = std::make_unique<Coma>(LexemType::CL_CURLY_BRACKET);
        return 0;
    }
    int processComma() {
        currentLexem_ = std::make_unique<Coma>(LexemType::COMMA);
        return 0;
    }
    int processString() {
        std::string withQuotes(yytext);
        currentLexem_ = std::make_unique<String>(
            LexemType::STRING, withQuotes.substr(1, withQuotes.size() - 2));
        return 0;
    }
    int processUnknown() {
        currentLexem_ = std::make_unique<Unknown>(LexemType::UNKNOWN);
        return 1;
    }
};

} // namespace front::assembler
