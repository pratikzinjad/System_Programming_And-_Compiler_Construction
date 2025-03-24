/*Credits to:https://github.com/rajpratyush/MacroProcessor-Two-Pass/blob/main/op2.txt*/

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
void main()
{
FILE *f1,*f2,*f3,*f4,*f5;
int i,len;
char mne[20],opnd[20],la[20],name[20],mne1[20],opnd1[20],arg[20];
clrscr();
f1=fopen("minp2.txt","r");
f2=fopen("ntab2.txt","r");
f3=fopen("dtab2.txt","r");
f4=fopen("atab2.txt","w+");
f5=fopen("op2.txt","w");
fscanf(f1,"%s%s%s",la,mne,opnd);
while(strcmp(mne,"END")!=0)
{
if(strcmp(mne,"MACRO")==0)
{
fscanf(f1,"%s%s%s",la,mne,opnd);
while(strcmp(mne,"MEND")!=0)
fscanf(f1,"%s%s%s",la,mne,opnd);
}
else
{
fscanf(f2,"%s",name);
if(strcmp(mne,name)==0)
{
len=strlen(opnd);
for(i=0;i<len;i++)
{
if(opnd[i]!=',')
fprintf(f4,"%c",opnd[i]);
else
fprintf(f4,"\n");
}
fseek(f2,SEEK_SET,0);
fseek(f4,SEEK_SET,0);
fscanf(f3,"%s%s",mne1,opnd1);
fprintf(f5,".\t%s\t%s\n",mne1,opnd);
fscanf(f3,"%s%s",mne1,opnd1);
while(strcmp(mne1,"MEND")!=0)
{
if((opnd1[0]=='&'))
{
fscanf(f4,"%s",arg);
fprintf(f5,"-\t%s\t%s\n",mne1,arg);
}
else
fprintf(f5,"-\t%s\t%s\n",mne1,opnd1);
fscanf(f3,"%s%s",mne1,opnd1);
}
}
else
fprintf(f5,"%s\t%s\t%s\n",la,mne,opnd);
}
fscanf(f1,"%s%s%s",la,mne,opnd);
}
fprintf(f5,"%s\t%s\t%s\n",la,mne,opnd);
fclose(f1);
fclose(f2);
fclose(f3);
fclose(f4);
fclose(f5);
printf("pass2");
getch();
}


/*

minp2.txt

EX1	MACRO	&A,&B
-	LDA	&A
-	STA	&B
-	MEND	-
SAMPLE	START	1000
-	EX1	N1,N2
N1	RESW	1
N2	RESW	1
-	END	-


ntab2.txt

EX1



dtab2.txt

EX1	&A,&B
LDA	&A
STA	&B
MEND


atab2.txt

N1
N2


op2.txt

SAMPLE	START	1000
.	EX1	N1,N2
-	LDA	N1
-	STA	N2
N1	RESW	1
N2	RESW	1
-	END	-

*/
