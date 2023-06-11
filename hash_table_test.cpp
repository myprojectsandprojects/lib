#include <stdio.h>
#include "hash_table.hpp"
#include "hash_table.hpp"

struct greeting
{
	const char *Msg;
};

struct oneChars {char A;};
struct twoChars {char A,B;};
struct threeChars {char A,B,C;};

//struct movie
//{
//	const char *Title;
//	int ReleaseYear;
//};

void print_key(const void *Key)
{
	printf("Msg: %s", ((greeting *)Key)->Msg);
}

void print_value(const void *Value)
{
	printf("%s", *((const char **)Value));
}

void test_remove()
{
//	hashTable<int, const char *> Table(5);
////	int Key1 = 13 + 0*16;
////	int Key2 = 14 + 1*16;
////	int Key3 = 14 + 2*16;
////	int Key4 = 14 + 3*16;
////	int Key5 = 17 + 4*16;
////	int Key6 = 12 + 5*16;
////	int Key7 = 12 + 6*16;
////	Table.put(Key1, "One");
////	Table.put(Key2, "Two");
////	Table.put(Key3, "Three");
////	Table.put(Key4, "Four");
////	Table.put(Key5, "Five");
////	Table.put(Key6, "Six");
////	Table.put(Key7, "Seven");
//	int Key1 = 11 + 1 * 16;
//	int Key2 = 12 + 2 * 16;
//	int Key3 = 9 + 3 * 16;
//	int Key4 = 15 + 4 * 16;
//	int Key5 = 15 + 5 * 16;
//	int Key6 = 15 + 6 * 16;
//	int Key7 = 15 + 7 * 16;
//	Table.put(Key1, "One");
//	Table.put(Key2, "Two");
//	Table.put(Key3, "Three");
//	Table.put(Key4, "Four");
//	Table.put(Key5, "Five");
//	Table.put(Key6, "Six");
//	Table.put(Key7, "Seven");
//	Table.print();
//	Table.remove(Key3);
//	Table.print();
//	{
//		int Key;
//		const char *Value;
//		Key = Key1; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		Key = Key2; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		Key = Key3; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		Key = Key4; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		Key = Key5; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		Key = Key6; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		Key = Key7; if (Table.get(Key, &Value)) printf("%d: %s\n", Key, Value); else printf("Key not in the table: %d\n", Key);
//		printf("---\n\n");
//	}
}

int main()
{
	greeting PortugeseGreeting = {"Ola mundo!"};
	greeting SpanishGreeting = {"Hola mundo!"};
	greeting ItalianGreeting = {"Ciao mondo!"};

	hashTable<greeting, const char *> Greetings;
//	hashTable<greeting, const char *> Greetings(3);
//	hashTable<greeting, const char *> Greetings(print_key, print_value);
//	hashTable<greeting, const char *> Greetings(2, print_key, print_value);
//	hashTable<greeting, const char *> Greetings(3, print_key);
	Greetings.put(PortugeseGreeting, "Portugese");
	Greetings.put(SpanishGreeting, "Spanish");
	Greetings.put(ItalianGreeting, "Italian");
	Greetings.print();

//	greeting *Key = &PortugeseGreeting;
//	greeting *Key = &SpanishGreeting;
//	greeting *Key = &ItalianGreeting;
//	greeting *Key = (greeting *)&Greetings;
//	const char *Greeting;
//	bool GotIt = Greetings.get(Key, &Greeting);
//	if(GotIt)
//	{
//		printf("%p: %s\n", Key, Greeting);
//	}
//	else
//	{
//		printf("%p: -\n", Key);
//	}

//	greeting PortugeseGreeting = {"Ola mundo!"};
//	greeting SpanishGreeting = {"Hola mundo!"};
//	greeting ItalianGreeting = {"Ciao mondo!"};
//
//	hashTable<greeting *, const char *> Greetings;
//	Greetings.put(&PortugeseGreeting, "Portugese");
//	Greetings.put(&SpanishGreeting, "Spanish");
//	Greetings.put(&ItalianGreeting, "Italian");
//	Greetings.print();
//
////	greeting *Key = &PortugeseGreeting;
////	greeting *Key = &SpanishGreeting;
////	greeting *Key = &ItalianGreeting;
//	greeting *Key = (greeting *)&Greetings;
//	const char *Greeting;
//	bool GotIt = Greetings.get(Key, &Greeting);
//	if(GotIt)
//	{
//		printf("%p: %s\n", Key, Greeting);
//	}
//	else
//	{
//		printf("%p: -\n", Key);
//	}

//	greeting PortugeseGreeting = {"Ola mundo!"};
//	greeting SpanishGreeting = {"Hola mundo!"};
//	greeting ItalianGreeting = {"Ciao mondo!"};
//
//	hashTable<const char *, greeting> Greetings;
//	Greetings.put("Portugese", PortugeseGreeting);
//	Greetings.put("Spanish", SpanishGreeting);
//	Greetings.put("Italian", ItalianGreeting);
//
//	const char *Key = "Italian";
//	greeting Greeting;
//	bool GotIt = Greetings.get(Key, &Greeting);
//	if(GotIt)
//	{
//		printf("%s: %s\n", Key, Greeting.Msg);
//	}
//	else
//	{
//		printf("%s: -\n", Key);
//	}

//	Greetings.print();

//
//	greeting PortugeseGreeting = {"Ola mundo!"};
//	greeting SpanishGreeting = {"Hola mundo!"};
//	greeting ItalianGreeting = {"Ciao mondo!"};
//
//	hashTable<const char *, greeting *> Greetings(1);
//	Greetings.put("Portugese", &PortugeseGreeting);
//	Greetings.put("Spanish", &SpanishGreeting);
//	Greetings.put("Italian", &ItalianGreeting);
//
//	const char *Key = "Portugese";
//	greeting *Greeting;
//	bool GotIt = Greetings.get(Key, &Greeting);
//	if(GotIt)
//	{
//		printf("%s: %s\n", Key, Greeting->Msg);
//	}
//	else
//	{
//		printf("%s: -\n", Key);
//	}
//
//	Greetings.print();

//	enum language
//	{
//		ENGLISH = 0,
//		PORTUGESE,
//		SPANISH,
//		ITALIAN,
//	};
//
//	greeting PortugeseGreeting = {"Ola mundo!"};
//	greeting SpanishGreeting = {"Hola mundo!"};
//	greeting ItalianGreeting = {"Ciao mondo!"};
//
//	hashTable<int, greeting *> Greetings(1);
//	Greetings.put(PORTUGESE, &PortugeseGreeting);
//	Greetings.put(SPANISH, &SpanishGreeting);
//	Greetings.put(ITALIAN, &ItalianGreeting);
//
//	language Key = ITALIAN;
//	greeting *Greeting;
//	bool GotIt = Greetings.get(Key, &Greeting);
//	if(GotIt)
//	{
//		printf("%d: %s\n", Key, Greeting->Msg);
//	}
//	else
//	{
//		printf("%d: -\n", Key);
//	}
//
//	Greetings.print();
}


