#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>

class Log {
public:
    enum Severity {
        DEBUG,
        INFORMATIONAL,
        NOTICE,
        WARNING,
        ERROR,
        CRITICAL,
        ALERT,
        EMERGENCY
    };

private:
    // C++ streams can not be copied! Therefore we need to use a reference
    // counting approach.
    struct Stream {
        Stream() : severity(INFORMATIONAL), refCount(1) {}

        Stream(Severity s, const char* cat)
                            : severity(s), category(cat), refCount(1) {}

        // The severity defines the detail of the logged messages
        Severity severity;

        // The category defines the module for which to log messages
        std::string category;

        int refCount;
        std::stringstream result;
    }*stream;

    void doLog();

public:

    inline Log() : stream(new Stream()) {}

    inline Log(Severity severity, const char* category) :
            stream(new Stream(severity, category)) {}

    inline Log(const Log& l) : stream(l.stream) { stream->refCount++; }

    inline ~Log() {
        if (--stream->refCount == 0) {
            // do the actual logging through the desired library
            doLog();
            delete stream;
        }
    }

    static inline Log log() { return Log(); }

    static inline Log log(Severity severity, const char* category) {
        return Log(severity, category);
    }

    // typesafe inserters for standard C++ types
    inline Log& operator<<(bool v) { v ? stream->result << "true" : stream->result << "false"; return *this; }
    inline Log& operator<<(char v) { stream->result << v; return *this; }
    inline Log& operator<<(signed short v) { stream->result << v; return *this; }
    inline Log& operator<<(unsigned short v) { stream->result << v; return *this; }
    inline Log& operator<<(signed int v) { stream->result << v; return *this; }
    inline Log& operator<<(unsigned int v) { stream->result << v; return *this; }
    inline Log& operator<<(signed long v) { stream->result << v; return *this; }
    inline Log& operator<<(unsigned long v) { stream->result << v; return *this; }
    inline Log& operator<<(float v) { stream->result << v; return *this; }
    inline Log& operator<<(double v) { stream->result << v; return *this; }
    inline Log& operator<<(const char* v) { stream->result << v; return *this; }
    inline Log& operator<<(const void* v) { stream->result << v; return *this; }
};

#define LOG(s) (Log() << s)
// #define LOG(s)


// typesafe inserters for Qt types
#include <QString>
#include <QRect>

inline Log operator<<(Log l, const QString& v) {
    l << v.toLatin1().constData();
    return l;
}

inline Log operator<<(Log l, const QRect& v) {
    l << "QRect[" << v.x() << "," << v.y() << " " << v.width() << "x" << v.height() << "]";
    return l;
}

inline Log operator<<(Log l, const QRectF& v) {
    l << "QRectF[" << v.x() << "," << v.y() << " " << v.width() << "x" << v.height() << "]";
    return l;
}

inline Log operator<<(Log l, const QPoint& v) {
    l << "QPoint[" << v.x() << "," << v.y() << "]";
    return l;
}

inline Log operator<<(Log l, const QPointF& v) {
    l << "QPointF[" << v.x() << "," << v.y() << "]";
    return l;
}

inline Log operator<<(Log l, const QSize& v) {
    l << "QSize[" << v.width() << "x" << v.height() << "]";
    return l;
}

inline Log operator<<(Log l, const QSizeF& v) {
    l << "QSizeF[" << v.width() << "x" << v.height() << "]";
    return l;
}


#endif
