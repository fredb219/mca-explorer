#ifndef NTB_H
#define NTB_H

typedef struct Tag Tag;

typedef enum {
    TAG_END = 0x00,
    TAG_BYTE = 0x01,
    TAG_SHORT = 0x02,
    TAG_INT = 0x03,
    TAG_LONG = 0x04,
    TAG_FLOAT = 0x05,
    TAG_DOUBLE = 0x06,
    TAG_BYTE_ARRAY = 0x07,
    TAG_STRING = 0x08,
    TAG_LIST = 0x09,
    TAG_COMPOUND = 0x0A,
    TAG_INT_ARRAY = 0x0B
} TagType;

struct Tag {
    TagType type;
    char *name;
    void *payload;
};

struct TagByteArrayPayload {
    int length;
    unsigned char *payload;
};

struct TagIntArrayPayload {
    int length;
    int *payload;
};

struct TagListPayload {
    int length;
    TagType type;
    void *payload;
};

struct TagCompoundPayload {
    Tag *payload;
    struct TagCompoundPayload *nextTagPayload;
};


Tag *ntb_parseData(unsigned char * data, int size, int *indexUse);

void ntb_destroyTag(Tag *tag);

unsigned char ntb_getByteByName(Tag *parent, char* name);

struct TagByteArrayPayload *ntb_getByteArrayByName(Tag *parent, char* name);

struct Tag *ntb_getTagByName(Tag *parent, char* name);

void ntb_print(Tag *tag, int indent);

#endif

