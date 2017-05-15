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

    char shortsha[100] = {0};
    git_oid_tostr(shortsha, 99, &tree_id);
    cout << "shortsha=" << shortsha << endl;

    git_index_free(index);

    if (git_tree_lookup(&tree, repo, &tree_id) < 0) {
        printf("Unable to look up initial tree");
    }

    if (git_commit_create_v(&commit_id, repo, NULL, sig, sig,
                NULL, "commit 2, modify readme", tree, 0) < 0) {
        printf("Unable to create initial commit");
    }

    git_tree_free(tree);
    git_signature_free(sig);
}

int run_git_repo()
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

static void check_error(int error_code, const char* action)
{
    const git_error* error = giterr_last();
    if (!error_code) {
        return;
    }

    printf("Error %d %s - %s\n", error_code, action,
           (error && error->message) ? error->message : "???");

    exit(1);
}

int open_git_repo()
{
    int error;

    git_repository* repo;
    const char* repo_path = "/home/liuyihao/workspace/configservice/search/configservice/gitlib/confsev-demo/configservice_meta/.git";

    git_libgit2_init();

    error = git_repository_open(&repo, repo_path);
    if (error < 0) {
        repo = NULL;
        cout << "open configservice_meta repo fail" << endl;
    }

    git_oid oid;
    //error = git_oid_fromstr(&oid, "HEAD^{tree}");
    error = git_reference_name_to_id(&oid, repo, "HEAD");;
    cout << "git_reference error = " << error << endl;

    char shortsha[10] = {0};
    git_oid_tostr(shortsha, 9, &oid);

    cout << "oid str =" << shortsha << endl;

    git_commit* commit;
    error = git_commit_lookup(&commit, repo, &oid);
    cout << "git_commit error = " << error << endl;

    /*
    git_tree* tree;
    error = git_tree_lookup(&tree, repo, &oid);
    cout << "git_tree error = " << error << endl;
    */

    //create_initial_commit(repo);

    if (repo) {
        git_repository_free(repo);
    }

    git_libgit2_shutdown();

    return error;
}

int main(int argc, char **argv)
{
    int ret = open_git_repo();

    cout << "ret=" << ret << endl;

    return 0;
}
