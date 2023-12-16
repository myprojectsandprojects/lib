#include <stdio.h>
#include "linked_list.hpp"

void print_str(const void *Data)
{
	printf("%s\n", *((const char **)Data));
}

void print_int(const void *Data)
{
	printf("%d\n", *((int *)Data));
}

// struct movie
// {
// 	const char *Title;
// 	int ReleaseYear;
// };

// void print_movie(const void *Ptr)
// {
// 	const movie *Movie = (const movie *)Ptr;
// 	printf("Title: %s, Released: %d\n", Movie->Title, Movie->ReleaseYear);
// }

// int main()
// {
// 	linkedList<const char *> Greetings(print_str);
// 	link<const char *> *English = Greetings.append("Hello world!");
// 	link<const char *> *Dutch = Greetings.append("Hallo Wereld!");
// 	link<const char *> *German = Greetings.append("Hallo Welt!");
// 	Greetings.print();
// 	Greetings.remove(English);
// 	Greetings.print();

// 	linkedList<int> Numbers(print_int);
// //	linkedList<int> Numbers;
// 	link<int> *Negative = Numbers.append(-123);
// 	link<int> *Positive = Numbers.append(123);
// 	link<int> *Zero = Numbers.append(0);
// 	Numbers.print();
// //	Greetings.remove(English);
// //	Greetings.print();

// 	linkedList<movie> RecentlyWatched(print_movie);
// //	linkedList<movie> RecentlyWatched;
// //	RecentlyWatched.append({.Title="The Langoliers", .ReleaseYear=1995});
// 	RecentlyWatched.append({"The Langoliers", 1995});
// 	link<movie> *ToBeDeleted = RecentlyWatched.append({"The Triangle", 2005});
// 	RecentlyWatched.print();
// 	RecentlyWatched.remove(ToBeDeleted);
// 	RecentlyWatched.print();
// }


