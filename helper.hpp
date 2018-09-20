#pragma once
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>

class helper {
public:
    static void init_daemon(int with_watch_dog, const char * working, const char * crash_run)
    {
        int pid;
        if ((pid = fork())) {
            exit(0);//terminate parent process
        } else if (pid< 0) {
            exit(1);//can not fork , exit
        }
        setsid();//set first child process be new session group and process group leader
        if ((pid = fork())) {
            exit(0);//terminate first child process
        } else if (pid< 0) {
            exit(1);//fcan not fork , exit
        }
        for (int i = 0; i < NOFILE; ++i)//close all opened file desc
            close(i);
        chdir(working);//change working folder to ..
        umask(0);//reset file create mask
        if (with_watch_dog) {
            while (1) {
                int status;
                pid_t pid = fork();
                if (pid) {
                    pid_t pid_wait = wait(&status);
                    char temp[128];
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "kill -9 %d", pid);
                    ::system(temp);
                    printf("Child process %d returned!\n", pid_wait);
                    if (crash_run) { ::system(crash_run); }
                    sleep(1);
                } else
                    break;
            }
        }
    }
};

//#define WITH_BASE64
//#if defined(WITH_BASE64)
//#include <openssl/sha.h>
//#include <openssl/hmac.h>
//#include <openssl/evp.h>
//#include <openssl/bio.h>
//#include <openssl/buffer.h>
//class helper_ext {
//public:
//    static std::string base64(const  char *input, int length)
//    {
//        BIO *bmem, *b64;
//        BUF_MEM *bptr;
//        b64 = BIO_new(BIO_f_base64());
//        bmem = BIO_new(BIO_s_mem());
//        b64 = BIO_push(b64, bmem);
//        BIO_write(b64, input, length);
//        BIO_flush(b64);
//        BIO_get_mem_ptr(b64, &bptr);
//        std::string result(bptr->data, bptr->length-1);
//        BIO_free_all(b64);
//        return result;
//    }
//    static int do_sha1(const char *data, const int32_t data_len, char &result) {
//#if 1
//        int rc = 0;
//        SHA_CTX c;
//        rc = SHA1_Init(&c);
//        rc = SHA1_Update(&c, data, data_len);
//        rc = SHA1_Final((unsigned char*)&result,&c);
//        return rc;
//    }
//#else
//    const EVP_MD *md = EVP_get_digestbyname("RSA-SHA1");
//    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();
//    EVP_DigestInit_ex(mdctx, md, NULL);
//    EVP_DigestUpdate(mdctx, data, strlen(data));
//    EVP_DigestFinal_ex(mdctx, &result, 20);
//    EVP_MD_CTX_destroy(mdctx);
//    printf("Digest is: ");
//    for(i = 0; i < 20; i++) printf("%02x", result[i]);
//        printf("\n");
//#endif
//};
//#endif



template< class T, class D >// = std::default_delete<T> は VC++ 対応のため泣く泣く削除
inline std::unique_ptr<T, D> to_unique( T* p, D d = D() )
{
    return std::unique_ptr<T, D>( p, std::forward<D>(d) );
}

template<class F>
inline auto scope_exit( F f )
-> decltype( to_unique( (void*)0, std::bind( std::forward<F>(f) ) ) )
{
    void* const p = reinterpret_cast<void*>(666); // 特に意味のない値。 NULL でなければよい
    return to_unique( p, std::bind( std::forward<F>(f) ) );
}




