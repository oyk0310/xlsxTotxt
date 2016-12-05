#include <unistd.h>
#include <string.h>
#include <libxml/xpath.h>

//unzip file.xlsx
int initExcel(char *path,char *name)
{
	int r;  
	system("rm -rf xlsx_txt_tmp");
	if(!(access(path,0)))
	{
		char cmd[128]="mkdir xlsx_txt_tmp";
		system(cmd);
		sprintf(cmd,"unzip %s -d xlsx_txt_tmp >/dev/null 2>&1",path);
		r=system(cmd);
		if (r == -1)  
			goto Error;  
		if (WIFEXITED(r))  
		{  
			if (WEXITSTATUS(r)==0)  	  
				return 0; 
			else  
				goto Error;
		}  
		else   
			goto Error; 
	}
	Error:
		printf("open: %s failed!\n",path);
		printf("usage: %s filename.xlsx\n",name);
		system("rm -rf xlsx_txt_tmp");
		return -1;
}

//get data from sharedStrings.xml
int getSharedStrings(char*** p,int *num)
{
	xmlDocPtr doc;           
    	xmlNodePtr curNode;               
	int size;
    	const char *szDocName="./xlsx_txt_tmp/xl/sharedStrings.xml";
	doc = xmlReadFile(szDocName,"utf8",XML_PARSE_RECOVER); 
	if (NULL == doc) 
    	{  
       		fprintf(stderr,"Document parse failed. ");     
       		return -1; 
    	} 
    	curNode = xmlDocGetRootElement(doc);
	xmlChar* szAttr = xmlGetProp(curNode,BAD_CAST "uniqueCount");
	size=atoi((char*)szAttr);
	xmlFree(szAttr);
	curNode=curNode->xmlChildrenNode;
	char **data;
	data=(char **)malloc(sizeof(char *) * size);
	int count=0;
	while(curNode)
	{		
		char * str=(char *)xmlNodeGetContent(curNode);
		data[count]=(char *)malloc(sizeof(char)*strlen(str));
		data[count]=str;
		curNode=curNode->next;
		count++;
	}
	*p=data;	
	*num=size;
	data=NULL;
	xmlFreeDoc(doc);
	xmlFree(curNode);
	return 0;
}

//free memory
void freeData(char ***p,int *size)
{
	int count=0;	
	while(count!=*size)
	{		
		free((*p)[count]);
		count++;
	}	
}

int main(int argc, char* argv[])
{
	if(initExcel(argv[1],argv[0])==-1)
		return -1;
	
	xmlDocPtr doc;           
    	xmlNodePtr curNode;               
   	const char *szDocName="./xlsx_txt_tmp/xl/worksheets/sheet1.xml";
	FILE *fp;
	int size=0;
	char **sharedStrings;
	getSharedStrings(&sharedStrings,&size);
	doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER); 
	if (NULL == doc) 
    	{  
       		fprintf(stderr,"Document parse failed. ");     
       		return -1; 
    	} 
	
    	curNode = xmlDocGetRootElement(doc); 
	curNode=curNode->xmlChildrenNode;
	while(1)
	{
		if(strcmp((char *)curNode->name,"sheetData")!=0)
			curNode=curNode->next;
		else
			break;
	}
	curNode=curNode->xmlChildrenNode;
	if(!(access("sheet1.txt",0)))
	{
		system("rm -f sheet1.txt");
	}
	while(curNode)
	{
		if((fp=fopen("sheet1.txt","aw")))
		{
			xmlChar* szAttr = xmlGetProp(curNode,BAD_CAST "r");
			char str[512];
			sprintf(str,"%s%s: ",curNode->name,szAttr);
			xmlFree(szAttr);
			fputs(str,fp);
		}else{
			printf("error: 'sheet1.txt' open failed!");
			return -1;
		}

		xmlNodePtr tmpNode =curNode->xmlChildrenNode;
		while(tmpNode)
		{
			char * str;
			if(xmlHasProp(tmpNode,BAD_CAST "t"))
			{
				str=sharedStrings[atoi((char*)xmlNodeGetContent(tmpNode))];
				fputs(str,fp);
				fputs("\t",fp);
			}else
			{
				str=(char*)xmlNodeGetContent(tmpNode);
				fputs(str,fp);
				fputs("\t",fp);
			}
			/*
			xmlChar* szAttr = xmlGetProp(tmpNode,BAD_CAST "r");
			printf("attr: %s\n",szAttr);
			xmlFree(szAttr);
			printf("name: %s\n",tmpNode->name);
			printf("content: %s\n",xmlNodeGetContent(tmpNode));
			*/
			tmpNode=tmpNode->next;
		}
		fputs("\n",fp);
		fclose(fp);
		curNode=curNode->next;
	}  
   	xmlFreeDoc(doc);
	freeData(&sharedStrings,&size);
	free(sharedStrings);
	sharedStrings=NULL;
	system("rm -rf xlsx_txt_tmp");
    return 0;
}


