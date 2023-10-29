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
    AsmLex(const std::filesystem::path &paht);
    int yylex() override;
    void printCurrent() {
        std::cout << currentValue_ << std::endl;
    }

private:
    int processDigit() {
        currentValue_ = yytext;
        return 1;
    }
    int processInstruction() {
        currentValue_ = yytext;
        return 1;
    }
    int processUnknown() {
        return 1;
    }

    std::string currentValue_;
    std::ifstream inputFile_;
};

int AsmLex::yylex() {

}
AsmLex::AsmLex(const std::filesystem::path &path) : inputFile_(path) {}

}
