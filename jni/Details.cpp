#include <iostream>
#include <jni.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <tchar.h>
#include "Details.h"
#include "accctrl.h"
#include "aclapi.h"

using namespace std;

PACL SetPerm(LPCTSTR file, string user, char val, int perm, PACL pOldDACL)
{
	EXPLICIT_ACCESS eas[1];
    PACL pacl = 0;
    DWORD rc;
    long long int access_val;
    LPSTR use = const_cast<char *>(user.c_str());
    if(val=='R'||val=='r')
    	access_val=0x80000000;		//GENERIC_READ
    if(val=='W'||val=='w')
    	access_val=0x40000000;		//GENERIC_WRITE
    if(val=='A'||val=='a')
    	access_val=0x10000000;		//GENERIC_ALL
    if(perm==1)
    {
    	eas[0].grfAccessPermissions = access_val;
    	eas[0].grfAccessMode = GRANT_ACCESS;
    	eas[0].grfInheritance = NO_INHERITANCE;
    	eas[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    	eas[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    	eas[0].Trustee.ptstrName = use;
	}
    else if(perm==2)
    {
    	eas[0].grfAccessPermissions = access_val;
    	eas[0].grfAccessMode = DENY_ACCESS;
    	eas[0].grfInheritance = NO_INHERITANCE;
    	eas[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    	eas[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    	eas[0].Trustee.ptstrName = use;
	}
    rc = SetEntriesInAcl(1, eas, pOldDACL, &pacl);
    if (rc != ERROR_SUCCESS)
    {
        printf("ERROR---------SetEntriesInAcl: %u\n", rc);
        return NULL;
    }

    rc = SetNamedSecurityInfo((LPSTR)file, SE_FILE_OBJECT,
             DACL_SECURITY_INFORMATION | PROTECTED_DACL_SECURITY_INFORMATION,
             NULL, NULL, pacl, NULL);
    if (rc != ERROR_SUCCESS)
    {
        printf("ERROR---------SetNamedSecurityInfo: %u\n", rc);
        return NULL;
    }

    cout << "----------------PERMISSION GRANTED----------------\n";
    return pacl;
}

JNIEXPORT jobjectArray JNICALL Java_Details_returndays(JNIEnv *env, jobject jobj, jstring s)
{

  	jobjectArray ret;
	jstring jstr;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	DWORD dwRtnCode = 0;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD = NULL;
	PACL pOldDACL = NULL;
	int i,aceNum;
	const char* test;
	std::string reslt;

	const char* nativeString = env->GetStringUTFChars(s, 0);
	LPCSTR file = nativeString;
	// Get the handle of the file object.
	hFile = CreateFile(
                  file,
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);


	// Get the SID of the file.
	dwRtnCode = GetSecurityInfo(
                  hFile,
                  SE_FILE_OBJECT,
                  DACL_SECURITY_INFORMATION,
                  &pSidOwner,
                  NULL,
                  &pOldDACL,
                  NULL,
                  &pSD);

	PACL pAcl = pOldDACL;
	aceNum = pOldDACL->AceCount;

	ret = (jobjectArray)env->NewObjectArray(aceNum, env->FindClass("java/lang/String"), env->NewStringUTF(""));

	for (i = 0; i < aceNum; i++)
	{
	PACCESS_ALLOWED_ACE AceItem;
    ACE_HEADER *aceAddr = NULL;
	if (GetAce(pOldDACL, i, (LPVOID*)&AceItem) && GetAce(pOldDACL, i, (LPVOID*)&aceAddr))
		{
    	LPTSTR AccountBuff = NULL, DomainBuff = NULL;
        DWORD AccountBufflength = 1, DomainBufflength = 1;
        PSID_NAME_USE peUse = new SID_NAME_USE;
        PSID Sid = &AceItem->SidStart;
        LookupAccountSid(NULL, Sid, AccountBuff, (LPDWORD)&AccountBufflength, DomainBuff, (LPDWORD)&DomainBufflength, peUse);

		AccountBuff = (LPSTR)malloc(AccountBufflength * sizeof(LPSTR));
    	DomainBuff = (LPSTR)malloc(DomainBufflength * sizeof(LPSTR));

		LookupAccountSid(NULL, Sid, AccountBuff, &AccountBufflength, DomainBuff, &DomainBufflength, peUse);

		std::string acc=AccountBuff;
		std::string dom=DomainBuff;
		reslt=acc+"\\"+dom+"\\";

        ACCESS_MASK Mask = AceItem->Mask;
        if (((Mask & GENERIC_ALL) == GENERIC_ALL) || ((Mask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS)){
		 reslt=reslt+"Full Control";
		 test = reslt.c_str();
		 env->SetObjectArrayElement(ret,i,env->NewStringUTF(test));
         continue;
		 }
   		if (((Mask & GENERIC_READ) == GENERIC_READ) || ((Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
         	reslt=reslt+" Read,";
		if (((Mask & GENERIC_WRITE) == GENERIC_WRITE) || ((Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
			reslt=reslt+" Write,";
		if (((Mask & GENERIC_EXECUTE) == GENERIC_EXECUTE) || ((Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
			reslt=reslt+" Execute";
		test = reslt.c_str();
		env->SetObjectArrayElement(ret,i,env->NewStringUTF(test));
    	}
	}
	return (ret);
}

JNIEXPORT jstring JNICALL Java_Details_sayHello(JNIEnv *env, jobject thisObj, jstring inJNIStr)
{
jstring jstr=NULL;
const char *str=env->GetStringUTFChars(inJNIStr, NULL);
DWORD dwRtnCode = 0;
PSID pSidOwner = NULL;
BOOL bRtnBool = TRUE;
LPTSTR AcctName = NULL;
LPTSTR DomainName = NULL;
DWORD dwAcctName = 1, dwDomainName = 1;
SID_NAME_USE eUse = SidTypeUnknown;
HANDLE hFile;
PSECURITY_DESCRIPTOR pSD = NULL;

// Get the handle of the file object.
hFile = CreateFile(
                  str,
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);

// Check GetLastError for CreateFile error code.
if (hFile == INVALID_HANDLE_VALUE) {
          DWORD dwErrorCode = 0;

          dwErrorCode = GetLastError();
		  jstr = env->NewStringUTF("CreateFile error.\n");
          return jstr;
}

// Get the owner SID of the file.
dwRtnCode = GetSecurityInfo(
                  hFile,
                  SE_FILE_OBJECT,
                  OWNER_SECURITY_INFORMATION,
                  &pSidOwner,
                  NULL,
                  NULL,
                  NULL,
                  &pSD);

// Check GetLastError for GetSecurityInfo error condition.
if (dwRtnCode != ERROR_SUCCESS) {
          DWORD dwErrorCode = 0;

          dwErrorCode = GetLastError();
          jstr = env->NewStringUTF("GetSecurityInfo error.\n");
          return jstr;
}

// First call to LookupAccountSid to get the buffer sizes.
bRtnBool = LookupAccountSid(
                  NULL,           // local computer
                  pSidOwner,
                  AcctName,
                  (LPDWORD)&dwAcctName,
                  DomainName,
                  (LPDWORD)&dwDomainName,
                  &eUse);

// Reallocate memory for the buffers.
AcctName = (LPTSTR)GlobalAlloc(
          GMEM_FIXED,
          dwAcctName);

// Check GetLastError for GlobalAlloc error condition.
if (AcctName == NULL) {
          DWORD dwErrorCode = 0;

          dwErrorCode = GetLastError();
          jstr = env->NewStringUTF("GlobalAlloc error.\n");
          return jstr;
}

    DomainName = (LPTSTR)GlobalAlloc(
           GMEM_FIXED,
           dwDomainName);

    // Check GetLastError for GlobalAlloc error condition.
    if (DomainName == NULL) {
          DWORD dwErrorCode = 0;

          dwErrorCode = GetLastError();
          jstr = env->NewStringUTF("GlobalAlloc error.\n");
          return jstr;

    }

    // Second call to LookupAccountSid to get the account name.
    bRtnBool = LookupAccountSid(
          NULL,                   // name of local or remote computer
          pSidOwner,              // security identifier
          AcctName,               // account name buffer
          (LPDWORD)&dwAcctName,   // size of account name buffer
          DomainName,             // domain name
          (LPDWORD)&dwDomainName, // size of domain name buffer
          &eUse);                 // SID type

    // Check GetLastError for LookupAccountSid error condition.
    if (bRtnBool == FALSE) {
          DWORD dwErrorCode = 0;

          dwErrorCode = GetLastError();

          if (dwErrorCode == ERROR_NONE_MAPPED)
              jstr = env->NewStringUTF("Account owner not found for specified SID.\n");
          else
				jstr = env->NewStringUTF("Error in LookupAccountSid.\n");
          return jstr;

    } else if (bRtnBool == TRUE)

        {// Print the account name.
		jstr = env->NewStringUTF(AcctName);
		env->ReleaseStringUTFChars(inJNIStr, str);
	}
    return jstr;
}

JNIEXPORT void JNICALL Java_Details_modifyACL(JNIEnv *env, jobject thisObj, jstring inJNIStr, jstring inStr, jchar chr, jint num)
{
cout<<"Inside modifyACL()\n";
PSID pSidOwner = NULL;
DWORD dwRtnCode = 0, dwAcctName = 1, dwDomainName = 1;
HANDLE hFile;
PSECURITY_DESCRIPTOR pSD = NULL;
PACL pOldDACL = NULL;
char ch;
char tp=(char)chr;
int val,i,aceNum;
int perm=(int)num;

const char *input = env->GetStringUTFChars(inJNIStr, NULL);
const char *user = env->GetStringUTFChars(inStr, NULL);

// Get the handle of the file object.
hFile = CreateFile(
                  input,
                  GENERIC_READ,
                  FILE_SHARE_READ,
                  NULL,
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);

// Check GetLastError for CreateFile error code.
if (hFile == INVALID_HANDLE_VALUE) {
          DWORD dwErrorCode = 0;
          dwErrorCode = GetLastError();
          cout << "CreateFile error = " << dwErrorCode<<". Possibly NO file exist in the given path or READ Access denied.";
          exit;
}

// Get the SID of the file.
dwRtnCode = GetSecurityInfo(
                  hFile,
                  SE_FILE_OBJECT,
                  DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION,
                  &pSidOwner,
                  NULL,
                  &pOldDACL,
                  NULL,
                  &pSD);

// Check GetLastError for GetSecurityInfo error condition.
if (dwRtnCode != ERROR_SUCCESS) {
          DWORD dwErrorCode = 0;
          dwErrorCode = GetLastError();
          cout << "GetSecurityInfo error = " << dwErrorCode;
}
	pOldDACL = SetPerm(input,user,tp,perm,pOldDACL);		//passing pOldDACL to add the ACE in exixting ACE.
}
