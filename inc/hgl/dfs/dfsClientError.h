#ifndef HGL_DFS_CLIENT_ERROR_INCLUDE
#define HGL_DFS_CLIENT_ERROR_INCLUDE

namespace hgl
{
    namespace dfs
    {
        /**
        * 错误代码
        */
        enum ErrorCode
        {
            ecNone=0,               ///<无错误

            ecInitialNodeNameError, ///<初始节点名称错误
            ecInitialNodeIDError,   ///<初始节点ID错误
            ecConnectFailed,        ///<网络连接操作
            ecSocketSendError,      ///<网络发送出错
            ecSocketRecvError,      ///<网址接收出错
            ecLoginError,           ///<登陆错误
            ecBaseVersionOutdate,   ///<基础版本过期
            ecCreateFileError,      ///<创建文件失败
            ecDeleteFileFailed,     ///<删除文件失败
            ecFileLock,             ///<文件在锁定状态

            ecEnd                   ///<未知错误
        };//enum ErrorCode
    }//namespace dfs
}//namespace hgl
#endif//HGL_DFS_CLIENT_ERROR_INCLUDE
