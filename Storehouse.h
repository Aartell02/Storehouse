#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#define U 16
#define CN 32
typedef struct product{
    char name[CN];
    char category[CN];
    int amount;
    char unit[U];
    int year;
    int mon;
    int day;
    struct product *next;
    struct product *prev;
}product;

typedef struct filter{
	int expire;
    struct product *address;
    struct filter *next;
}filter;

void mread(product **element){
    FILE *p;
    p = fopen("Storehouse.txt","r");
    if(p==NULL){
        *element = NULL;
        return;
    }
    product *current;
    do{
        current = (product *)malloc(sizeof(product));
        memset(current->category,0,CN);
        memset(current->name,0,CN);
        memset(current->unit,0,U);
        fscanf(p,"%s",&current->name);
        fscanf(p,"%s",&current->category);
        fscanf(p,"%d",&current->amount);
        fscanf(p,"%s",&current->unit);
        fscanf(p,"%d",&current->year);
        fscanf(p,"%d",&current->mon);
        fscanf(p,"%d",&current->day);
        current->next = (*element);

        *element = current;
    }while(!feof(p));
    rewind(p);
    fclose(p);
}

void save(product *element){
    if(element==NULL) return ;
    FILE *fp;
    fp = fopen("Storehouse.txt","w");
    int i = 0;
    product *current = element;
    product *tmp;
        while(current!=NULL){
        if(i!=0)fprintf(fp,"\n");
        i=1;
        fprintf(fp,"%s %s %d %s %d %d %d",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
        tmp = current;
        current = current->next;
        free(tmp);
        }
    fclose(fp);
}

void show(product *element){
    if(element==NULL) printf("List is empty");
    else{
        product *current = element;
        while (current!=NULL){
            printf("%s %s %d %s %d-%d-%d\n",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
            current = current->next;
        }
    free(current);
    }
}

void show_selected(filter *felement){
    if(felement==NULL) return;
    filter *fcurrent = felement;
    product *current;
    filter *tmp;
    while(fcurrent!=NULL){
        current = fcurrent->address;
        printf("%s %s %d %s %d-%d-%d\n",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
        fcurrent = fcurrent->next;
    }
    fcurrent = felement;
    while(fcurrent!=NULL){
        tmp = fcurrent;
        fcurrent = fcurrent->next;
        free(tmp);
    }
    printf("Press Any Key to Continue\n");
	getch();
}

int d_in_month(int yr, int mn){
	if(mn==2){
        if(yr%4==0)return 29;
        else return 28;
	}
	else if(mn==1||mn==3||mn==5||mn==7||mn==8||mn==10||mn==12) return 31;
	else return 30;

}

int push_year(int num){
    if(num<1900){
        system("cls");
        printf("\nEnter proper year: ");
        scanf("%d",&num);
    }

    return num;
}
int push_month(int num, int yr){
	while(num<0 || num>12){
        system("cls");
        printf("%d-\nEnter proper month: ",yr);
        scanf("%d",&num);
    }
    return num;
}
int push_day(int num,int mn, int yr){
	int p;
	p = d_in_month(yr,mn);
	while(num<0 || num>p){
        system("cls");
        printf("%d-%d-\nEnter proper day: ",yr,mn);
        scanf("%d",&num);
    }
    return num;
}

int d_del(int o, product **current){
	printf("\nProduct is out of date or its expiring today\nAre you still willing to add the product? :\n1.Yes\n2.No\nChoice: ");
    scanf("%d",&o);
    if(o==2){
    	printf("\nThe product was not added\nPress Any Key to Continue\n");
		getch();
    	free(*current);
	}
	return o;
}

int measure_expire(int d,int m, int y){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int nd=0, i=0,j=0;
	if(y>(tm.tm_year + 1900)){
		for(i=y;i>=(tm.tm_year + 1900);i--){
			if(i==y){
                for(j=m;j>=1;j--){
                    if(j==m)nd += d;
                    else nd += d_in_month(i,j);
                }
			}
			else if(i==(tm.tm_year + 1900)){
                if(i%4==0)nd+=366;
                if(i%4!=0)nd+=365;
                for(j=1;j<=(tm.tm_mon + 1);j++){
                    if(j==(tm.tm_mon + 1))nd -= tm.tm_mday;
                    else nd -= d_in_month(i,j);
                }
			}
			else{
                if(i%4==0)nd+=366;
                if(i%4!=0)nd+=365;
			}
			//printf("%d\n",nd);
            }
		return nd;
	}
	else if(y==(tm.tm_year + 1900) && m>(tm.tm_mon + 1)){
		for(i=m;i>=(tm.tm_mon + 1);i--){
			if(i==m)nd += d;
			else if(i==(tm.tm_mon + 1)) nd+=d_in_month(y,i)-tm.tm_mday;
			else nd += d_in_month(y,i);
		}
		return nd;
	}
	else if(y==(tm.tm_year + 1900)&& m==(tm.tm_mon + 1)&&d>=tm.tm_mday){
		return tm.tm_mday-d;
	}
	return 0;
}



filter *sort_elements(product *element){
    product *current = element;
	filter *fcurrent, *fhead = NULL;
    int dte;
    while (current!=NULL){
        dte = measure_expire(current->day,current->mon,current->year);
        if(dte!=0){
            fcurrent = (filter *)malloc(sizeof(filter));
            fcurrent->address = current;
            fcurrent->expire = dte;
            fcurrent->next = fhead;
            fhead = fcurrent;
        }
        current = current->next;
    }
    filter *scurrent , *shead = NULL, *lap = fhead , *dtmp, *ptmp, *pptmp;
    int Max = 0;
    while (lap!=NULL){
         ptmp = NULL;
        while (fcurrent!=NULL){
            if(fcurrent->expire>=Max ){
                pptmp = ptmp;
                dtmp = fcurrent;
                Max=fcurrent->expire;
            }
            ptmp = fcurrent;
            fcurrent=fcurrent->next;
        }
        scurrent = (filter *)malloc(sizeof(filter));
        scurrent->expire = Max;
        scurrent->address = dtmp->address;
        scurrent->next=shead;
        shead = scurrent;
        if(pptmp!=NULL){
            pptmp->next = dtmp->next;
            free(dtmp);
        }else{
            fhead=dtmp->next;
            free(dtmp);
        }
        fcurrent=fhead;
        Max = 0;
        if(fhead==NULL){
            lap = fhead;
        }
    }
    return shead;
}

filter *find_expired(product *element){
    product *current = element;
    filter *ehead = NULL, *ecurrent;
    int dte;
    while (current!=NULL){
        dte = measure_expire(current->day,current->mon,current->year);
        if(dte==0){
            ecurrent = (filter *)malloc(sizeof(filter));
            ecurrent->address = current;
            ecurrent->expire = dte;
            ecurrent->next = ehead;
            ehead = ecurrent;
        }
        current = current->next;
    }

    return ehead;
}

void pop_by_address(product *address,product **element){
    if(*element==NULL) return;
    product *current = *element;
    product *tmp;
    tmp=NULL;
    while(current!=NULL){
        if(current==address){
            if(tmp!=NULL){
                tmp->next=current->next;
                free(current);
            }
            else{
                *element= current->next;
                free(current);

            }
            break;
        }else{
            tmp = current;
            current=current->next;
        }
    }
}
