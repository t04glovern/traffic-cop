#ifndef __STEPHEN_STRING_HPP
#   define __STEPHEN_STRING_HPP

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
/*
class FakeSerial {
public:
    void print(const char* str) {
        std::cout << str << std::endl;
    }
};

FakeSerial Serial;
*/

class StephenString {
private:
    char* _buffer = nullptr;
    size_t _length = 0;

private:
    void allocate_buffer(size_t length) {
        _length = length;
        _buffer = static_cast<char*>(malloc(_length + 1));

        if(!_buffer) {
            // Serial.print("Could not allocate String buffer");
            abort();
        }
    }

public:
    StephenString(const char* cstr) {
        allocate_buffer(strlen(cstr));

        memcpy(_buffer, cstr, _length);
        _buffer[_length] = '\0';
    }

    StephenString(const char* lhs, size_t lhs_length,
                  const char* rhs, size_t rhs_length) {
        allocate_buffer(lhs_length + rhs_length);

        memcpy(_buffer, lhs, lhs_length);
        memcpy(_buffer + lhs_length, rhs, rhs_length);
        _buffer[lhs_length + rhs_length] = '\0';
    }

    StephenString(const StephenString& s) {
        allocate_buffer(s._length);

        memcpy(_buffer, s.cstr(), _length);
        _buffer[_length] = '\0';
    }

    ~StephenString() {
        free(_buffer);
    }

     char* cstr() const {
        return _buffer;
    }

    size_t length() const {
        return _length;
    }

    StephenString& operator=(const StephenString& s) {
        if(this == &s) {
            return *this;
        }

        free(_buffer);
        allocate_buffer(s._length);
        memcpy(_buffer, s.cstr(), _length);
        _buffer[_length] = '\0';
        return *this;
    }
};

inline StephenString operator+(const StephenString& lhs, const StephenString& rhs) {
    StephenString s(lhs.cstr(), lhs.length(), rhs.cstr(), rhs.length());
    return s;
}

#endif
