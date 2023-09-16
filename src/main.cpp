#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
void gotoxy(int row,int col)
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position={row,col};
	SetConsoleCursorPosition(h,position);
}
void hidecursor()
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	_CONSOLE_CURSOR_INFO info;
	info.dwSize=100;
	info.bVisible=FALSE;
	SetConsoleCursorInfo(h,&info);
}
void showcursor()
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	_CONSOLE_CURSOR_INFO info;
	info.dwSize=25;
	info.bVisible=TRUE;
	SetConsoleCursorInfo(h,&info);
}
void box(char *name)
{
	system("cls");
	gotoxy(1,2);
	for(int count=0;count<118;count++)
		printf("%c",205);
	for(int count=0;count<24;count++)
		{
			gotoxy(1,3+count);
			printf("%c",186);	
		}
	gotoxy(1,27);
	for(int count=0;count<118;count++)
		printf("%c",205);
	for(int count=0;count<24;count++)
		{
			gotoxy(118,3+count);
			printf("%c",186);	
		}
	int len=strlen(name);
	gotoxy((120-len)/2,0);
	printf("%s",name);
	gotoxy(2,28);
	printf("Press 8 and 2 to move up and down, 5 for open new file, Esc for exit");
}
void print_pageno(int line)
{
	int pn;float p_n;
	pn=(line+23)/24;
	p_n=(line+23)/24.0;
	if(p_n*10>pn*10) pn=pn+1;
	gotoxy(105,28);
	printf("Page No. : %d",pn);
}
int total_line(FILE *fp)
{
	char buff[115];
	int c_p=ftell(fp),line=0;
	fseek(fp,0,SEEK_SET);
	while(fgets(buff,115,fp)!=NULL) line++;
	return line;
}
int myfgets(char *buff,int max_ele,FILE *fp)
{
	int count;
	char ch=fgetc(fp);
	if(ch=='\n') *buff='\n';
	for(count=0;ch!='\n';count++)
		{
			if(count>max_ele-2)
				{
					fseek(fp,-1,SEEK_CUR);
					break;
				}
			if(ch==EOF)
				{
					for(int count1=count;count1<max_ele-1;count1++)
						*(buff+count1)=' ';
					*(buff+max_ele-1)='\0';
					return 0;
				}
			else if(ch=='\r')
				{
					count--;
					continue;
				}
			else if(ch=='\t')
				{
					count+=3;
					if(count>max_ele-2) break;
					strcpy(buff+count-3,"    ");
				}
			else *(buff+count)=ch;
			ch=fgetc(fp);
		}
	for(int count1=count;count1<max_ele-1;count1++)
		*(buff+count1)=' ';
	*(buff+max_ele-1)='\0';
	return 1;
}
void print_text(FILE *fp,int s_line)
{
	int line=1;
	char arr[115];
	fseek(fp,0,SEEK_SET);
	for(int count=1;count<s_line;count++)
		myfgets(arr,115,fp);
	while(line<=24)
		{
			gotoxy(3,3+line-1);
			if(myfgets(arr,115,fp)==0)
				{
					printf("%s",arr);
					break;
				}
			else printf("%s",arr);
			line++;
		}
}
main()
{
	again :
	char name[100];
	printf("Enter a file name or file path : ");
	gets(name);
	hidecursor();
	FILE *fp;
	fp=fopen(name,"rt");
	if(fp==NULL)
		{
			printf("\nCan not open file! Try again!\n");
			printf("Press any key .... ");
			getch();
			system("cls");
			showcursor();
			goto again;
			exit(0);
		}
	int t_line=total_line(fp),line=1,choice;
	box(name);
	print_text(fp,1);
	print_pageno(line);
	while(1)
	{
		choice=getch();
		switch(choice)
		{
			case '8':
					if(t_line<=24) continue;
					if(line==(t_line-24+1)) continue;
					line++;
					print_pageno(line);
					print_text(fp,line);
					break;
			case '2':
					if(t_line<=24) continue;
					if(line==1) continue;
					line--;
					print_pageno(line);
					print_text(fp,line);
					break;
			case '5':
					system("cls");
					fclose(fp);
					showcursor();
					goto again;
					break;
			case 27:
					fclose(fp);
					exit(1);
		}
	}
}
