#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define R_BLOCK_DATA  0x50
#define R_UINT8       0x60
#define R_INT8        0x61
#define R_UINT16      0x62
#define R_INT16       0x63
#define R_UINT32      0x64
#define R_INT32       0x65
#define R_FLOAT       0x66
#define R_DOUBLE      0x67







typedef struct
 {
     int8_t default_val;
     int8_t min_val;
     int8_t max_val;
 }BLOCK_DATA;

typedef struct
{
    uint32_t default_val;
    uint32_t min_val;
    uint32_t max_val;
}UINT32_PARAM_U;

typedef struct
 {
     int8_t default_val;
     int8_t min_val;
     int8_t max_val;
 }INT8_DATA;


typedef struct
 {
     float default_val;
     float min_val;
     float max_val;
 }FLOAT_DATA;

typedef union
{
    BLOCK_DATA     R_BLOCK_DATA_T;
    UINT32_PARAM_U R_UINT32_T;
    FLOAT_DATA R_FLOAT_T;
    INT8_DATA R_INT8_T;
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

typedef struct 
{
    char *name;
    uint32_t type;
    uint32_t attr;
    param_data_u data;
}param_config_t;


typedef struct data

#define INDEX 0
param_data_t param[] = 
{
    "temp",    0, R_UINT32,     [0].data.R_UINT32_T = {5,-20,85}, 0, NULL, 
    "current", 1, R_UINT32,     [1].data.R_UINT32_T = {20,-20,20},0, NULL,
    "voltage", 2, R_FLOAT,    [2].data.R_FLOAT_T = {50.00f,-100.0f,200.0f},0, NULL,
};
extern param_config_t*  __param_config_start__;
extern param_config_t*  __param_config_end__;
extern param_config_t*  _param_config_end__;

#define DECLARE_PARAM_BLOCK(block_name) \


#define DECLARE_PARAM_CFG(data_name, type, attr, def, min,max)  \
    static param_config_t data_name __attribute__((unused, section(".param_config"))) \
    = {#data_name, type, attr, .data.type##_T = {def, min, max}};

DECLARE_PARAM_CFG(bat_data,    R_BLOCK_DATA, 0, 0,0, 0);
DECLARE_PARAM_CFG(temperature, R_UINT32, 0, 10,0, 85);
DECLARE_PARAM_CFG(current,     R_FLOAT,      0, -10.0f,-1000.0f, 1000.0f);
DECLARE_PARAM_CFG(vol,         R_INT8,           0, -10,-128, 127);



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
            if(!strcmp(p[i].name, p[j].name))
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
        printf("%10s %0x %d %d\r\n", param[i].name, param[i].type, param[i].id,  param[i].data.R_UINT32_T.default_val);
    }
        printf("%10s %0x %d %f\r\n", param[i].name, param[i].type, param[i].id, param[i].data.R_FLOAT_T.default_val);

        //TODO: how to define a section in linux gcc
    //printf("config[0]:%s\r\n", __param_config_start__->name);;
    printf("config[0]:%s\r\n", current.name);;

    //printf("start/end_addr:0x%llx 0x%llx %llu\r\n", (uint64_t)__param_config_start__,(uint64_t)_param_config_end__,(uint64_t)_param_config_end__);

    return 0;
}
