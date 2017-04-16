/*================================================================
*   Copyright (C) 2015 All rights reserved.
*
*   文件名称：test.cpp
*   创 建 者：
*   创建日期：2017年03月30日
*   描    述：
*
================================================================*/

#include <iostream>
#include "test.h"
#include "git2.h"
using namespace std;

struct opts {
    int no_options;
    int quiet;
    int bare;
    int initial_commit;
    uint32_t shared;
    const char* temp;
    const char* gitdir;
    const char* dir;
};

static void create_initial_commit(git_repository* repo);
static void parse_opts(struct opts* o, int argc, char *argv[]);

static void create_initial_commit(git_repository* repo)
{
    git_signature *sig;
    git_index *index;
    git_oid tree_id, commit_id;
    git_tree *tree;

    if (git_signature_default(&sig, repo) < 0) {
        printf("Unable to create a commit signature");
    }

    if (git_repository_index(&index, repo) < 0) {
        printf("Unable to open repository index");
    }

    if (git_index_write_tree(&tree_id, index) < 0) {
        printf("Unable to write initial tree from index");
    }

    git_index_free(index);

    if (git_tree_lookup(&tree, repo, &tree_id) < 0) {
        printf("Unable to look up initial tree");
    }

    if (git_commit_create_v(&commit_id, repo, "HEAD", sig, sig,
                NULL, "initial commit 2", tree, 0) < 0) {
        printf("Unable to create initial commit");
    }

    git_tree_free(tree);
    git_signature_free(sig);
}

int runGitRepo()
{
    int error = 0;

    git_libgit2_init();

    git_repository *repo = NULL;

    error = git_repository_init(&repo, ".", false);

    if (0 == error) {
        create_initial_commit(repo);
    } else {
        cout << "error=" << error << endl;
    }

    git_repository_free(repo);

    git_libgit2_shutdown();

    return error;
}

int main(int argc, char **argv)
{
    int ret = runGitRepo();

    cout << "ret=" << ret << endl;

    return 0;
}
