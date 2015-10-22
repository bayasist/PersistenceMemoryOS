#ifndef COMMON_FILE_DEFINED
	#define COMMON_FILE_DEFINED 1
	void outPortByte( unsigned short address, unsigned char value );
	unsigned char inPortByte( unsigned short address );
	#define	_sti( ) ({ __asm__ __volatile__ ( "sti" ); })
	#define	_cli( ) ({ __asm__ __volatile__ ( "cli" ); })
	void cli(void);
	void sti(void);

	#define NULL ((void*)0x0)
	typedef unsigned int ADDRESS;
	typedef unsigned int MemorySize;
	
	typedef struct{
		void* start;
		MemorySize size;
	} MemoryRange;
	
	
	#define DEBUG_STOP() do{cli();while(1);}while(0);
	#define DEBUG_STOP_INT(________value,xx, yy) do{Point ________p;________p.x = xx;________p.y = yy;cli();clearScreen(mainImageSheet, COLOR_WHITE);putInt(mainImageSheet, ________value, ________p, COLOR_BLACK);draw();while(1);}while(0)
	
	
typedef struct Point{
	int x, y;
} Point;
typedef struct Size{
	int width, height;
} Size;


void initCommon(void);

/*
#define ADD_LIST_FUNCTION(FUNC_NAME, LIST_TYPE, VALUE_TYPE, LIST_VALUE) \
	void FUNC_NAME(VALUE_TYPE *value){ \
		LIST_TYPE *p; \
		LIST_TYPE *tmp = allocMemoryType(LIST_TYPE); \
		tmp->next = NULL; \
		tmp->value = value; \
		for(p = LIST_VALUE; p->next != NULL; p = p->next); \
		p->next = tmp; \
	}
*/




/*リストの定義をします*/
#define LIST_DEFINE(TYPE_NAME, VALUE_TYPE) \
	typedef struct TYPE_NAME{ \
		struct TYPE_NAME *next; \
		VALUE_TYPE value; \
	} TYPE_NAME;


#define INIT_LIST_FUNCTION(FUNC_NAME, LIST_TYPE)	\
	void FUNC_NAME(LIST_TYPE **pList){				\
		*pList = allocMemoryType(LIST_TYPE);			\
		(*pList)->next = NULL;						\
	}
/*リストの最後に項目を追加する関数を定義します*/
#define ADD_LIST_FUNCTION(FUNC_NAME, LIST_TYPE, VALUE_TYPE)			\
	LIST_TYPE* FUNC_NAME(LIST_TYPE *list, VALUE_TYPE value){		\
		LIST_TYPE *p;												\
		LIST_TYPE *tmp = allocMemoryType(LIST_TYPE);				\
		tmp->next = NULL;											\
		tmp->value = value;											\
		for(p = list; p->next != NULL; p = p->next);				\
		p->next = tmp;												\
		return tmp;													\
	}

#define REMOVE_LIST_FUNCTION(FUNC_NAME, LIST_TYPE, VALUE_TYPE)		\
	int FUNC_NAME(LIST_TYPE *list, VALUE_TYPE value){				\
		LIST_TYPE *p, *select, *selectBefore;						\
		select = NULL;												\
		for(p = list; p->next != NULL; p = p->next){				\
			if(p->next->value == value){							\
				selectBefore = p;									\
				select = p->next;									\
				break;												\
			}														\
		}															\
		if(select == NULL){											\
			return 0;												\
		}															\
		selectBefore->next = select->next;							\
		freeMemory(select, sizeof(LIST_TYPE));						\
		return 1;													\
	}
#define INSERT_QUEUE_FUNCTION(FUNC_NAME, LIST_TYPE, VALUE_TYPE) ADD_LIST_FUNCTION(FUNC_NAME, LIST_TYPE, VALUE_TYPE)
#define DELETE_QUEUE_FUNCTION(FUNCTION_NAME, LIST_TYPE, VALUE_TYPE)	\
	int FUNCTION_NAME(LIST_TYPE *list,VALUE_TYPE *value){			\
		LIST_TYPE *nextCell;										\
		if(list->next == NULL){										\
			return 0;												\
		}															\
		nextCell = list->next->next;								\
		*value = list->next->value;									\
		freeMemory(list->next, sizeof(LIST_TYPE));					\
		list->next = nextCell; 										\
		return 1; 													\
	}
#define COUNT_LIST_FUNCTION(FUNCTION_NAME, LIST_TYPE)				\
	int FUNCTION_NAME(LIST_TYPE *list){								\
		LIST_TYPE *p;												\
		int value;													\
		value = 0;													\
		for(p = list; p->next != NULL; p = p->next){				\
			value++;												\
		}															\
		return value;												\
	}

#define KEY_VALUE_PAIR_DEFINE(TYPE_NAME, KEY_TYPE, VALUE_TYPE) 		\
	typedef struct TYPE_NAME{ 										\
		struct TYPE_NAME *next; 									\
		KEY_TYPE key;												\
		VALUE_TYPE value; 											\
	} TYPE_NAME;
	

#define GET_KEY_VALUE_PAIR(FUNC_NAME, LIST_TYPE, KEY_TYPE, VALUE_TYPE)	\
	int FUNC_NAME(LIST_TYPE *list, KEY_TYPE key, VALUE_TYPE *value){			\
		LIST_TYPE *p;															\
		for(p = list; p->next != NULL; p = p->next){							\
			if(p->next->key == key){											\
				*value = p->next->value;										\
				return 1;														\
			}																	\
		}																		\
		return 0;																\
	}

#define INSERT_KEY_VALUE_PAIR(FUNC_NAME, GET_KEY_VALUE_PAIR_NAME, LIST_TYPE, KEY_TYPE, VALUE_TYPE)	\
	int FUNC_NAME(LIST_TYPE *list, KEY_TYPE key, VALUE_TYPE value){									\
		VALUE_TYPE tmp;																				\
		LIST_TYPE *newCell;																			\
		if(GET_KEY_VALUE_PAIR_NAME(list, key, &tmp)){												\
			return 0;																				\
		}																							\
		newCell = allocMemoryType(LIST_TYPE);														\
		newCell->key = key;																			\
		newCell->value = value;																		\
		newCell->next = list->next;																	\
		list->next = newCell;																		\
		return 1;																					\
	}
	
	
	
	
	
	
	
	
	
	
int isHitRectanglePoint(Point, Size, Point);
	
#endif

