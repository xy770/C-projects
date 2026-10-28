#ifndef XYC_STRING_INCLUDED
#define XYC_STRING_INCLUDED

#include <stddef.h>

typedef enum {
    XYC_STRING_OK = 0,
    XYC_STRING_MEMORY_ERR = 1,
    XYC_STRING_EMPTY_POINTER_ERR = 2,
} XycStringErr;

typedef struct XycStringHeader XycStringHeader;
struct XycStringHeader{
    size_t len;      // the lengh of the string, include '\0'
    size_t capacity; // include the size of header!
};

XycStringErr xyc_string_push(char **xycstr,const char *str);
XycStringErr xyc_string_free(char **xycstr);

#endif /* XYC_STRING_INCLUDED */

#define XYC_IMPLEMENTATION

#ifdef XYC_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define XYC_STRING_TO_HEADER(xycstr) ((XycStringHeader*)((char*)(xycstr) - sizeof(XycStringHeader)))
#define XYC_STRING_INITIAL_CAP 128
#define XYC_STRING_DEFAULT_GROWING_FACTOR 2

XycStringErr xyc_string_push(char **xycstr,const char *str){
    if(xycstr == NULL || str == NULL) return XYC_STRING_EMPTY_POINTER_ERR;

    if(*xycstr == NULL){
        *xycstr = (char*)malloc(sizeof(XycStringHeader) + XYC_STRING_INITIAL_CAP);
        if(*xycstr == NULL) return XYC_STRING_MEMORY_ERR;
        ((XycStringHeader*)(*xycstr))->capacity = sizeof(XycStringHeader) + XYC_STRING_INITIAL_CAP;
        ((XycStringHeader*)(*xycstr))->len = 1;
        *xycstr += sizeof(XycStringHeader);
        **xycstr = '\0';
    }

    if(*str == '\0') return XYC_STRING_OK;

    XycStringHeader *header = XYC_STRING_TO_HEADER(*xycstr);

    size_t times = 1;
    size_t str_len = strlen(str);
    while(header->len + str_len > (header->capacity - sizeof(XycStringHeader)) * times){
        times *= XYC_STRING_DEFAULT_GROWING_FACTOR;
    }

    if(times != 1){
        size_t re_cap = sizeof(XycStringHeader) + ((header->capacity - sizeof(XycStringHeader)) * times);
        char *tmp = (char*)realloc(header, re_cap);
        if(tmp == NULL) return XYC_STRING_MEMORY_ERR;
        header = (XycStringHeader*)tmp;
        *xycstr = tmp + sizeof(XycStringHeader);
        header->capacity = re_cap;
    }

    assert(header->len > 0);
    memcpy(*xycstr + header->len - 1, str, str_len + 1);
    header->len += str_len;

    return XYC_STRING_OK;
}

XycStringErr xyc_string_free(char **xycstr){
    if(xycstr == NULL) return XYC_STRING_EMPTY_POINTER_ERR;
    free(*xycstr - sizeof(XycStringHeader));
    *xycstr = NULL;
    return XYC_STRING_OK;
}

#endif /* XYC_IMPLEMENTATION */
