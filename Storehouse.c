#include "Storehouse.h"


void push_front(product **element);
void select(product *element, char n[CN],int o);
void select_sorted(product *element);
void pop_expired(product **element);
void choose_to_pop(product **element);
void refresh(product **element);

int main(){
    product *element;
    int option;
    element = NULL;
    mread(&element);
    while(1){
    printf("List: \n");
    show(element);
    printf("\n\tStorehouse\n1.Add product\n2.Delete product\n3.Select product\n4.Refresh List\n5.Exit\n\nOption: ");
    scanf("%d",&option);
    switch(option){
        case 1:
            system("cls");
            printf("\nAdd product:\n");
            push_front(&element);
        break;
        case 2:
            system("cls");
            printf("\nDelete product:\n1.Delete chosen product\n2.Delete all expired\n\nOption: ");
            scanf("%d",&option);
            switch(option){
                case 1:;
                    choose_to_pop(&element);
                break;
                case 2:
                	pop_expired(&element);
				break;
            }
        break;
        case 3:
            system("cls");
            char n[CN];
            printf("\nSelect product:\n1.Select by category\n2.Select by name\n3.Sort by date\n\nOption: ");
            scanf("%d",&option);
            switch(option){
                case 1:
                    scanf("%s",n);
                    select(element, n, option);
                break;
                case 2:
                    scanf("%s",n);
                    select(element, n, option);
                break;
                case 3:
                	select_sorted(element);
				break;
            }
        break;
        case 4:
            system("cls");
            refresh(&element);

        break;
        case 5:
            save(element);
            return 0;
        break;

    }
    system("cls");
    }
    save(element);
    return 0;
}

void select(product *element, char n[CN], int o){
    product *current = element;
    if(current==NULL) printf("List is empty");
    else{
        filter *tmp;
        filter *fcurrent=NULL;
        tmp = fcurrent;
        while (current!=NULL && o==2){
            if (!stricmp(current->name,n)){
                fcurrent = (filter *)malloc(sizeof(filter));
                fcurrent->address = current;
                fcurrent->next = tmp;
                tmp = fcurrent;
            }
            current = current->next;
        }
        while (current!=NULL && o==1){
            if (!stricmp(current->category,n)){
                fcurrent = (filter *)malloc(sizeof(filter));
                fcurrent->address = current;
                fcurrent->next = tmp;
                tmp = fcurrent;
            }
            current = current->next;
        }
        show_selected(fcurrent);
    }
}


void push_front(product **element){
    int num, o;
    time_t t = time(NULL);
	struct tm tm = *localtime(&t);
    FILE *fp;
    fp = fopen("Magazyn.txt","a");
    product *current;
    current = (product *)malloc(sizeof(product));
        printf("Name: "); scanf("%s",&current->name);
        printf("Category: "); scanf("%s",&current->category);
        printf("Amount: "); scanf("%d",&current->amount);
        printf("Unit: "); scanf("%s",&current->unit);
        printf("Expiration Date:\nYear: ");
        scanf("%d",&num);
        current->year = push_year(num);
        printf("\nMonth: ");
        scanf("%d",&num);
        current->mon = push_month(num, current->year);
        printf("\nDay: ");
        scanf("%d",&num);
        current->day = push_day(num, current->mon, current->year);
        if(current->year<(tm.tm_year + 1900)){
			if(d_del(o,&current)==2) return;;
		}
		else if(current->year==(tm.tm_year + 1900) && current->mon<(tm.tm_mon + 1)){
			if(d_del(o,&current)==2) return;;
		}
		if(current->mon==(tm.tm_mon + 1) && current->day<=tm.tm_mday){
        	if(d_del(o,&current)==2) return;;
		}
    current->next=(*element);
    *element = current;
    fclose(fp);
}


void select_sorted(product *element ){
    filter *shead = sort_elements(element);
    filter *scurrent = shead, *tmp;
    product *current;
    while(scurrent!=NULL){
        current = scurrent->address;
        printf("%s %s %d %s %d-%d-%d\n",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
        printf("%d days left\n",scurrent->expire);
        tmp = scurrent;
        scurrent=scurrent->next;
        free(tmp);
    }
    filter *ehead = find_expired(element);
    filter *ecurrent = ehead;
    printf("\nExpired: \n\n");
    while(ecurrent!=NULL){
        current = ecurrent->address;
        printf("%s %s %d %s %d-%d-%d\n",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
        tmp = ecurrent;
        ecurrent = ecurrent->next;
        free(tmp);

    }

    printf("\nPress Any Key to Continue\n");
	getch();

}
void choose_to_pop(product **element){
    product *tosell = (product*)malloc(sizeof(product));
    printf("Name: "); scanf("%s",&tosell->name);
    printf("Category: "); scanf("%s",&tosell->category);
    printf("Unit: "); scanf("%s",&tosell->unit);
    filter *shead = sort_elements(*element);
    filter *scurrent,*stmp;
    product *current;
    int amtosell, option;
    printf("Amount: ");
    scanf("%d",&amtosell);
    while(amtosell>0){
        scurrent = shead;
        while(scurrent!=NULL){
            current = scurrent->address;
            if(!stricmp(current->name,tosell->name) && !stricmp(current->category,tosell->category)&& !stricmp(current->unit,tosell->unit)){
                printf("Amount to remove: %d\n\n",amtosell);
                printf("%s %s %d %s %d-%d-%d\n",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
                printf("This supply expires in %d days\nDo you wish to remove product from this supply?\n1.Yes\n2.No\n\nOption: ",scurrent->expire);
                scanf("%d",&option);
                if(option==1){
                    current->amount -=amtosell;
                    if(current->amount<=0){
                        amtosell = abs(current->amount);
                        pop_by_address(current,*&element);
                        printf("\nProduct was removed\nPress Any Key to Continue\n");
                        getch();
                    }else{
                        amtosell=0;
                    }
                }
                if(amtosell==0)break;
            }
            scurrent=scurrent->next;
        }
        if(amtosell!=0){
            printf("Amount to Remove: %d\nDo you still want to continue?\n1.Yes\n2.No\n\nOption: ",amtosell);
            scanf("%d",&option);
            if(option==2) amtosell=0;
        }
    }

}

void pop_expired(product **element){
    filter *ehead = find_expired(*element);
    if(ehead==NULL){
        printf("\nThere's no expired products\nPress Any Key to Continue\n");
        getch();
        return;
    }
    filter *ecurrent = ehead;
    product *current;
    int option;
    printf("\nExpired: \n\n");
    while(ecurrent!=NULL){
        current = ecurrent->address;
        printf("%s %s %d %s %d-%d-%d\n",current->name,current->category,current->amount,current->unit,current->year,current->mon,current->day);
        printf("Do you wish to remove this product?\n1.Yes\n2.No\nOption: ");
        scanf("%d",&option);
        if(option==1){
            pop_by_address(ecurrent->address,*&element);
            printf("\nProduct was removed\nPress Any Key to Continue\n");
            getch();
        }
        ecurrent = ecurrent->next;

    }
}
void refresh(product **element){
    product *current;
    product *lap = *element, *tmp;
    while(lap!=NULL){
        current = lap;
        while(current!=NULL){
            tmp = current->next;
            if(lap!=current && !stricmp(lap->name,current->name) && !stricmp(lap->category,current->category)&& !stricmp(lap->unit,current->unit) && lap->year==current->year && lap->mon==current->mon && lap->day==current->day ){
                lap->amount+=current->amount;
                pop_by_address(current,*&element);
                current = tmp;
            }
            current=tmp;
        }
        current=*element;
        lap=lap->next;
    }
    printf("\nList has been refreshed\nPress Any Key to Continue\n");
    getch();
}
