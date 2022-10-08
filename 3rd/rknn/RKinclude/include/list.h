
#ifndef WEBRTC_LIST_H_F360898B84CE49FF9F88F5DB59B5950C
#define WEBRTC_LIST_H_F360898B84CE49FF9F88F5DB59B5950C
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
typedef int (*WRCompareFunc)(const void *a, const void *b);
struct _WRList {
	struct _WRList *next;
	struct _WRList *prev;
	void *data;
};
#define wr_list_next(elem) ((elem)->next)
typedef struct _WRList WRList;
WRList * wr_list_append(WRList *elem, void * data);
WRList * wr_list_prepend(WRList *elem, void * data);
WRList *wr_list_append_link(WRList *elem, WRList *new_elem);
WRList * wr_list_free(WRList *elem);
WRList * wr_list_remove(WRList *first, void *data);
WRList *wr_list_remove_link(WRList *list, WRList *elem);
int wr_list_size(const WRList *first);
void * wr_list_nth_data(const WRList *list, int index);
WRList *wr_list_find(WRList *list, void *data);
void wr_list_for_each(const WRList *list, void (*func)(void *));
WRList *wr_list_find_custom(WRList *list, WRCompareFunc compare_func, const void *user_data);


#endif


