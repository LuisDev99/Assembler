#ifndef myExceptions__
#define myExceptions__

#include <exception>
#include <string>
#include "tokens.h"

class UnexpectedPointReachedException : public std::exception
{
public:
    UnexpectedPointReachedException(const char *functionName, const char *fileName, int lineNumber)
    {
        std::string wStringFuncName = functionName;
        std::string wStringFileName = fileName;

        message = "Program hit a point where it should not have reached exception, in file: " + wStringFileName + ", function: " + wStringFuncName + ", on line: " + std::to_string(lineNumber);
    }

    const char *getErrorMessage()
    {
        return message.c_str();
    }

private:
    std::string message;
};

class IllegalInstructionException : public std::exception
{
public:
    IllegalInstructionException(int lineNumber)
    {
        message = "Invalid instruction at: " + std::to_string(lineNumber);
    }

    const char *getErrorMessage()
    {
        return message.c_str();
    }

private:
    std::string message;
};

class UnexpectedTokenException : public std::exception
{
public:
    UnexpectedTokenException(Token expectedToken, Token currentToken, int lineNumber)
    {
        const char *expectedTokenStr = tokenToStringy(expectedToken);
        const char *illegalTokenStr = tokenToStringy(currentToken);

        message = "Expected: " + std::string(expectedTokenStr) + ", Found: " + std::string(illegalTokenStr) + ", on line: " + std::to_string(lineNumber);
    }

    const char *getErrorMessage()
    {
        return message.c_str();
    }

private:
    std::string message;
};

#endif
