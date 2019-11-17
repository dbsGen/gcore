//
// Created by gen on 16/8/29.
//

#ifndef VOIPPROJECT_DATA_H
#define VOIPPROJECT_DATA_H


#include <memory>
#include "Ref.h"
#include "core_define.h"

namespace gc {
    CLASS_BEGIN_NV(Data, Object)

        void *buffer;

    public:
        METHOD virtual long getSize() const = 0;
        METHOD virtual bool empty() const = 0;
        METHOD virtual void close() = 0;
        virtual bool gets(void *chs, long start, long len) = 0;
        virtual const void *getBuffer();
        METHOD const char *text();

        _FORCE_INLINE_ Data() : buffer(NULL) {}
        _FORCE_INLINE_ ~Data() {if (buffer) free(buffer);}
    
        METHOD static Ref<Data> fromString(const std::string &str);

    protected:
        ON_LOADED_BEGIN(cls, Object)
            ADD_METHOD(cls, Data, getSize);
            ADD_METHOD(cls, Data, empty);
            ADD_METHOD(cls, Data, close);
            ADD_METHOD(cls, Data, text);
            ADD_METHOD(cls, Data, fromString);
        ON_LOADED_END
    CLASS_END

    CLASS_BEGIN_N(BufferData, Data)

        long size;
        void *b_buffer;
        bool retain;

    public:
        typedef int RetainType;
        enum {
            Ref,
            Copy,
            Retain
        };
    
        _FORCE_INLINE_ virtual long getSize() const {return size;}
        _FORCE_INLINE_ virtual const void *getBuffer() { return b_buffer;}
        _FORCE_INLINE_ virtual bool empty() const {
            return !(b_buffer && size);
        }
        _FORCE_INLINE_ virtual void close() {
        }
        virtual bool gets(void *chs, long start, long len);

        _FORCE_INLINE_ BufferData() : size(0), b_buffer(NULL) {}
        void initialize(void* buffer, long size, RetainType retain = Copy);
        ~BufferData() {
            if (retain && b_buffer) {
                free(b_buffer);
            }
        }

    CLASS_END
    
    
    CLASS_BEGIN_N(FileData, Data)
    
        long size;
        FILE *file;
        std::string path;
        void *buffer;
    
        void loadFile() {
            if (!file && !path.empty()) {
                file = fopen(path.c_str(), "r");
            }
        }
    
    public:
        virtual long getSize() const;
        virtual bool empty() const {
            return path.empty();
        }
        virtual void close() {
            if (buffer) {
                free(buffer);
                buffer = NULL;
            }
            if (file) {
                fclose(file);
                file = NULL;
            }
        }
        virtual bool gets(void *chs, long start, long len);
        virtual const void *getBuffer();
    
        FileData() : file(NULL), size(0), buffer(NULL) {}
        FileData(const std::string &path) : FileData() {
            this->path = path;
        }
        ~FileData() {
            close();
        }
        METHOD _FORCE_INLINE_ void initialize(const char *path) {
            if (path) this->path = path;
        }
    
    protected:
        ON_LOADED_BEGIN(cls, Data)
            INITIALIZER(cls, FileData, initialize);
        ON_LOADED_END
    CLASS_END
}


#endif //VOIPPROJECT_DATA_H
