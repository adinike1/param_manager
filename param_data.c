#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define R_UINT8  0x60
#define R_INT8   0x61
#define UINT16   0x62
#define INT16    0x63
#define R_UINT32 0x64
#define R_INT32  0x65
#define R_FLOAT32 0x66
#define R_DOUBLE  0x67

typedef struct 
{
    const char *name;
    uint32_t type;
    uint32_t attr;
    uint32_t hash;
}param_config_t;

typedef struct
{
    uint32_t default_val;
    uint32_t min_val;
    uint32_t max_val;
}UINT32_PARAM_U;

typedef struct
 {
     float default_val;
     float min_val;
     float max_val;
 }FLOAT_DATA;

typedef union
{
    UINT32_PARAM_U data_uint32;
    FLOAT_DATA data_float;
}param_data_u;

typedef struct
{
    const char *name;
    uint32_t id;
    uint8_t type;
    param_data_u data;
    uint32_t attr;
    void * data_addr;
}param_data_t;

#define INDEX 0
param_data_t param[] = 
{
    "temp",    0, R_UINT32,     [0].data.data_uint32 = {5,-20,85}, 0, NULL, 
    "current", 1, R_UINT32,     [1].data.data_uint32 = {20,-20,20},0, NULL,
    "voltage", 2, R_FLOAT32,    [2].data.data_float = {50.00f,-100.0f,200.0f},0, NULL,
};


param_data_t *param_find(param_data_t p[], uint32_t size, const char *name, const uint32_t id)
{
    
    for(uint32_t i = 0; i<size-1;i++)
    {
        if((!strcmp(p[i].name, name)) ||(p[i].id == id))
        {
            return &p[i];
        }
            
    }

    return NULL;
}

int32_t param_config_check(param_data_t p[], uint32_t size)
{
    for(uint32_t i = 0; i<size-1;i++)
    {
        for(uint32_t j = i+1; j<size;j++)
        {
            if(strcmp(p[i].name, p[j].name))
            {
                printf("param config error%s <%d/%d>\r\n", p[i].name, i, j);
                return -1;
            }
            
        }
    }

    return 0;
}

int32_t param_write(const char*name, uint8_t *buf,uint8_t len)
{
    param_data_t *p = NULL;

    p = param_find(param, ARRAY_SIZE(param), name, 0);
    if((!p))
    {
        printf("param_write failed,name:%s, len:%d\r\n", name, len);
        return -1;
    }
    //TODO:need verify data type and len
    memcpy(&p->data, buf, len);
    return 0;
}

int32_t main()
{
    int32_t ret = 0;
    int32_t i = 0;
    ret = param_config_check(param, ARRAY_SIZE(param));
    if(ret)
    {
        return ret;
    }
    for( i=0;i<2;i++)
    {
        printf("%10s %0x %d %d\r\n", param[i].name, param[i].type, param[i].id,  param[i].data.data_uint32.default_val);
    }
        printf("%10s %0x %d %f\r\n", param[i].name, param[i].type, param[i].id, param[i].data.data_float.default_val);

    return 0;
}
