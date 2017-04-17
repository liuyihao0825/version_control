/*
 * libgit2 "add" example - shows how to modify the index
 *
 * Written by the libgit2 contributors
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <iostream>
#include <assert.h>
#include "git2.h"
#include <string.h>
using namespace std;

/* Forward declarations for helpers */
void init_array(git_strarray *array, int argc, char **argv);

int main (int argc, char** argv)
{
	git_index_matched_path_cb matched_cb = NULL;
	git_repository *repo = NULL;
	git_index *index;
	git_strarray array = {0};
    void* payload = NULL;

	git_libgit2_init();

    int count = 1;
	init_array(&array, argc-count, argv+count);

    if (0 > git_repository_open(&repo, "/home/liuyihao/workspace/configservice/search/configservice/gitlib/confsev-demo/configservice_meta/.git")) {
        repo = NULL;
        cout << "No git repository" << endl;
    }

    if (0 > git_repository_index(&index, repo)) {
        repo = NULL;
        cout << "Could not open repository index" << endl;
    }

    git_index_update_all(index, &array, matched_cb, &payload);

	git_index_write(index);
	git_index_free(index);
	git_repository_free(repo);

	git_libgit2_shutdown();

	return 0;
}

void init_array(git_strarray *array, int argc, char **argv)
{
	unsigned int i;

	array->count = argc;
	array->strings = (char **)malloc(sizeof(char*) * array->count);
	assert(array->strings!=NULL);

	for(i=0; i<array->count; i++) {
		array->strings[i]=argv[i];
	}

	return;
}

void print_usage(void)
{
	fprintf(stderr, "usage: add [options] [--] file-spec [file-spec] [...]\n\n");
	fprintf(stderr, "\t-n, --dry-run    dry run\n");
	fprintf(stderr, "\t-v, --verbose    be verbose\n");
	fprintf(stderr, "\t-u, --update     update tracked files\n");
	exit(1);
}
