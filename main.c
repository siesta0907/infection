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


int trackInfester(int patient_no, int detected_time, int *place); //������ ��ȣ �����ϴ� �Լ� 

int main(int argc, const char * argv[]) {
    
    int menu_selection; //�޴� ���� ��ȣ 
    void *ifct_element; //���� �ҷ��ö� ���� ������ 
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    
    int i; //for�� �ε��� 
    int p_no, min_age, max_age; //�Է¹��� ȯ�� ��ȣ, �ּ� ����, �ִ� ���� 
    
	char place_[MAX_PLACENAME];  //�Է¹��� ��� 
	int isplace;  //��� �ִ��� Ȯ���� ���� 
	int infester = -1; //������ ȯ�� �ε���
	int places[5]; //ȯ���߰��Ҷ� ���� ����, �̵���� ������ ��� 
	 
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
                
				printf("�Է��Ͻ� ȯ���� �����ڸ� ������ ���� �����ڸ� ����մϴ�. \n");
				printf("ȯ�� ��ȣ�� �Է��ϼ���: ");
            	scanf("%d", &p_no);
            	
				
				do{
					for(i =0; i< 5; i++)
					{
	                    places[i] = ifctele_getHistPlaceIndex(ifctdb_getData(p_no), i);
					}
					infester = trackInfester(p_no, ifctele_getinfestedTime(ifctdb_getData(p_no)), &places[0]);
					p_no = infester;
				} while(infester != -1);
				
						
				break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    
    return 0;
}

//������ ã�� �Լ�
//ȯ�� ��ȣ�� �ߺ� Ȯ�ε� ��¥, 5���� �̵� ���(�迭)�� �Է¹����� �������� �ε����� ����, ������ -1 ����
 
int trackInfester(int patient_no, int detected_time, int *place){
	
	
	int i, j; //�ݺ��� ���� 
	int infestTime = detected_time; //������ ��¥, detected_time���� �ʱ� 
	int infestplace;
	int  p_no , p_time, p_place1, p_place2; //��ȯ���� ������ ���� ������ 
	int infesterNum = -1; //������ ��ȣ 

	
	for (i =0; i< ifctdb_len(); i++) { 	//ȯ�� �� ��ŭ �ݺ� 
		if (patient_no != i) 	//���ϴ� ȯ�� ����Ʈ�߿��� �ڱ� �ڽ��� �����ϰԲ� 
		{
			p_no = i;
		   	p_time = ifctele_getinfestedTime(ifctdb_getData(i));
			p_place1 = ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY-1); //��ȯ���� �ߺ� Ȯ�� ���ϳ� ��� 
			p_place2 = ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY-2); //��ȯ���� �ߺ� Ȯ�� ���� ��� 
			
			for(j =0; j<3; j++) //���������� ��¥��ŭ ���� 
			{
				//�ߺ� Ȯ�� ���ϳ� ���� ���, �ð��� �־����� �˻� 
				if ((place[0+j] == p_place1) && (detected_time -4+j == p_time))
				{
					if(infestTime > p_time) 
					{
						infestTime = p_time; 
						infesterNum = p_no;
						infestplace = p_place1;
					}
					
				}
				
				//�ߺ� Ȯ�� ���� ���� ���, �ð��� �־����� �˻� 
				if ((place[0+j] == p_place2) && (detected_time -4+j == p_time-1))
				{
					if(infestTime > p_time) 
					{
						infestTime = p_time; 
						infesterNum = p_no;
						infestplace = p_place2;
					}
				}
				
			}
		}
		
	}
	if (infesterNum == -1){
		printf("%d�� ȯ�ڴ� ���� �������Դϴ�. \n", patient_no);
	} 
	else
	{
		printf("%d�� ȯ�ڴ� %d �� ȯ�ڿ��� %d�Ͽ� %s ���� �������Ͽ����ϴ�. \n", patient_no, infesterNum, infestTime, ifctele_getPlaceName(infestplace));
	}
	return infesterNum;
}
