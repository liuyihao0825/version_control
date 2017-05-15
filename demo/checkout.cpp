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
    const char* repo_path = "/home/liuyihao/workspace/configservice_meta/.git";

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

    git_strarray array = {0};
    if (0 > init_git_strarray(filename_list, &array)) {
        printf("init_git_strarray fail.\n");
        error = -1;
    }

    if (0 > git_reset_default(repo, target, &array)) {
        cout << "git_reset fail, reset_commitid:" << reset_commitid << endl;
    }

    /*
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    //opts.paths = array;
    error = git_checkout_index(repo, NULL, &opts);
    cout << "git_checkout_index error=" << error << endl;

    // end
    if (repo) {
        git_repository_free(repo);
    }
    */

    git_libgit2_shutdown();

    return error;
}

int checkout_repo(const std::vector<std::string>& filename_list)
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

    /*
    git_object* target;
    if (0 > git_revparse_single(&target, repo, "HEAD")) {
        target = NULL;
        cout << "git_revparse_single fail, reset_commitid:" << reset_commitid << endl;
    }

    git_strarray array = {0};
    if (0 > init_git_strarray(filename_list, &array)) {
        printf("init_git_strarray fail.\n");
        error = -1;
    }

    if (0 > git_reset_default(repo, target, &array)) {
        cout << "git_reset fail, reset_commitid:" << reset_commitid << endl;
    }
    */

    /*
    git_index* index = NULL;

    if (git_repository_index(&index, repo) < 0) {
        cout << "get repo repo index fail" << endl;
    }
    */

    git_strarray array = {0};
    if (0 > init_git_strarray(filename_list, &array)) {
        printf("init_git_strarray fail.\n");
        error = -1;
    }

    //git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
    opts.checkout_strategy = GIT_CHECKOUT_FORCE;
    opts.paths = array;
    //error = git_checkout_tree(repo, target, &opts);
    error = git_checkout_head(repo, &opts);
    cout << "git_checkout_index error=" << error << endl;

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
    filename_list.push_back(argv[1]);

    int ret = checkout_repo(filename_list);

    cout << "ret=" << ret << endl;

    return 0;
}
