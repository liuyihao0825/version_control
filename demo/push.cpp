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

static void create_initial_commit(git_repository* repo);

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

/*
int commit_git_repo()
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

    git_signature *sig;
    if (git_signature_default(&sig, repo) < 0) {
        printf("Unable to create a commit signature\n");
    }

    git_oid tree_id;
    git_tree *tree;
    //error = git_oid_fromstr(&tree_id, "HEAD^{tree}");
    //error = git_reference_name_to_id(&tree_id, repo, "HEAD");;
    git_oid_fromstr(&tree_id, "6b1dce18fb8e5de9df4fe718234b0c72b648d649");
    if (0 > error) {
        cout << "get repo head tree_id fail" << endl;
    }
    char shortsha[100] = {0};
    git_oid_tostr(shortsha, 50, &tree_id);

    cout << "tree_id str =" << shortsha << endl;

	error = git_tree_lookup(&tree, repo, &tree_id);
    if (0 > error) {
        cout << "git_tree_lookup fail" << endl;
    }

    //cout << "git_reference error = " << error << endl;

    //char shortsha[10] = {0};
    //git_oid_tostr(shortsha, 9, &tree_id);

    //cout << "tree_id str =" << shortsha << endl;

    git_oid parent_id;
    git_commit *parent;
    error = git_commit_lookup(&parent, repo, &tree_id);
    if (0 > error) {
        cout << "get commit lookup parent fail" << endl;
    }

    git_oid commit_id;
    if (git_commit_create_v(&commit_id, repo, NULL, sig, sig,
                NULL, "commit v5, modify readme", tree, 0) < 0) {
        printf("Unable to create initial commit\n");
    }

    char shortsha2[10] = {0};
	git_oid_tostr(shortsha2, 9, &commit_id);
	printf("New Commit: %s\n", shortsha2);

    if (repo) {
        git_repository_free(repo);
    }

    git_libgit2_shutdown();

    return error;
}
*/

int index_matched_path_cb(const char* path,
                          const char* matched_pathspec,
                          void* payload) {
  cout << "update_all matched: " << path << endl;
  return 0;
}

int cred_acquire_cb(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{

    git_cred_ssh_key_new(cred, username_from_url, NULL, "/home/sankuai/.ssh/id_rsa", NULL);

    return 0;
}

/*
int commit_git_repo()
{
    int error;

    git_repository* repo;
    const char* repo_path = "/home/liuyihao/workspace/configservice_meta/.git";

    git_libgit2_init();

    error = git_repository_open(&repo, repo_path);
    if (error < 0) {
        repo = NULL;
        cout << "open configservice_meta repo fail" << endl;
    }

    git_signature *sig;
    if (git_signature_default(&sig, repo) < 0) {
        printf("Unable to create a commit signature\n");
    }

    git_oid tree_id;
    git_tree *tree;
    git_index* index;

    if (git_repository_index(&index, repo) < 0) {
        cout << "get repo repo index fail" << endl;
    }

    if (git_index_read(index, 1) < 0) {
        cout << "get index read fail" << endl;
    }

    const char* paths[] = {"."};
    git_strarray array = {(char**)paths, 1};
    if (git_index_add_all(index, &array, GIT_INDEX_ADD_DEFAULT,
                          index_matched_path_cb, NULL) < 0) {
        cout << "get update all fail" << endl;
    }

    if (git_index_write_tree(&tree_id, index) < 0) {
        cout << "Unable to write tree from index" << endl;
    }

    if (git_tree_lookup(&tree, repo, &tree_id) < 0) {
        cout << "git tree lookup fail" << endl;
    }

    git_oid head_id;
    git_commit* parent = NULL;
    if (git_reference_name_to_id(&head_id, repo, "HEAD") < 0) {
        cout << "git reference to id fail" << endl;
    }

    if (git_commit_lookup(&parent, repo, &head_id) < 0) {
        cout << "git reference to id fail" << endl;
    }

    git_oid commit_id;
    if (git_commit_create_v(&commit_id, repo, "HEAD", sig,
                        sig, NULL,
                        "commit v5, modify readme",
                        tree, 1, parent) < 0) {
        cout << "git commit fail" << endl;
    }

    char shortsha2[10] = {0};
	git_oid_tostr(shortsha2, 9, &commit_id);
	printf("New Commit: %s\n", shortsha2);

    // push
    git_push_options opts = GIT_PUSH_OPTIONS_INIT;
    git_remote* remote = NULL;

    if (git_remote_lookup(&remote, repo, "origin") < 0) {
        cout << "git remote lookup fail" << endl;
    }

    const char* refs[] {"refs/heads/master:refs/heads/master"};
    git_strarray strarr = {(char **)refs, 1};

    opts.callbacks.credentials = cred_acquire_cb;
    if (git_remote_push(remote, &strarr, &opts)) {
        cout << "git remote push fail" << endl;
        const git_error *e = giterr_last();
        cout << "Error: " << error << " / " << e->klass << " : " << e->message << endl;
    }

    // end
    if (repo) {
        git_repository_free(repo);
    }

    git_libgit2_shutdown();

    return error;
}
*/

int push_git_repo()
{
    int err;

    const char* m_repo_path = "/opt/meituan/liuyihao_cstest/server/search_confsev_meta/.git";

    git_libgit2_init();

    git_repository* repo = NULL;
    git_remote* remote = NULL;
    git_push_options opts = GIT_PUSH_OPTIONS_INIT;

    do {
        if (0 > git_repository_open(&repo, m_repo_path)) {
            printf("git_repository_open fail, repo_path:%s\n",
                   m_repo_path);
            repo = NULL;
            err = -1;
        }

        if (0 > git_remote_lookup(&remote, repo, "origin")) {
            printf("git_remote_lookup fail.\n");
            err = -1;
        }

        const char* refs[] {"refs/heads/master:refs/heads/master"};
        git_strarray strarr = {(char **)refs, 1};

        opts.callbacks.credentials = cred_acquire_cb;
        if (git_remote_push(remote, &strarr, &opts)) {
            const git_error *e = giterr_last();
            printf("git_remote_push fail: %d / %d : %s\n", err,
                   e->klass, e->message);
            err = -1;
        }

        err = 0;
    } while (0);

    git_libgit2_shutdown();

    // delete 资源
    if (repo) {
        git_repository_free(repo);
    }

    return err;
}

int main(int argc, char **argv)
{
    int ret = push_git_repo();

    cout << "ret=" << ret << endl;

    return 0;
}
