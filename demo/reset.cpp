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
#include <vector>
using namespace std;

int cred_acquire_cb(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{

    git_cred_ssh_key_new(cred, username_from_url, NULL, "/home/liuyihao/.ssh/id_rsa", NULL);

    return 0;
}

int init_git_strarray(const std::vector<std::string>& filename_list,
                      git_strarray* array)
{
    size_t count = filename_list.size();
    if (0 == count) {
        printf("init_git_strarray fail: filename_list size is 0.\n");
        return -1;
    }

    array->count = count;
    array->strings = (char **)malloc(sizeof(char *) * array->count);

    if (NULL == array->strings) {
        printf("init_git_strarray fail: malloc array->strings is null.\n");
        return -1;
    }

    for (size_t i = 0; i < array->count; ++i) {
        array->strings[i] = (char *)filename_list[i].c_str();
    }

    return 0;
}

int reset_commit_repo(const char* reset_commitid,
                      const std::vector<std::string>& filename_list)
{
    int error;

    git_repository* repo;
    const char* repo_path = "/opt/meituan/liuyihao_cstest/server/search_confsev_meta/.git";

    git_libgit2_init();
    error = git_repository_open(&repo, repo_path);
    if (error < 0) {
        repo = NULL;
        cout << "open configservice_meta repo fail" << endl;
    }

    git_object* target;
    if (0 > git_revparse_single(&target, repo, reset_commitid)) {
        target = NULL;
        cout << "git_revparse_single fail, reset_commitid:" << reset_commitid << endl;
    }

    /*
    git_strarray array = {0};
    if (0 > init_git_strarray(filename_list, &array)) {
        printf("init_git_strarray fail.\n");
        error = -1;
    }

    if (0 > git_reset_default(repo, target, &array)) {
        cout << "git_reset fail, reset_commitid:" << reset_commitid << endl;
    }

    // git commit
    git_oid tree_id;
    git_tree *tree;
    git_index* index;

    git_signature *sig;
    if (git_signature_default(&sig, repo) < 0) {
        printf("Unable to create a commit signature\n");
    }

    if (git_repository_index(&index, repo) < 0) {
        cout << "get repo repo index fail" << endl;
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

    // git checkout
    git_checkout_options co_opts = GIT_CHECKOUT_OPTIONS_INIT;
    co_opts.checkout_strategy = GIT_CHECKOUT_FORCE;
    co_opts.paths = array;
    //error = git_checkout_tree(repo, target, &opts);
    error = git_checkout_head(repo, &co_opts);
    cout << "git_checkout_head error=" << error << endl;

    // git push
    git_push_options push_opts = GIT_PUSH_OPTIONS_INIT;
    git_remote* remote = NULL;

    if (git_remote_lookup(&remote, repo, "origin") < 0) {
        cout << "git remote lookup fail" << endl;
    }

    const char* refs[] {"refs/heads/master:refs/heads/master"};
    git_strarray strarr = {(char **)refs, 1};

    push_opts.callbacks.credentials = cred_acquire_cb;
    if (git_remote_push(remote, &strarr, &push_opts)) {
        cout << "git remote push fail" << endl;
        const git_error *e = giterr_last();
        cout << "Error: " << error << " / " << e->klass << " : " << e->message << endl;
    }
    */

    // end
    if (repo) {
        git_repository_free(repo);
    }

    git_libgit2_shutdown();

    return error;
}

int main(int argc, char **argv)
{
    std::vector<std::string> filename_list;
    filename_list.push_back(argv[2]);

    int ret = reset_commit_repo(argv[1], filename_list);

    cout << "ret=" << ret << endl;

    return 0;
}
