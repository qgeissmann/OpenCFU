#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include "defines.hpp"
#include "ProcessingOptions.hpp"

class ArgumentParser
{
    public:
        ArgumentParser(int argc, char **argv,ProcessingOptions& opts);
    protected:
        void printHelp();
        void printVersion();
    private:
        const std::string m_help_string;
};

#endif // ARGUMENTPARSER_H



