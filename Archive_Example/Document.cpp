#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<iostream>

using namespace std;

#define DESTDIRECTORY "D:\\ "
#define SAMPLEFOLDER "myfolder"
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

string SystimeInString(SYSTEMTIME stime)
{
	char buf[40] = {0};
	sprintf(buf,"%02d%02d%04d",stime.wDay, stime.wMonth, stime.wYear);
	string dt;
	dt.assign(buf);
		return dt;
}

int main(int argc, CHAR* argv[])
{

   //Set the current directory to your required folder 
  ::SetCurrentDirectory ("C:\\NEWBASE\\tools\\");
   
  WIN32_FIND_DATA fd;
  FILETIME ftCreate;
  SYSTEMTIME stUTC, stLocal,stCurrent;
	int noofDays;
	cout<<"Enter the number of days:";
	cin>>noofDays;
  
	HANDLE hFind = ::FindFirstFile (("*.*"), &fd); 
   

   if (hFind != INVALID_HANDLE_VALUE) 
      {

						char sampleFolder[MAX_PATH];
						strcpy(sampleFolder,DESTDIRECTORY);
						strcat(sampleFolder,SAMPLEFOLDER);
						CreateDirectory(sampleFolder, NULL);
        do  {
                  //Only print files present in the directory, ignore if it is directory, the flag
                //FILE_ATTRIBUTE_DIRECTORY will tell you if the item found is a file or directory

                 if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				 {
						printf(("%s\n"), fd.cFileName);
						ftCreate=fd.ftLastWriteTime;

						FileTimeToSystemTime(&ftCreate, &stUTC);
						SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
						string date=SystimeInString(stLocal);
						
						GetLocalTime(&stCurrent);
						long diffTime=getDiffBetween(stLocal,stCurrent);
						cout<<"Difference In times:"<<diffTime<<endl;

						long noOfSeconds=noofDays*24*60*60;

						if(diffTime>=noOfSeconds)
						{
							char OutputFolder[MAX_PATH];
							strcpy(OutputFolder,sampleFolder);
							strcat(OutputFolder,"\\");
							strcat(OutputFolder,date.c_str());
							CreateDirectory(OutputFolder, NULL);
							strcat(OutputFolder,"\\");
							strcat(OutputFolder,fd.cFileName);
							if (OutputFolder) 
								{
								 remove(OutputFolder);
								}
					
							MoveFile(fd.cFileName,OutputFolder);
						}
				 }


} while (::FindNextFile (hFind, &fd));
::FindClose (hFind);
      }
return 0;
}