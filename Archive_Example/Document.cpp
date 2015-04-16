#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<iostream>



using namespace std;

#define DESTDIRECTORY "C:\\myfolder\\ "
long getDiffBetween(SYSTEMTIME start,SYSTEMTIME end)
{
	FILETIME ftStart;
	FILETIME ftEnd;

	SystemTimeToFileTime(&start,&ftStart);
	SystemTimeToFileTime(&end,&ftEnd);

	
    union timeunion {
        FILETIME fileTime;
        ULARGE_INTEGER ul;
    } ;
    
    timeunion ft1;
    timeunion ft2;

    SystemTimeToFileTime(&start, &ft1.fileTime);
    SystemTimeToFileTime(&end, &ft2.fileTime);

    _int64 Delta =  ft2.ul.QuadPart - ft1.ul.QuadPart;
	
	
	unsigned long long seconds = Delta / 10000000;  // to get minutes
	//int minutes = seconds/60;
	if(seconds)
		return seconds;
	else
		return 0;
	
}

int main(int argc, CHAR* argv[])
{

   //Set the current directory to your required folder 
  ::SetCurrentDirectory ("C:\\NEWBASE\\tools\\SQL-Scripts\\");
   
  WIN32_FIND_DATA fd;
  FILETIME ftCreate;
  SYSTEMTIME stUTC, stLocal,stCurrent;
	int noofDays;
	cout<<"Enter the number of days:";
	cin>>noofDays;
  //Here you can filter the extension 
	HANDLE hFind = ::FindFirstFile (("*.txt*"), &fd); 
   

   if (hFind != INVALID_HANDLE_VALUE) 
      {
        do  {
                  //Only print files present in the directory, ignore if it is directory, the flag
                //FILE_ATTRIBUTE_DIRECTORY will tell you if the item found is a file or directory

                 if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				 {
						printf(("%s\n"), fd.cFileName);
						ftCreate=fd.ftCreationTime;

						 FileTimeToSystemTime(&ftCreate, &stUTC);
						 SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
						char buffer[ 256 ];
						 sprintf( buffer, "%d-%02d-%02d %02d:%02d:%02d", 
									stLocal.wYear,
									stLocal.wMonth, 
									stLocal.wDay,                      
									stLocal.wHour, 
									stLocal.wMinute, 
									stLocal.wSecond );
						

						GetLocalTime(&stCurrent);
						long diffTime=getDiffBetween(stLocal,stCurrent);
						cout<<"Difference In times:"<<diffTime<<endl;

						long noOfSeconds=noofDays*24*60*60;

						if(diffTime>=noOfSeconds)
						{
							CreateDirectory (DESTDIRECTORY, NULL);
							char destPath[MAX_PATH];
							strcpy(destPath,DESTDIRECTORY);
							strcat(destPath,fd.cFileName);
							cout<<"Path is:"<<destPath<<endl;
							MoveFile(fd.cFileName,destPath);
						}
				 }


} while (::FindNextFile (hFind, &fd));
::FindClose (hFind);
      }
return 0;
}