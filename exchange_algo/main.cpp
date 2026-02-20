#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

FILE *data,*data2,*IP,*OP,*RS;
void P_want();
void P_want_tmp(),FP_want_tmp();
void TRY();
void log(int ,int ,int , int );
int find_conf_idx(int , int );

char F_DATA2[30]="./_comb_data.txt";
char F_DATA[30]="./_user_data.txt";
char F_IP[30]="./input.txt";
char F_OP[30]="./_error_log.txt";
char F_RS[30]="./results.txt";

char name[10][30];
char goaway[100];

int num_player;
int i,j,k,l,n;
int Alice,Sig,MAX;

int Comb3[3][4];
int Comb4[10][5];
int Comb5[45][6];
int Comb6[266][7];
int Comb7[1855][8];

int store[10][8],want[10][8],tmp_want[10][8],limit[10],signal;
int valid_spot[100][2],num_valid; 	// 	 [n][0] = 玩家編號	[n][[1] = 線索編號  
int record[100]; 	// = 0 第一次來 ; = 1 退回來的 ; = 2 第二次退回來 
int tmp_solu[100]; 	// = 0 那一格不選 ; = 1 那一格選want 
int num_solu;
int pct[11];

int conf[100][3],num_conf; 	// 	[0] = 投票  [1][2] 是 玩家1給玩家2  
int tmp_how[8][10]; 	// 	7個數字 最多9個玩家  tmp_how[][0] 是存他是幾方 
int tmp_vote[100],tmp_chg[1000][3],tmp_num_chg; 	
int tmp_rank[6]; 
int tmp_3[3],tmp_4[10],tmp_5[45],tmp_6[266],tmp_7[1855];
int B_rank[6],Best_want[10][8],Best_chg[100][8],B_num_chg; 	// 	conf:紀錄誰給誰 

int main()
{

	printf("【方舟線索交換器 ver 1.2】\n\n");
    
	printf("讀取基本資料資料...\n\n");
    
	data2 = fopen(F_DATA2,"r");
	for(i=1;i<=2;i++)for(j=1;j<=3;j++)fscanf(data2,"%d",&Comb3[i][j]);
	for(i=1;i<=9;i++)for(j=1;j<=4;j++)fscanf(data2,"%d",&Comb4[i][j]);
	for(i=1;i<=44;i++)for(j=1;j<=5;j++)fscanf(data2,"%d",&Comb5[i][j]);
	for(i=1;i<=265;i++)for(j=1;j<=6;j++)fscanf(data2,"%d",&Comb6[i][j]);
	for(i=1;i<=1854;i++)for(j=1;j<=7;j++)fscanf(data2,"%d",&Comb7[i][j]);

	printf("讀取玩家資料...\n\n");
    
	data = fopen(F_DATA,"r");
	IP = fopen(F_IP,"r");
	OP = fopen(F_OP,"w");
	RS = fopen(F_RS,"w");
	fscanf(data,"%s",&goaway);
	fscanf(data,"%d",&num_player);
	fscanf(data,"%s",&goaway);
	for(i=1;i<=num_player;i++)fscanf(data,"%s",&name[i]);
	printf("線索交換人數：%d人\n",num_player);
	for(i=1;i<=num_player;i++)printf("%d號 %s\n",i,name[i]);
	printf("\n\n");
    
	// printf("請決定要手動輸入數據 或 讀取資料 (1 = 手動; 1以外的數字 = 讀取)：");
	// scanf("%d",&Alice);
	Alice = 2;
    
	for(i=1;i<=num_player;i++)for(j=1;j<=num_player;j++)if(i!=j)
	{
		num_conf++;
		conf[num_conf][1]=i;
		conf[num_conf][2]=j;
	}
	printf("組合數 = %d\n",num_conf);
	B_rank[1]=100; 	// 	先設最大 
    
	if(Alice==1)    
	{
		for(i=1;i<=num_player;i++)
		{
			printf("請輸入 %d 號玩家 %s 擁有的線索：",i,name[i]);
			scanf("%s",&goaway);
			j=0;
            
			for(j=0;goaway[j]!='\0';j++)store[i][ goaway[j]-48 ] ++ ;
            
			printf("請輸入 %d 號玩家 %s 想換的線索：\n(沒指定打0，有指定交換數的話在前面加上對應數目的$字號)",i,name[i]);
			scanf("%s",&goaway);
			for(j=0;goaway[j]!='\0';j++)
			{
				if(goaway[j]=='$')limit[i]++;
				else want[i][ goaway[j]-48 ] = 1;            
			}
		}
	}
        
	else	// 讀取    
	{
		for(i=1;i<=num_player;i++)
		{
			fscanf(IP,"%s",&goaway);
			j=0;            
			for(j=0;goaway[j]!='\0';j++)store[i][ goaway[j]-48 ] ++ ;
			printf("%d 號玩家 %s 擁有的線索：%s\n",i,name[i],goaway);
            
			fscanf(IP,"%s",&goaway);
			for(j=0;goaway[j]!='\0';j++)
			{
				if(goaway[j]=='$')limit[i]++;
				else want[i][ goaway[j]-48 ] = 1;            
			}
			printf("%d 號玩家 %s 想換的線索：%s\n",i,name[i],goaway);
			printf("\n");
		}        
	}
    
	// 標記不可能換的成 -1 
	for(j=1;j<=num_player;j++)for(i=1;i<=7;i++)if(want[j][i]!=1)if(store[j][i]==0)want[j][i]=-1;    
    
    
	printf("\n*********特殊要求*********\n\n");
	for(i=1;i<=num_player;i++)
	{
		if(limit[i]!=0)printf("%d 號玩家只想換 %d張 線索\n",i,limit[i]);
		else limit[i]=10; // 沒有限制的人設為最大值 
	}
        
	printf("\n**************************\n若以上資訊無誤");
	// system("PAUSE"); 
    
	///    檢查有沒有人自己沒有線索硬要換 
    
	for(j=1;j<=num_player;j++)for(i=1;i<=7;i++)if(want[j][i]==1)if(store[j][i]==0)
	{
		printf("\n%s 的 %d 號線索自己沒有，不能跟別人換QAQ!!! \n\n",name[j],i);
		want[j][i]=-1;
		// system("PAUSE"); 
	}
     
    
	///    檢查有沒有不可以換的 有的話貼出錯誤訊息 ///
    
	printf("\n>> (1) 檢查有沒有人線索換不掉、有沒有線索只有一人有\n");    
	for(i=1;i<=7;i++)
	{
		Alice = 0;
		for(j=1;j<=num_player;j++)if(want[j][i]==1)Alice++;
		if(Alice==0)continue;   // 這個號碼沒人要換  
		else                    // 有人要換 
		{
			Alice = 0;
			for(j=1;j<=num_player;j++)if(want[j][i]!=-1)
			{
				Alice++;
				Sig = j;
			}
			if(Alice<=1)
			{
				printf("\n%s 的 %d 號線索 沒人跟他換QAQ!!! \n\n",name[Sig],i);
				for(j=1;j<=num_player;j++)want[j][i]=-1;    //    這個編號的線索都可以休息了 
				// system("PAUSE"); 
			}
			else;    // 確定這個號碼超過一個人以上擁有 
		}
	}
    
	for(i=1;i<=7;i++)
	{
		Alice = 0;
		for(j=1;j<=num_player;j++)if(store[j][i]!=0)Alice++;
		if(Alice<=1)for(j=1;j<=num_player;j++)want[j][i]=-1; //    如果只有一個人有 這個編號的線索可以休息了 
	}
     
    
	printf("\n>> (1) 完成\n");
        
	printf("\n>> (2) 某人的某號線索大於等於3個，讓他也換那個號碼\n");
    
	for(i=1;i<=7;i++)for(j=1;j<=num_player;j++)if(store[j][i]>=3)if(want[j][i]!=-1)want[j][i]=1;

	printf("\n>> (2) 完成\n");
    
	printf("\n>> (3) 檢查計算數量是否過於龐大\n");
    
	//if(0)for(j=1;j<=num_player;j++)printf("玩家%d 預計要換%d個\n",j,player_state[j]);

	//    先數好有幾個可動點 
	for(j=1;j<=num_player;j++)for(i=1;i<=7;i++)if(want[j][i]==0)
	{
		num_valid++;
		valid_spot[num_valid][0]=j;
		valid_spot[num_valid][1]=i;        
	}
    
	printf("\nnum_valid = %d\n",num_valid);
    
	MAX = pow(2,num_valid);
	printf("\n嘗試組合總數 = %d\n",MAX);
	for(i=1;i<=9;i++)pct[i] = MAX/10*i;
    
	//for(i=1;i<=num_valid;i++)printf("(%d , %d)\n",valid_spot[i][0],valid_spot[i][1]);
 
	if(num_valid>=23)
	{
		printf("\n【組合數過多，應嘗試優化】\n");
        
		printf("\n>> (3-1) 某人的某號線索大於等於2個，讓他也換那個號碼\n");
    
		for(i=1;i<=7;i++)for(j=1;j<=num_player;j++)if(store[j][i]>=2)if(want[j][i]!=-1)want[j][i]=1;
        
		printf("\n>> (3-1) 完成\n");
            
		num_valid = 0;
        
		for(j=1;j<=num_player;j++)for(i=1;i<=7;i++)if(want[j][i]==0)
		{
			num_valid++;
			valid_spot[num_valid][0]=j;
			valid_spot[num_valid][1]=i;        
		}    
        
		printf("\nnum_valid = %d\n",num_valid);
        
		MAX = pow(2,num_valid);
		printf("\n嘗試組合總數 = %d\n",MAX);
		for(i=1;i<=9;i++)pct[i] = MAX/10*i;        
	}
            
		if(num_valid>=31)
	{
		printf("\n\n警告：溢位爆炸了!!!\n\n強制結束程式運算!!!\n\n");
		goto END;
	}
    	
		printf("\n>> (4) 開始計算所有可能性\n");
 
	if(num_valid>=20)if(num_valid<=22)printf("\n請稍等一下 (約1分鐘內完成)......\n\n");
	if(num_valid>=23)if(num_valid<=25)printf("\n可能會跑有點久 (約1分鐘以上、10分鐘以下)......\n\n");
	if(num_valid>=26)if(num_valid<=30)printf("\n可能會跑超級久 (遠超過10分鐘)......\n\n");
 
	//    先把 tmp_want 確定
	for(j=1;j<=num_player;j++)for(i=1;i<=7;i++)tmp_want[j][i]=want[j][i];
    
	//printf("走迷宮測試開始!!!!!\n");

	printf("已計算 0%%......\n");    
	Alice = 1;    
	while(Alice!=0)
	{
		if(Alice <= num_valid)
		{
			if(record[ Alice ]==0)    //    (第一次來) 
			{
				record[Alice]++;    //	紀錄試過了 = 1
				tmp_solu[Alice] = -1;
				Alice++;    
                
			}
			else if(record[ Alice ]==1)    // (後面退回來的) 
			{
				record[Alice]++;    //	紀錄試過第二次了  = 2
				tmp_solu[Alice] = 1;
				Alice++;            
			}
			else    // (第二次退回來) 
			{
				record[Alice]=0;    //	都試完了 = 0
				Alice--;
			}

		}
        
		else    //    到底層 
		{
			num_solu++;
			for(i=1;i<=num_valid;i++)tmp_want[ valid_spot[i][0] ][ valid_spot[i][1]] = tmp_solu[i];
			TRY();
			Alice --; 
		}
        
		for(i=1;i<=9;i++)if(num_solu==pct[i])
		{
			printf("已計算 %d0%%......\n",i);
			pct[i]--;
		}
	}
	printf("已計算 100%%......\n");


	printf("\n>> (4) 完成\n");
        
	//    輸出最佳結果
    
	//    紀錄 B_rank[6],Best_want[10][8],Best_chg[100][8],B_num_chg 
    
	printf("最佳結果參數：(%d,%d,%d,%d,%d)\n",B_rank[1],B_rank[2],B_rank[3],B_rank[4],B_rank[5]);
	printf("\n號碼\t");
	for(j=1;j<=7;j++)printf("%d\t",j);    
	printf("\n");
	printf("--------------------------------------------------------------\n");
	for(i=1;i<=num_player;i++)
	{
		printf("%s\t",name[i]);
		for(j=1;j<=7;j++)
		{
			if(Best_want[i][j]==1)printf("O\t");
			else if(Best_want[i][j]==0)printf(".\t");
			else printf(" \t");                
		}
		printf("\n");
	}
	printf("\n");
    
    
	printf("兌換方法：\n");
	for(j=1;j<=num_player;j++)
	{
		for(i=1;i<=num_player;i++)
		{
			Alice = 0;
			for(l=1;l<=7;l++)
			{
				for(k=1;k<=B_num_chg;k++)if(Best_chg[k][1]==j)if(Best_chg[k][2]==i)if(Best_chg[k][0]==l)
				{
					if(Alice==0)
					{
						printf("%s\t -->\t %s\t",name[j],name[i]);
						fprintf(RS,"%s\t -->\t %s\t",name[j],name[i]);    
					}
					Alice++;
					printf("%d",Best_chg[k][0]);
					fprintf(RS,"%d",Best_chg[k][0]);
				}
			}
			if(Alice!=0)
			{
				printf("\n");
				fprintf(RS,"\n");
			}
		}        
		printf("--------------------------\n");
		fprintf(RS,"--------------------------\n");
	}    
    
	if(B_rank[1]>=num_player)
	{
		END:
		printf("\n\n\n交換失敗......請洽技術猿處理！！！\n\n\n");
	}
        
    
	fclose(data);
	fclose(IP);    
	fclose(OP);    
	fclose(RS);
    
    
	// system("PAUSE");
	return 0;
}

void P_want_tmp()
{
	int i,j;
	printf("\n號碼\t");
	for(j=1;j<=7;j++)printf("%d\t",j);    
	printf("\n【需求狀況】\n");
	printf("--------------------------------------------------------------\n");
	for(i=1;i<=num_player;i++)
	{
		printf("%s\t",name[i]);
		for(j=1;j<=7;j++)
		{
			if(tmp_want[i][j]==1)printf("O\t");
			else if(tmp_want[i][j]==0)printf(".\t");
			else printf(" \t");                
		}
		printf("\n");
	}
	printf("\n");        
}

void FP_want_tmp()
{
	int i,j;
	fprintf(OP,"\n號碼\t");
	for(j=1;j<=7;j++)fprintf(OP,"%d\t",j);    
	fprintf(OP,"\n【需求狀況】\n");
	fprintf(OP,"--------------------------------------------------------------\n");
	for(i=1;i<=num_player;i++)
	{
		fprintf(OP,"%s\t",name[i]);
		for(j=1;j<=7;j++)
		{
			if(tmp_want[i][j]==1)fprintf(OP,"O\t");
			else if(tmp_want[i][j]==0)fprintf(OP,".\t");
			else fprintf(OP," \t");                
		}
		fprintf(OP,"\n");
	}
	fprintf(OP,"\n");        
}

void TRY()
{
	int i,j,k,a,b,c,Alice,BEST;
	int cnt,shift,pos,score,best_shift,best_score,from,to;
    
	//RANK:    1:P0少  2:P1少  3:P6少  4:箭頭少  5:數字用少 
    
	for(i=1;i<=7;i++)for(j=0;j<=num_player;j++)tmp_how[i][j]=0;
	for(k=1;k<=num_conf;k++)tmp_vote[k]=0;
	for(i=1;i<=5;i++)tmp_rank[i]=0;


	fprintf(OP,"NO. %d\t",num_solu);
    
	//    檢查是否能換 
	for(i=1;i<=7;i++)    
	{
		for(j=1;j<=num_player;j++)if(tmp_want[j][i]==1)
		{
			tmp_how[i][0]++;    
			tmp_how[i][ tmp_how[i][0] ] = j;
		}
        
		if(tmp_how[i][0] ==1)    //    不能有號碼只有一人有 
		{
			fprintf(OP,">>ERROR ANS!!!\n");
			return;
		}

		if (tmp_how[i][0] > 0)tmp_rank[5]+=tmp_how[i][0];    //    計算用了幾個數字 
	}
	if(0)FP_want_tmp();
    
	//    計算每個人花的線索 
    
	for(j=1;j<=num_player;j++)
	{
		Alice = 0;
		for(i=1;i<=7;i++)if(tmp_want[j][i]==1)Alice++;
		if(Alice==0)tmp_rank[1]++;
		else if(Alice==1)tmp_rank[2]++;
		else if(Alice>=6)tmp_rank[3]++;
		if(Alice>limit[j])
		{
			fprintf(OP,">>LIMITED ANS!!!\n");
			return;
		}
	}
    
	tmp_num_chg=0;
    
	//    計算用了幾個箭頭  num_conf, conf[i][1], conf[i][2]    
    
	for(i=1;i<=7;i++)if(tmp_how[i][0]==2) // 先算2  
	{
		for(k=1;k<=num_conf;k++)
		{
			if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][2])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][1])log(k,i,conf[k][1],conf[k][2]);
		}
	}

	for(i=1;i<=7;i++)if(tmp_how[i][0]==3) // 算3  
	{
		for(n=1;n<=2;n++)tmp_3[n]=0;    
		for(n=1;n<=2;n++)
		{
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb3[n][1] ])tmp_3[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb3[n][2] ])tmp_3[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb3[n][3] ])tmp_3[n]+=tmp_vote[k];
		}

		Alice = -1;    //    試試看最佳投票是誰  
		for(n=1;n<=2;n++)if(tmp_3[n]>Alice)
		{
			BEST = n;
			Alice = tmp_3[n];
		} 

		for(k=1;k<=num_conf;k++)
		{
			if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb3[BEST][1] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb3[BEST][2] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb3[BEST][3] ])log(k,i,conf[k][1],conf[k][2]);
		}
    
	}
    
	for(i=1;i<=7;i++)if(tmp_how[i][0]==4) // 算4  
	{
		for(n=1;n<=9;n++)tmp_4[n]=0;
		for(n=1;n<=9;n++)
		{
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb4[n][1] ])tmp_4[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb4[n][2] ])tmp_4[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb4[n][3] ])tmp_4[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb4[n][4] ])tmp_4[n]+=tmp_vote[k];
		}    

		Alice = -1;
		for(n=1;n<=9;n++)if(tmp_4[n]>Alice)
		{
			BEST = n;
			Alice = tmp_4[n];
		} 
        
		for(k=1;k<=num_conf;k++)
		{
			if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb4[BEST][1] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb4[BEST][2] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb4[BEST][3] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb4[BEST][4] ])log(k,i,conf[k][1],conf[k][2]);
		}    
	}    

	for(i=1;i<=7;i++)if(tmp_how[i][0]==5) // 算5  
	{
		for(n=1;n<=44;n++)tmp_5[n]=0;
		for(n=1;n<=44;n++)
		{
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb5[n][1] ])tmp_5[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb5[n][2] ])tmp_5[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb5[n][3] ])tmp_5[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb5[n][4] ])tmp_5[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][5])if(conf[k][2]==tmp_how[i][ Comb5[n][5] ])tmp_5[n]+=tmp_vote[k];
		}    

		Alice = -1;
		for(n=1;n<=44;n++)if(tmp_5[n]>Alice)
		{
			BEST = n;
			Alice = tmp_5[n];
		} 
        
		for(k=1;k<=num_conf;k++)
		{
			if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb5[BEST][1] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb5[BEST][2] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb5[BEST][3] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb5[BEST][4] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][5])if(conf[k][2]==tmp_how[i][ Comb5[BEST][5] ])log(k,i,conf[k][1],conf[k][2]);
		}    
	}

	for(i=1;i<=7;i++)if(tmp_how[i][0]==6) // 算6  
	{
		for(n=1;n<=265;n++)tmp_6[n]=0;
		for(n=1;n<=265;n++)
		{
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb6[n][1] ])tmp_6[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb6[n][2] ])tmp_6[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb6[n][3] ])tmp_6[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb6[n][4] ])tmp_6[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][5])if(conf[k][2]==tmp_how[i][ Comb6[n][5] ])tmp_6[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][6])if(conf[k][2]==tmp_how[i][ Comb6[n][6] ])tmp_6[n]+=tmp_vote[k];
		}    

		Alice = -1;
		for(n=1;n<=265;n++)if(tmp_6[n]>Alice)
		{
			BEST = n;
			Alice = tmp_6[n];
		} 
        
		for(k=1;k<=num_conf;k++)
		{
			if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb6[BEST][1] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb6[BEST][2] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb6[BEST][3] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb6[BEST][4] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][5])if(conf[k][2]==tmp_how[i][ Comb6[BEST][5] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][6])if(conf[k][2]==tmp_how[i][ Comb6[BEST][6] ])log(k,i,conf[k][1],conf[k][2]);
		}    
	}    

	for(i=1;i<=7;i++)if(tmp_how[i][0]==7) // 算7  
	{
		for(n=1;n<=1854;n++)tmp_7[n]=0;
		for(n=1;n<=1854;n++)
		{
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb7[n][1] ])tmp_7[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb7[n][2] ])tmp_7[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb7[n][3] ])tmp_7[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb7[n][4] ])tmp_7[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][5])if(conf[k][2]==tmp_how[i][ Comb7[n][5] ])tmp_7[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][6])if(conf[k][2]==tmp_how[i][ Comb7[n][6] ])tmp_7[n]+=tmp_vote[k];
			for(k=1;k<=num_conf;k++)if(conf[k][1]==tmp_how[i][7])if(conf[k][2]==tmp_how[i][ Comb7[n][7] ])tmp_7[n]+=tmp_vote[k];
		}    

		Alice = -1;
		for(n=1;n<=1854;n++)if(tmp_7[n]>Alice)
		{
			BEST = n;
			Alice = tmp_7[n];
		} 
        
		for(k=1;k<=num_conf;k++)
		{
			if(conf[k][1]==tmp_how[i][1])if(conf[k][2]==tmp_how[i][ Comb7[BEST][1] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][2])if(conf[k][2]==tmp_how[i][ Comb7[BEST][2] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][3])if(conf[k][2]==tmp_how[i][ Comb7[BEST][3] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][4])if(conf[k][2]==tmp_how[i][ Comb7[BEST][4] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][5])if(conf[k][2]==tmp_how[i][ Comb7[BEST][5] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][6])if(conf[k][2]==tmp_how[i][ Comb7[BEST][6] ])log(k,i,conf[k][1],conf[k][2]);
			if(conf[k][1]==tmp_how[i][7])if(conf[k][2]==tmp_how[i][ Comb7[BEST][7] ])log(k,i,conf[k][1],conf[k][2]);
		}    
	}
    
	for(i=1;i<=7;i++)if(tmp_how[i][0]>=8) // 算8以上 (fallback)
	{
		cnt = tmp_how[i][0];
		best_shift = 1;
		best_score = -1;

		for(shift=1;shift<=cnt-1;shift++)
		{
			score = 0;
			for(pos=1;pos<=cnt;pos++)
			{
				from = tmp_how[i][pos];
				to = tmp_how[i][ ((pos+shift-1)%cnt)+1 ];
				k = find_conf_idx(from,to);
				if(k>0)score += tmp_vote[k];
			}
			if(score>best_score)
			{
				best_score = score;
				best_shift = shift;
			}
		}

		for(pos=1;pos<=cnt;pos++)
		{
			from = tmp_how[i][pos];
			to = tmp_how[i][ ((pos+best_shift-1)%cnt)+1 ];
			k = find_conf_idx(from,to);
			if(k>0)log(k,i,from,to);
		}
	}


	
	for(k=1;k<=num_conf;k++)if(tmp_vote[k]>0)tmp_rank[4]++;		//	箭頭使用數量       
    
	fprintf(OP,"(%d,%d,%d,%d,%d)\n",tmp_rank[1],tmp_rank[2],tmp_rank[3],tmp_rank[4],tmp_rank[5]);
    
	//    紀錄 B_rank[6],Best_want[10][8],Best_chg[100][8],B_num_chg 
    
	for(i=1;i<=5;i++)
	{
		if(B_rank[i]>tmp_rank[i])
		{
			B_rank[i] = tmp_rank[i];
			for(j=i;j<=5;j++)B_rank[j] = tmp_rank[j];
			for(a=1;a<=num_player;a++)for(b=1;b<=7;b++)Best_want[a][b]=tmp_want[a][b];
			B_num_chg = tmp_num_chg;
			for(j=1;j<=B_num_chg;j++)
			{
				Best_chg[j][0]=tmp_chg[j][0];
				Best_chg[j][1]=tmp_chg[j][1];
				Best_chg[j][2]=tmp_chg[j][2];
			}			
		}
		else if (B_rank[i] == tmp_rank[i]);
		else break;
	}
	
	if(0)fprintf(OP,"BEST:(%d,%d,%d,%d,%d)\n",B_rank[1],B_rank[2],B_rank[3],B_rank[4],B_rank[5]);
}

void log(int k,int a,int b, int c)
{
	tmp_vote[k]++;
	tmp_num_chg++;
	tmp_chg[tmp_num_chg][0] = a;
	tmp_chg[tmp_num_chg][1] = b;
	tmp_chg[tmp_num_chg][2] = c;
}

int find_conf_idx(int from, int to)
{
	int idx;
	for(idx=1;idx<=num_conf;idx++)if(conf[idx][1]==from)if(conf[idx][2]==to)return idx;
	return 0;
}


