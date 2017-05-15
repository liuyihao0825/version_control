/*================================================================
*   Copyright (C) 2015 All rights reserved.
*
*   文件名称：git_tool.h
*   创 建 者：
*   创建日期：2017年05月03日
*   描    述：使用libgit2封装的git tool
*
================================================================*/
#pragma once
#include <vector>
#include <string>
#include "git2.h"

// 单例使用,全局只有一个GitTool实例
class GitTool
{
public:
    GitTool();

    GitTool(const std::string& git_repo_path);

    virtual ~GitTool();

    bool isValid();

    // 必须先调用本函数启动,如果不调用,使用默认构造函数中创建的repo_path
    int start(const std::string& git_repo_path);

    int gitAdd(const std::vector<std::string>& filename_list);

    int gitCommit(const std::string& author,
                  const std::string& desc,
                  std::string& last_commitid,
                  std::string& curr_commitid);

    int gitPush();

    int gitReset(const std::string& reset_commitid,
                 const std::vector<std::string>& filename_list);

    int gitCheckout(const std::vector<std::string>& filename_list);

    std::string getRepoPath() const;

private:
    int init_git_strarray(const std::vector<std::string>& filename_list,
                          git_strarray* array);

private:
    std::string m_repo_path;
    bool m_is_valid;
};
