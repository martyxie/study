#ifndef __SWJSON_H__
#define __SWJSON_H__


#define VTYPE_STR    1
#define VTYPE_OBJ    2
#define VTYPE_ARRAY  3


typedef struct sw_json_t sw_json_t;
typedef struct json_name_value_t json_name_value_t;
typedef struct json_object_t json_object_t;
typedef struct json_array_t json_array_t;

struct sw_json_t
{
	int   type;
	union
	{
		char* str;
		json_object_t* jobj;
		json_array_t* jarr;
	};
};

struct json_name_value_t
{
	char* name;
	sw_json_t value;
};


struct json_object_t
{
	json_name_value_t* nvs;
	int count;
};

struct json_array_t
{
	sw_json_t* values;
	int count;
};


#ifdef __cplusplus
extern "C"{
#endif



/***
 *   ���� json ���������һ�� sw_json_t ����ָ��
 * @param str: json ��
 * @param length: str ����
 * @param buf: ���� json ��ʹ�õĻ���������Ž������
 * @param size: buf �Ĵ�С
 * @return: ������������ָ�룬NULL ��ʾ����ʧ��
 * ע�������������� buf �У���˲���Ҫ��������
 */
sw_json_t* sw_json_decode( const char* str, int length, void* buf, int size );


#ifdef __cplusplus
}
#endif


#endif


