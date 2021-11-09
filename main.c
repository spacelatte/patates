
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifndef RECURSIVE
#define RECURSIVE
#endif

#ifndef DEFAULT_DIR
#define DEFAULT_DIR "."
#endif

typedef struct Directory {
	DIR *dirptr;
	struct dirent *dirent;
	unsigned long dirlength;
	char *path;
	char null;
} dir_t;

void dir_free(dir_t *dir) {
	if(!dir) return;
	if(dir->dirptr) closedir(dir->dirptr);
	if(dir->dirent) free(dir->dirent);
	if(dir->path) free(dir->path);
	free(dir);
	return;
}

dir_t* dir_make(const char *full_path) {
	if(!full_path || !*full_path) {
		return NULL;
	}
	dir_t *dir = (dir_t*) calloc(1, sizeof(dir_t));
	dir->dirptr = opendir(full_path);
	if(!dir->dirptr) goto cleanup;
	dir->dirlength = 0;
	dir->dirent = NULL;
	struct dirent *entry;
	while(entry = readdir(dir->dirptr)) {
		if(entry->d_name[0] == '.' || !strlen(entry->d_name)) {
			continue;
		}
		dir->dirent = realloc(dir->dirent, sizeof(struct dirent) * ++dir->dirlength);
		memcpy(&dir->dirent[dir->dirlength-1], entry, sizeof(struct dirent));
		continue;
	}
	//"mert0"
	// 01234
	// len:4
	unsigned long full_path_length = strlen(full_path);
	dir->path = (char*) calloc(
		2 + full_path_length,
		sizeof(char)
	);
	snprintf(dir->path, full_path_length+2, "%s/", full_path);
	return dir;
	cleanup:
	dir_free(dir);
	return NULL;
}

const char* dir_str(dir_t *dir) {
	unsigned long length = 2 + strlen(dir->path) + strlen(dir->dirent->d_name);
	char *string = (char*) calloc(length, sizeof(char));
	snprintf(string, length, "%s/%s", dir->path, dir->dirent->d_name);
	return string;
}

typedef struct List {
	struct List *next;
	void *data;
} list_t;

list_t* list_make(void *data, list_t *next) {
	list_t *list = (list_t*) calloc(1, sizeof(list_t));
	list->next = next;
	list->data = data;
	return list;
}

void list_add(list_t **list, list_t *node) {
	if(!list || !node) return;
	list_t *orig = node;
	while(node && node->next) node = node->next;
	node->next = *list;
	*list = orig;
	return;
}

void list_free(list_t **list) {
	if(!list || !*list) {
		return;
	}
	#ifdef RECURSIVE
	list_t *node = *list;
	if(node->next) list_free(&node->next);
	free(node->data);
	#else
	list_t *next = NULL;
	for(list_t *tmp=(*list); tmp; tmp=next) {
		next = tmp->next;
		free(tmp->data);
		free(tmp);
		continue;
	}
	#endif
	free(*list);
	*list = NULL;
	return;
}

list_t *glob = NULL;
list_t* find(char *name, dir_t *target, list_t *parent) {
	if(!name || !*name) {
		return NULL;
	}
	list_t *results = parent;
	dir_t *curdir = target ? target : dir_make(DEFAULT_DIR);
	for(int i=0; i<curdir->dirlength; i++) {
		struct dirent sub = curdir->dirent[i];
		unsigned sub_len = strlen(sub.d_name);
		unsigned fullpath_len = strlen(curdir->path) + sub_len + 1;
		char *fullpath = (char*) calloc(fullpath_len, sizeof(char));
		strncat(fullpath, curdir->path, strlen(curdir->path));
		strncat(fullpath, sub.d_name, sub_len);
		if(!strncmp(name, sub.d_name, sizeof(sub.d_name))) {
			char *found = (char*) calloc(fullpath_len, sizeof(char));
			strncpy(found, fullpath, fullpath_len);
			list_add(&results, list_make(found, NULL));
		}
		struct stat stats; 
		if(lstat(fullpath, &stats)) goto bypass;
		if(S_ISLNK(stats.st_mode))  goto bypass;
		if(S_ISDIR(stats.st_mode)) {
			dir_t *subtarget = dir_make(fullpath);
			list_t *found = find(name, subtarget, NULL);
			list_add(&results, found);
			dir_free(subtarget);
		}
		bypass:
		free(fullpath);
		continue;
	}
	if(!target) dir_free(curdir);
	return results;
}

void list_print(list_t *list) {
	while(list) {
		if(list->data) printf("%s\n", list->data);
		list = list->next;
	}
}

int main(int argc, char **argv) {
	for(int i=1; i<argc; i++) {
		list_t *found = find(argv[i], NULL, glob);
		list_print(found);
		list_free(&found);
		continue;
	}
	return 0;
}