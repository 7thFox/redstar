#include "headers/stringtable.h"
#define debugf(...) ;
// #define debugf(...) printf(__VA_ARGS__)

uint8_t get_hash(const char *str, size_t len);
static inline StringIndex strptr2index(StringTable *t, const char *str);
const char *_copy_string(StringTable *t, const char *str, size_t len);
StringIndex find_add_in_bucket(StringTable *t, StringTableBucket *b, const char *str, size_t len);

StringTable init_string_table() {
    StringTable t;

    t.size = 0;
    t.cap = 1024;
    t.strings = malloc(sizeof(char) * t.cap);

    t.lookup = malloc(sizeof(StringTableBucket) * 256);
    memset(t.lookup, 0, sizeof(StringTableBucket) * 256);

    t.collisions = init_array(sizeof(StringTableBucket), 256);

    return t;
}

void destroy_string_table(StringTable *t) {
    free(t->strings);
    free(t->lookup);
    free(t->collisions.array);
}

#define debugstop(x) debugf("\r    %s                ", x);

StringIndex copy_string(StringTable *t, const char *str, size_t len) {
    uint8_t hash = get_hash(str, len);

    debugf("\ncopy_string '%.*s' - 0x%02x\n", (int)len, str, hash);

    StringTableBucket *b = t->lookup + hash;
    StringIndex i;

    debugstop("first bucket val 1");
    if (!b->value1) return strptr2index(t, (b->value1 = _copy_string(t, str, len)));
    if ((i = find_add_in_bucket(t, b, str, len)).i) return i;

    while (b->next) {
        b = b->next;
        if ((i = find_add_in_bucket(t, b, str, len)).i) return i;
    }
    // end of bucket
    debugstop("new val 1");
    b->next = next_array_elem(&t->collisions);
    b = b->next;
    memset(b, 0, sizeof(StringTableBucket));
    b->value1 = _copy_string(t, str, len);
    return strptr2index(t, b->value1);
}

StringIndex find_add_in_bucket(StringTable *t, StringTableBucket *b, const char *str, size_t len) {
    // debugf("find_add_in_bucket\n");
    debugstop("found val 1");
    if (strncmp(b->value1, str, len) == 0) return strptr2index(t, b->value1);

    debugstop("new val 2");
    if (!b->value2) return strptr2index(t, (b->value2 = _copy_string(t, str, len)));
    debugstop("found val 2");
    if (strncmp(b->value2, str, len) == 0) return strptr2index(t, b->value2);

    debugstop("new val 3");
    if (!b->value3) return strptr2index(t, (b->value3 = _copy_string(t, str, len)));
    debugstop("found val 3");
    if (strncmp(b->value3, str, len) == 0) return strptr2index(t, b->value3);

    debugstop("new val 4");
    if (!b->value4) return strptr2index(t, (b->value4 = _copy_string(t, str, len)));
    debugstop("found val 4");
    if (strncmp(b->value4, str, len) == 0) return strptr2index(t, b->value4);

    debugstop("new val 5");
    if (!b->value5) return strptr2index(t, (b->value5 = _copy_string(t, str, len)));
    debugstop("found val 5");
    if (strncmp(b->value5, str, len) == 0) return strptr2index(t, b->value5);

    debugstop("new val 6");
    if (!b->value6) return strptr2index(t, (b->value6 = _copy_string(t, str, len)));
    debugstop("found val 6");
    if (strncmp(b->value6, str, len) == 0) return strptr2index(t, b->value6);

    debugstop("new val 7");
    if (!b->value7) return strptr2index(t, (b->value7 = _copy_string(t, str, len)));
    debugstop("found val 7");
    if (strncmp(b->value7, str, len) == 0) return strptr2index(t, b->value7);

    return (StringIndex){0};
}

uint8_t get_hash(const char *str, size_t len) {
    // debugf("get_hash\n");
    uint8_t hash = 0;
    for (int i = 0; i < len; i++)
    {
        hash ^= str[i];
    }
    return hash;
}

static inline StringIndex strptr2index(StringTable *t, const char *str) {
    // debugf("strptr2index\n", t->);
    return (StringIndex){(uint32_t)((uint64_t)str) - ((uint64_t)t->strings)+1};
}

const char *_copy_string(StringTable *t, const char *str, size_t len) {
    // debugf("_copy_string\n");
    int new_size = t->size + len + 1;
    if (new_size >= t->cap) {
        t->cap += 1024;
        t->strings = realloc(t->strings, sizeof(char) * t->cap);
    }
    char *cpy_str = t->strings + t->size;
    strncpy(cpy_str, str, len);
    cpy_str[len] = '\0';
    t->size = new_size;
    return cpy_str;
}