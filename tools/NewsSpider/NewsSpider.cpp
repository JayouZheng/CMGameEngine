﻿#include<hgl/Console.h>
#include<hgl/webapi/UserAgentString.h>

using namespace hgl;
using namespace hgl::filesystem;
using namespace hgl::webapi;

const UTF8String InitUserAgent()
{
    FirefoxUserAgentConfig cfg;

    cfg.os              =OS_WindowsAMD64;

    cfg.os_ver.major    =10;
    cfg.os_ver.minor    =0;

    cfg.ff_ver.major    =64;
    cfg.ff_ver.minor    =0;

    return FirefoxUserAgent(cfg);
}

void news_heb(const UTF8String &user_agent,const OSString &save_path);
void news_fz(const UTF8String &user_agent,const OSString &save_path);

HGL_CONSOLE_MAIN_FUNC()
{
    const UTF8String user_agent=InitUserAgent();

    OSString cur_path;

    GetCurrentPath(cur_path);
    OSString save_doc_path=MergeFilename(cur_path,OS_TEXT("news"));

//     std::cout<<std::endl<<"哈尔滨"<<std::endl;
//     news_heb(user_agent,MergeFilename(save_doc_path,"heb"));

    std::cout<<std::endl<<"抚州"<<std::endl;
    news_fz(user_agent,MergeFilename(save_doc_path,"fz"));

    return 0;
}
