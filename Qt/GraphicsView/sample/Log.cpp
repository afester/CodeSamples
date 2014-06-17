
#include "Log.h"

//#define PANTHEIOS_LOG
//#define STDERR_LOG
#define QDEBUG_LOG
//#define LOG4CXX_LOG


#ifdef PANTHEIOS_LOG

#include <pantheios/pantheios.hpp>

extern "C" const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = "test.exe";

void Log::doLog() {
    switch(stream->severity) {
        case DEBUG : pantheios::log(pantheios::debug, stream->result.str().c_str()); break;
        case INFORMATIONAL : pantheios::log(pantheios::informational, stream->result.str().c_str()); break;
        case NOTICE : pantheios::log(pantheios::notice, stream->result.str().c_str()); break;
        case WARNING : pantheios::log(pantheios::warning, stream->result.str().c_str()); break;
        case ERROR : pantheios::log(pantheios::error, stream->result.str().c_str()); break;
        case CRITICAL : pantheios::log(pantheios::critical, stream->result.str().c_str()); break;
        case ALERT : pantheios::log(pantheios::alert, stream->result.str().c_str()); break;
        case EMERGENCY : pantheios::log(pantheios::emergency, stream->result.str().c_str()); break;
    }
}
#endif

#ifdef STDERR_LOG

#include <stdio.h>

void Log::doLog() {
    fprintf(stderr, "[%s %d] %s\n",
            stream->category.c_str(), stream->severity, stream->result.str().c_str());
}
#endif

#ifdef QDEBUG_LOG

#include <QDebug>

void Log::doLog() {
    qDebug() << "[" << stream->category.c_str()
             << " " << stream->severity
             << "] " << stream->result.str().c_str();
}
#endif

#ifdef LOG4CXX_LOG

#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"


using namespace log4cxx;

void Log::doLog() {
    static bool init  = false;
    if (!init) {
        PropertyConfigurator::configure("log4cxx.properties");
        init = true;
    }

    LoggerPtr logger(Logger::getLogger(stream->category));
    switch(stream->severity) {
        case DEBUG : LOG4CXX_DEBUG(logger, stream->result.str()); break;

        case INFORMATIONAL :
        case NOTICE :LOG4CXX_INFO(logger, stream->result.str());   break;

        case WARNING :  LOG4CXX_WARN(logger, stream->result.str()); break;

        case ERROR : LOG4CXX_ERROR(logger, stream->result.str()); break;

        case CRITICAL : 
        case ALERT : 
        case EMERGENCY : LOG4CXX_FATAL(logger, stream->result.str());  break;
    }
} 

#endif

