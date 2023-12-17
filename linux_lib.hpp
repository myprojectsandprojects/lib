#ifndef LINUX_LIB_HPP
#define LINUX_LIB_HPP

char *get_parent_path_noalloc(char *path);
char *get_parent_path_alloc(const char *path);
bool get_executable_path(char *buffer, int buffer_size);

#ifdef LINUX_LIB_INCLUDE_IMPLEMENTATION

#include <unistd.h>
// #include <fcntl.h>

bool get_executable_path(char *buffer, int buffer_size)
{
	bool ret;
	ssize_t num_copied = readlink("/proc/self/exe", buffer, buffer_size);
	if (num_copied == -1)
	{
		// something went wrong (errno should contain further info)
		ret = false;
	}
	else if (num_copied == buffer_size)
	{
		// path too long
		ret = false;
	}
	else
	{
		buffer[num_copied] = 0;
		ret = true;
	}
	return ret;
}

//@ test
char *get_parent_path_noalloc(char *path)
{
	char *slash = NULL; // this stores the last slash
	for (char *ptr = path; *ptr; ++ptr) {
		if (*ptr == '/') {
			slash = ptr;
		}
	}
	if (slash) *slash = 0;
	return path;
}

//@ test
// In my mind directory paths should end with '/'
char *get_parent_path_alloc(const char *path)
{
	const char *slash = NULL; // this stores the last slash
	for (const char *ptr = path; *ptr; ++ptr) {
		if (*ptr == '/') {
			slash = ptr;
		}
	}

	if (!slash) {
		return NULL;
	}

	int len = slash - path + 1; // number of chars before last slash + slash
	// int len = slash - path; // number of chars before last slash
	char *parent_path = (char *)malloc(len+1);

	const char *src = path;
	char *dest = parent_path;
	while(src <= slash) {
		*dest = *src;
		dest += 1; src += 1;
	}
	*dest = '\0';

	return parent_path;
}

#endif

#endif