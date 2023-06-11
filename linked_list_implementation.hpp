#include <stdlib.h>

template <typename TYPE>
linkedList<TYPE>::linkedList(userPrintDataFunc _print_data)
{
	Head = NULL;
	Tail = NULL;

	print_data = _print_data;
}

template <typename TYPE>
link<TYPE> *linkedList<TYPE>::append(TYPE Data)
{
	link<TYPE> *NewLink = (link<TYPE> *) malloc(sizeof(link<TYPE>));
	NewLink->Data = Data;
	NewLink->Next = NULL;
	NewLink->Prev = NULL;

	if(Tail)
	{
		Tail->Prev = NewLink;
		NewLink->Next = Tail;
	}
	else
	{
		// list is empty
		Head = Tail = NewLink;
	}
	Tail = NewLink;

	return NewLink;
}

// O(1)!
template <typename TYPE>
void linkedList<TYPE>::remove(link<TYPE> *Link)
{
	if(Link->Prev) Link->Prev->Next = Link->Next;
	if(Link->Next) Link->Next->Prev = Link->Prev;

	if(!Link->Prev && !Link->Next)
	{
		Head = NULL;
		Tail = NULL;
	}
	else if(!Link->Prev)
	{
		Tail = Link->Next;
	}
	else if(!Link->Next)
	{
		Head = Link->Prev;
	}

	free(Link);
}

template <typename TYPE>
void linkedList<TYPE>::print()
{
	if(print_data)
	{
		//for(link *Ptr = Tail; Ptr; Ptr = Ptr->Next)
		for(link<TYPE> *Ptr = Head; Ptr; Ptr = Ptr->Prev)
		{
			print_data(&Ptr->Data);
//			print_data(Ptr->Data);
		}
	}
	else
	{
		printf("No function given to print data!\n");
	}
	printf("---\n");
}


