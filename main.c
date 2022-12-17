//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    //for�� �ε��� 
    int i;
    //�Է¹��� ȯ�� ��ȣ, �ּ� ����, �ִ� ���� 
    int p_no, min_age, max_age;
	char place_[MAX_PLACENAME]; 
	//��� �ִ��� Ȯ���� ���� 
	int isplace;
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    
	//main �Լ� �Ű������� 2���� �ƴϸ� ���� ����Ʈ 
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    
    //������ ���� ������ �� ���� ����Ʈ 
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    // �ؽ�Ʈ 3�� �Է¹޾Ƽ� ��ȣ, ����, �ð� �о��  
     while (3 == (fscanf(fp,"%d %d %d ",&pIndex,&age,&time)))
	 {
     	for (i =0; i< 5; i++)
		 {
     		fscanf(fp, "%d ",  &placeHist[i]); //5���� �̵���� placeHist�� ����  
		 }
		 
		ifct_element = ifctele_genElement(pIndex,age,time,placeHist);
    	ifctdb_addTail(ifct_element);
    	
    }
   // printf("���� �� ����\n");
    //1-3. FILE pointer close
    fclose(fp);
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
 
            	printf("�Է��Ͻ� ȯ���� ������ ����մϴ�. \n");
				printf("ȯ�� ��ȣ�� �Է��ϼ���: ");
            	scanf("%d", &p_no);
            	ifctele_printElement(ifctdb_getData(p_no));
                
                break;
                
            case MENU_PLACE:
            	
				printf("�Է��Ͻ� ��ҿ��� ������ ȯ�ڵ��� ������ ����մϴ�. \n"); 
            	printf("��Ҹ� ���ڿ��� �Է����ּ���: ");
                scanf("%s", &place_);
                
                isplace = 0;
                
                for(i = 0; i< N_PLACE; i++){
                	//��Ҹ� ����� �Է������� isplace�� 1�� ���� 
                	if (strcmp(place_, ifctele_getPlaceName(i)) == 0) isplace =1;
				}
                
                if (!isplace){
                	printf("�Է��Ͻ� %s �� ���� ����Դϴ�.\n", place_);
				}
				
				else{
					printf("%s���� ������ ȯ�ڵ��� �����Դϴ�.\n", place_);
					for (i = 0; i < ifctdb_len(); i++)
					{
						//�Է��� ��ҿ��� ������ ȯ�� ���� ��� 
						if  (strcmp(place_, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY -1))) ==0)
						{
							ifctele_printElement(ifctdb_getData(i));
						}
					}
				}
				
                break;
                
            case MENU_AGE:
            	
				printf("�Է��Ͻ� ���� ������ �ش��ϴ� ȯ�ڵ��� ������ ����մϴ�. \n");
				printf("�ּ� ���̿� �ִ� ���̸� �Է��ϼ���(�������� ����)): ");
            	scanf("%d %d", &min_age, &max_age);

	           	for (i = 0; i < ifctdb_len(); i++){
	           		
	                if (min_age <= ifctele_getAge(ifctdb_getData(i)) && max_age >= ifctele_getAge(ifctdb_getData(i)))
	                {
	                	ifctele_printElement(ifctdb_getData(i));
					}
	                    		
	            }

                break;
                
            case MENU_TRACK:
                
				printf("�� ȯ���� �����ڿ� �������� ���� �� ��� �׸��� ���� �����ڸ� ����մϴ�. \n"); 
            	for (i = 0; i < ifctdb_len(); i++)
				{
					
				}
				    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    //printf("����\n"); 
    
    return 0;
}
