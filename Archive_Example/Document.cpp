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

void MoveCurrrentFolderFiles(char* strFolder, int noofDays)
{
	::SetCurrentDirectory (strFolder);
	   
	WIN32_FIND_DATA fd;
	FILETIME ftCreate;
	SYSTEMTIME stUTC, stLocal,stCurrent;
			  
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
						//if its a text file
						if(stricmp(fd.cFileName+strlen(fd.cFileName)-4, ".txt")==0)
						{
							FileTimeToSystemTime(&ftCreate, &stUTC);
							SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
							string date=SystimeInString(stLocal);
							
							GetLocalTime(&stCurrent);
							long diffTime=getDiffBetween(stLocal,stCurrent);
							cout<<"Difference In times:"<<diffTime<<endl;

							long noOfSeconds=noofDays*24*60*60;

							if(diffTime>=noOfSeconds)
							{
								char OutputFile[MAX_PATH];
								strcpy(OutputFile,sampleFolder);
								strcat(OutputFile,"\\");
								strcat(OutputFile,date.c_str());
								CreateDirectory(OutputFile, NULL);
								strcat(OutputFile,"\\");
								strcat(OutputFile,fd.cFileName);
								if (OutputFile) 
									{
									 remove(OutputFile);
									}
								MoveFile(fd.cFileName,OutputFile);
							}
						}//endif txt file
				 }//endif file	
				 else if(strcmp(fd.cFileName, ".")==0 || strcmp(fd.cFileName, "..")==0)
				 {
					 continue;
				 }
				 else
				 {
					 char strSubFolder[1024];
					 strcpy(strSubFolder, strFolder);
					 strcat(strSubFolder, "\\");
					 strcat(strSubFolder, fd.cFileName);
					 MoveCurrrentFolderFiles(strSubFolder,noofDays);
				 }

		} while (::FindNextFile (hFind, &fd));
		::FindClose (hFind);
	}

}

int main(int argc, CHAR* argv[])
{
	int noofDays;
	cout<<"Enter the number of days:";
		cin>>noofDays;
	MoveCurrrentFolderFiles("C:\\NEWBASE", noofDays);
   //Set the current directory to your required folder 
  

	return 0;
}