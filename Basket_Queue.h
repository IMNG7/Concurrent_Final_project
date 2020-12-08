#ifndef BASKET_QUEUE_H
#define BASKET_QUEUE_H

using namespace std;
#include<atomic>

class node_t;
class pointer_t
{
public:	
	node_t* ptr;
	bool deleted;
	unsigned int tag;
	pointer_t(node_t* node_new,bool del,unsigned int t)
	{
		ptr=node_new;
		deleted=del;
		tag=t;
	}
};
class node_t
{
public:
	int value;
	atomic<pointer_t> next;
};
class queue_t
{
public:
	atomic<pointer_t> tail;
	atomic<pointer_t> head;
};

#endif