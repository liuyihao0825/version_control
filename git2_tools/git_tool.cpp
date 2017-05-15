/*================================================================
*   Copyright (C) 2015 All rights reserved.
*
*   文件名称：git_tool.cpp
*   创 建 者：
*   创建日期：2017年05月03日
*   描    述：
*
================================================================*/
#include "git_tool.h"

static int cred_acquire_cb(git_cred **cred, const char *url,
        const char *username_from_url, unsigned int allowed_types, void *payload)
{

    git_cred_ssh_key_new(cred, username_from_url, NULL, "/home/sankuai/.ssh/id_rsa", NULL);

    return 0;
}

GitTool::GitTool()
{
    git_libgit2_init();

    m_repo_path = "/opt/meituan/configservice/server/meta/.git";
    m_is_valid = false;
}

GitTool::GitTool(const std::string& git_repo_path)
    : m_repo_path(git_repo_path)
{
    git_libgit2_init();
    m_is_valid = false;
}

GitTool::~GitTool()
{
    git_libgit2_shutdown();
    m_is_valid = false;
}

bool GitTool::isValid()
{
    return m_is_valid;
}

int GitTool::start(const std::string& git_repo_path)
{
    if (git_repo_path.empty()) {
        printf("start GitTool fail, git_repo_path:%s\n", git_repo_path.c_str());
        m_repo_path = "";
        return -1;
    } else {
        m_repo_path = git_repo_path;
        m_is_valid = true;
    }

    return 0;
}

int GitTool::gitAdd(const std::vector<std::string>& filename_list)
{
    int err = 0;

    git_index_matched_path_cb matched_cb = NULL;
    git_repository* repo = NULL;
    git_index* index = NULL;
    git_strarray array = {0};
    void* payload = NULL;

    do {
        if (0 == filename_list.size()) {
            printf("git_add, filename_list size is 0.\n");
            err = -1;
            break;
        }

        if (0 > init_git_strarray(filename_list, &array)) {
            printf("init_git_strarray fail.\n");
            err = -1;
            break;
        }

        if (0 > git_repository_open(&repo, m_repo_path.c_str())) {
            printf("git_repository_open fail, repo_path:%s\n",
                   m_repo_path.c_str());
            repo = NULL;
            err = -1;
            break;
        }

        if (0 > git_repository_index(&index, repo)) {
            printf("git_repository_index fail.\n");
            index = NULL;
            err = -1;
            break;
        }

        if (0 > git_index_add_all(index, &array, GIT_INDEX_ADD_DEFAULT,
                                  matched_cb, &payload)) {
            printf("git_index_add_all fail.\n");
            err = -1;
            break;
        }

        git_index_write(index);

        err = 0;

    } while (0);

    // delete 资源
    if (index) {
        git_index_free(index);
    }
    if (repo) {
        git_repository_free(repo);
    }

    return err;
}

int GitTool::gitCommit(const std::string& author,
                       const std::string& desc,
                       std::string& last_commitid,
                       std::string& curr_commitid)
{
    int err = 0;

    git_repository* repo = NULL;
    git_index* index = NULL;
    git_signature* sig = NULL;
    git_tree* tree = NULL;
    git_oid tree_id;
    git_oid head_id;
    git_oid commit_id;
    git_commit* parent = NULL;

    do {
        if (0 > git_repository_open(&repo, m_repo_path.c_str())) {
            printf("git_repository_open fail, repo_path:%s\n",
                   m_repo_path.c_str());
            repo = NULL;
            err = -1;
            break;
        }

        if (0 > git_repository_index(&index, repo)) {
            printf("git_repository_index fail.\n");
            index = NULL;
            err = -1;
            break;
        }

        std::string email = author + "@meituan.com";
        if (0 > git_signature_now(&sig, author.c_str(), email.c_str())) {
            printf("git_signature_default fail.\n");
            sig = NULL;
            err = -1;
            break;
        }

        if (0 > git_index_write_tree(&tree_id, index)) {
            printf("git_index_write_tree fail.\n");
            err = -1;
            break;
        }

        if (0 > git_tree_lookup(&tree, repo, &tree_id) < 0) {
            printf("git_tree_lookup fail.\n");
            err = -1;
            break;
        }

        // 第一次初始化时，没有 HEAD 和 parent
        if (0 > git_reference_name_to_id(&head_id, repo, "HEAD")) {
            printf("git_reference_name_to_id: first commit, no HEAD\n");
        } else {
            if (0 > git_commit_lookup(&parent, repo, &head_id)) {
                printf("git_commit_lookup: first commit, no parent.\n");
            }
        }

        if (0 > git_commit_create_v(&commit_id, repo, "HEAD", sig,
                                    sig, NULL, desc.c_str(),
                                    tree, 1, parent)) {
            printf("git_commit_create_v fail.\n");
            err = -1;
            break;
        }

        char last[50] = {0};
        char curr[50] = {0};
        if (NULL == parent) {
            last_commitid = "";
        } else {
            git_oid_tostr(last, 50, &head_id);
            last_commitid = last;
        }

        git_oid_tostr(curr, 50, &commit_id);
        curr_commitid = curr;

        err = 0;

    } while (0);

    // delete 资源
    if (index) {
        git_index_free(index);
    }
    if (repo) {
        git_repository_free(repo);
    }

    return err;
}

int GitTool::gitPush()
{
    int err = 0;

    git_repository* repo = NULL;
    git_remote* remote = NULL;
    git_push_options opts = GIT_PUSH_OPTIONS_INIT;

    do {
        if (0 > git_repository_open(&repo, m_repo_path.c_str())) {
            printf("git_repository_open fail, repo_path:%s\n",
                   m_repo_path.c_str());
            repo = NULL;
            err = -1;
            break;
        }

        if (0 > git_remote_lookup(&remote, repo, "origin")) {
            printf("git_remote_lookup fail.\n");
            err = -1;
            break;
        }

        const char* refs[] {"refs/heads/master:refs/heads/master"};
        git_strarray strarr = {(char **)refs, 1};

        opts.callbacks.credentials = cred_acquire_cb;
        if (0 > git_remote_push(remote, &strarr, &opts)) {
            const git_error *e = giterr_last();
            printf("git_remote_push fail: %d / %d : %s\n", err,
                   e->klass, e->message);
            err = -1;
            break;
        }

        err = 0;
    } while (0);

    // delete 资源
    if (repo) {
        git_repository_free(repo);
    }

    return err;
}

int GitTool::gitReset(const std::string& reset_commitid,
                      const std::vector<std::string>& filename_list)
{
    int err = 0;

    git_repository* repo = NULL;
    git_strarray array = {0};
    git_object* target = NULL;

    do {
        if (reset_commitid.empty()) {
            printf("reset_commitid is invalid, commitid:%s\n",
                   reset_commitid.c_str());
            err = -1;
            break;
        }

        if (0 > init_git_strarray(filename_list, &array)) {
            printf("init_git_strarray fail.\n");
            err = -1;
            break;
        }

        if (0 > git_repository_open(&repo, m_repo_path.c_str())) {
            printf("git_repository_open fail, repo_path:%s\n",
                   m_repo_path.c_str());
            repo = NULL;
            err = -1;
            break;
        }

        if (0 > git_revparse_single(&target, repo, reset_commitid.c_str())) {
            target = NULL;
            printf("git_revparse_single fail, reset_commitid:%s\n",
                   reset_commitid.c_str());
            err = -1;
            break;
        }

        if (0 > git_reset_default(repo, target, &array)) {
            printf("git_reset fail, reset_commitid:%s\n",
                   reset_commitid.c_str());
            err = -1;
            break;
        }

        std::string last;
        std::string curr;
        if (0 > gitCommit("admin", "reset operation", last, curr)) {
            printf("git_commit_all fail\n");
            err = -1;
            break;
        }

        if (0 > gitCheckout(filename_list)) {
            printf("git_checkout_file fail\n");
            err = -1;
            break;
        }

        if (0 > gitPush()) {
            printf("git_push fail\n");
            err = -1;
            break;
        }

        err = 0;
    } while (0);

    // delete 资源
    if (repo) {
        git_repository_free(repo);
    }

    return err;
}

// filename_list 可以为空，为空表示 checkout 当前目录下所有可以co的文件
int GitTool::gitCheckout(const std::vector<std::string>& filename_list)
{
    int err = 0;

    git_repository* repo = NULL;
    git_strarray array = {0};

    do {
        if (0 > init_git_strarray(filename_list, &array)) {
            printf("init_git_strarray fail.\n");
            err = -1;
            break;
        }

        if (0 > git_repository_open(&repo, m_repo_path.c_str())) {
            printf("git_repository_open fail, repo_path:%s\n",
                   m_repo_path.c_str());
            repo = NULL;
            err = -1;
            break;
        }

        git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
        opts.checkout_strategy = GIT_CHECKOUT_FORCE;

        // filename_list不为空，则指定array
        if (0 < array.count) {
            opts.paths = array;
        }

        if (0 > git_checkout_head(repo, &opts)) {
            printf("git_checkout_head fail\n");
            err = -1;
            break;
        }

        err = 0;
    } while (0);

    // delete 资源
    if (repo) {
        git_repository_free(repo);
    }

    return err;
}

std::string GitTool::getRepoPath() const
{
    return m_repo_path;
}

// filename_list为空，则默认array->count = 0
int GitTool::init_git_strarray(const std::vector<std::string>& filename_list,
                               git_strarray* array)
{
    size_t count = filename_list.size();
    if (0 == count) {
        //printf("init_git_strarray: filename_list size is 0.\n");
        array->count = 0;
        array->strings = NULL;
        return 0;
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
