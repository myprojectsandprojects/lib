#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

namespace lib
{
	template <typename TYPE>
	struct link
	{
		link *Next, *Prev;
	//	const void *Data;
		TYPE Data;
	};

	typedef void (userPrintDataFunc)(const void *);

	template <typename TYPE>
	struct linkedList
	{
		link<TYPE> *Head, *Tail;

		linkedList(userPrintDataFunc print_data = NULL);
		link<TYPE> *append(TYPE Data);
		void remove(link<TYPE> *Link); // O(1)!
		void print();

		private:
		userPrintDataFunc *print_data;
	};

	#include "linked_list_implementation.hpp"
}

#endif


