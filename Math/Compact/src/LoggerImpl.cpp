#include <ostream>
#include <fstream>
#include <iostream>
#include <map>
#include "../include/ILogger.h"

namespace
{
    class LoggerImpl : public ILogger
    {
    public:
        static ILogger* createLogger()
        {
            return new LoggerImpl();
        }

        static ILogger* createLogger(const char* const& filename, bool overwrite = true)
        {
            return new LoggerImpl(filename, overwrite);
        }

        RC log(RC code, Level level, const char* const& srcfile, const char* const& function, int line) override
        {
            return log(code, level);
        }

        RC log(RC code, Level level) override
        {
            *stream << levels[level] << ": " << errors[code] << std::endl;
            return RC::SUCCESS;
        }

        ~LoggerImpl()
        {
            if (stream != &std::cout)
                delete stream;
        }

    private:
        std::ostream *stream;
        std::map<RC, std::string> errors;
        std::map<Level, std::string> levels;

    protected:
        LoggerImpl()
        {
            stream = &std::cout;
            InitErrorsMap();
            InitLevelsMap();
        }

        LoggerImpl(const char* const& filename, bool overwrite = true)
        {
            if (overwrite)
                stream = new std::ofstream(filename, std::ios_base::out);
            else
                stream = new std::ofstream(filename, std::ios_base::app);

            InitErrorsMap();
            InitLevelsMap();
        }

        void InitErrorsMap()
        {
            errors.insert({ RC::ALLOCATION_ERROR, "Couldn't allocate new memory"});
            errors.insert({ RC::FILE_NOT_FOUND, "Couldn't find file with corresponding name"});
            errors.insert({ RC::INDEX_OUT_OF_BOUND, "The index is outside the defined boundaries"});
            errors.insert({ RC::INFINITY_OVERFLOW, "Number is greater than infinity"});
            errors.insert({ RC::INVALID_ARGUMENT, "Received invalid argument"});
            errors.insert({ RC::IO_ERROR, "Couldn't write/read to/from file"});
            errors.insert({ RC::MEMORY_INTERSECTION, "Found intersecting memory while copying instance"});
            errors.insert({ RC::MISMATCHING_DIMENSIONS, "The dimensions of these vectors do not match"});
            errors.insert({ RC::NOT_NUMBER, "The argument is not a number in terms of mathematics"});
            errors.insert({ RC::NULLPTR_ERROR, "Received nullptr"});
            errors.insert({ RC::SOURCE_SET_DESTROYED, "Source set destroyed"});
            errors.insert({ RC::SOURCE_SET_EMPTY, "Source set empty"});
            errors.insert({ RC::SUCCESS, "Work done successfully"});
            errors.insert({ RC::UNKNOWN, "Unknown error"});
            errors.insert({ RC::VECTOR_NOT_FOUND, "Couldn't find vector instance in ISet method"});
            errors.insert({ RC::VECTOR_ALREADY_EXIST, "The requested vector already exists in the set"});
        }

        void InitLevelsMap()
        {
            levels.insert({ Level::INFO, "INFO" });
            levels.insert({ Level::SEVERE, "SEVERE" });
            levels.insert({ Level::WARNING, "WARNING" });
        }
    };
};

ILogger* ILogger::createLogger()
{
    return LoggerImpl::createLogger();
}

ILogger* ILogger::createLogger(const char* const& filename, bool overwrite)
{
    return LoggerImpl::createLogger(filename, overwrite);
}

ILogger::~ILogger() = default;
