#ifndef AUTENTICATOR_H_
#define AUTENTICATOR_H_

#include <list>
#include "../util/yadmaptr.h"
#include "../database/idatabase.h"
#include "../thread/mutex.h"
#include "iautenticator.h"


//----------------------------------------------------------------------------------------------
typedef struct
{
    char    Spected[300];
    char     Data[300];
}TWriteData;

//----------------------------------------------------------------------------------------------
typedef struct
{
    int        id_server;
    char     Server[400];
    char    UserName[50];
    char     Password[50];
    char    LastCookie[500];
    char    LoginUrl[100];
    char    PasswordVar[50];
    char    UserNameVar[50];
    char     Token[100];
    char    Prefix[100];
    char    Additional[100];
} TAutenticationInfo;

//----------------------------------------------------------------------------------------------
class CAutenticator: public IAutenticator
{
public:
    CAutenticator( yadmaptr<IDataBase> DataBase );
    std::string GetAutenticationCookie( const std::string& Url, std::string* Prefix = (std::string*)0 );
    std::string GetLastCookie( const std::string& Url, std::string* Prefix = (std::string*)0 );
    void Update();
private:
    bool Compatible( const std::string& Server, const std::string& Url );
    //static size_t WriteData( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData );
    static size_t WriteNull( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData );
    //bool CheckCookie( const std::string& ValidationUrl, const std::string& Cookie, const std::string& Token );
    std::string NewAutentication( std::list<TAutenticationInfo>::iterator Info );
    
private:
    CMutex                            m_mtxAutenticator;
    yadmaptr<IDataBase>             m_DataBase;
    std::list<TAutenticationInfo>     m_Infos;
};

#endif /*AUTENTICATOR_H_*/
