#include "tracelog.h"
// 默认日志最大100KB大小
#define MAXFILELEN 102400

static unsigned char getcode(char *path)
{
	unsigned char code=255;
	if(strcmp("ERROR",path)==0)
	{
		code=1;
	}
	else if(strcmp("INF",path)==0)
	{
		code=2;
	}
	else if(strcmp("DEBUG",path)==0)
	{
		code=4;
	}
	else if(strcmp("NONE",path)==0)
	{
		code=0;
	}
	return code;
}
 
static unsigned long get_file_size(const char *path)  
{  
	unsigned long filesize = -1;	  
	struct stat statbuff;  
	if(stat(path, &statbuff) < 0)
	{  
		return filesize;  
	}
	else
	{  
		filesize = statbuff.st_size;  
	}  
	return filesize;  
}  

/*
 *日志设置信息
 * */
static LOGSET *getlogset(char level[])
{
	char value[]="/tmp/scutclient.log";
	memcpy(logsetting.filepath,value,strlen(value));
	logsetting.loglevel=getcode(level);
	return &logsetting;
}

/*
 *获取时间
 * */
static void settime()
{
	time_t timer=time(NULL);
	strftime(loging.logtime,20,"%Y-%m-%d %H:%M:%S",localtime(&timer));
}
 
/*
 *不定参打印
 * */
static void PrintfLog(char * fromat,va_list args)
{
	int d,x;
	char c,*s;
	while(*fromat)
	{
		switch(*fromat)
		{
			case 's':
				s = va_arg(args, char *);
				fprintf(loging.logfile,"%s",s);
				printf("%s",s);
			break;
			case 'd':
				d = va_arg(args, int);
				fprintf(loging.logfile,"%d",d);
				printf("%d",d);
			break;
			case 'c':
				c = (char)va_arg(args, int);
				fprintf(loging.logfile,"%c",c);
				printf("%c",c);
			break;
			case 'x':
				x = (unsigned char)va_arg(args,int);
				fprintf(loging.logfile,"%02x",x);
				printf("%02x",x);
			break;
			default:
				if(*fromat!='%'&&*fromat!='\n')
				{
					fprintf(loging.logfile,"%c",*fromat);
					printf("%c",*fromat);
				}
			break;
		}
		fromat++;
	}
	fprintf(loging.logfile,"%s","]\n");
	printf("%s","]\n");
}
 
static int initlog(unsigned char loglevel)
{
	LOGSET *logsetting;
	//获取日志配置信息
	if((logsetting=getlogset(&loglevel))==NULL)
	{
		perror("Get Log Set Fail!");
		return -1;
	}
	if((loglevel&(logsetting->loglevel))!=loglevel)
	{
		perror("Get Log Level Set Fail!");
		return -1;
	}
	memset(&loging,0,sizeof(LOG));
	//获取日志时间
	settime();
	memcpy(loging.filepath,logsetting->filepath,MAXFILEPATH);
	
	// 判定是否大于指定的大小，进行重命名为备份文件
	if (get_file_size(loging.filepath)  > MAXFILELEN)  
	{
		char cmdbuf[256] ={0};  
		strcat(cmdbuf,"mv ");
		strcat(cmdbuf,loging.filepath);
		strcat(cmdbuf," ");
		strcat(cmdbuf,loging.filepath);
		strcat(cmdbuf,".backup.log");
		if ((access(loging.filepath,F_OK)) != -1)  
		{
			system(cmdbuf);	  
		}
	}
	
	//打开日志文件
	if(loging.logfile==NULL)
	{
		loging.logfile=fopen(loging.filepath,"a+");
	}
	if(loging.logfile==NULL)
	{
		perror("Open Log File Fail!");
		return -1;
	}
	//写入日志级别，日志时间
	fprintf(loging.logfile,"[%s] [%s]:[",LogLevelText[loglevel-1],loging.logtime);
	printf("[%s] [%s]:[",LogLevelText[loglevel-1],loging.logtime);
	return 0;
}
 
/*
 *日志写入
 * */
int LogWrite(unsigned char loglevel,char *fromat,...)
{
	va_list args;
	//初始化日志
	if(initlog(loglevel)!=0)
		return -1;
	//打印日志信息
	va_start(args,fromat);
	PrintfLog(fromat,args);
	va_end(args);
	//文件刷出
	fflush(loging.logfile);
	//日志关闭
	if(loging.logfile!=NULL)
		fclose(loging.logfile);
	loging.logfile=NULL;
	return 0;
}