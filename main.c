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
    
    //for문 인덱스 
    int i;
    //입력받을 환자 번호, 최소 나이, 최대 나이 
    int p_no, min_age, max_age;
	char place_[MAX_PLACENAME]; 
	//장소 있는지 확인할 변수 
	int isplace;
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    
	//main 함수 매개변수가 2개가 아니면 오류 프린트 
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    
    //파일을 읽지 못했을 때 오류 프린트 
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    // 텍스트 3개 입력받아서 번호, 나이, 시간 읽어옴  
     while (3 == (fscanf(fp,"%d %d %d ",&pIndex,&age,&time)))
	 {
     	for (i =0; i< 5; i++)
		 {
     		fscanf(fp, "%d ",  &placeHist[i]); //5개의 이동경로 placeHist에 저장  
		 }
		 
		ifct_element = ifctele_genElement(pIndex,age,time,placeHist);
    	ifctdb_addTail(ifct_element);
    	
    }
   // printf("파일 잘 읽음\n");
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
 
            	printf("입력하신 환자의 정보를 출력합니다. \n");
				printf("환자 번호를 입력하세요: ");
            	scanf("%d", &p_no);
            	ifctele_printElement(ifctdb_getData(p_no));
                
                break;
                
            case MENU_PLACE:
            	
				printf("입력하신 장소에서 감염된 환자들의 정보를 출력합니다. \n"); 
            	printf("장소를 문자열로 입력해주세요: ");
                scanf("%s", &place_);
                
                isplace = 0;
                
                for(i = 0; i< N_PLACE; i++){
                	//장소를 제대로 입력했을때 isplace를 1로 설정 
                	if (strcmp(place_, ifctele_getPlaceName(i)) == 0) isplace =1;
				}
                
                if (!isplace){
                	printf("입력하신 %s 는 없는 장소입니다.\n", place_);
				}
				
				else{
					printf("%s에서 감염된 환자들의 정보입니다.\n", place_);
					for (i = 0; i < ifctdb_len(); i++)
					{
						//입력한 장소에서 감염된 환자 정보 출력 
						if  (strcmp(place_, ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY -1))) ==0)
						{
							ifctele_printElement(ifctdb_getData(i));
						}
					}
				}
				
                break;
                
            case MENU_AGE:
            	
				printf("입력하신 나이 범위에 해당하는 환자들의 정보를 출력합니다. \n");
				printf("최소 나이와 최대 나이를 입력하세요(공백으로 구분)): ");
            	scanf("%d %d", &min_age, &max_age);

	           	for (i = 0; i < ifctdb_len(); i++){
	           		
	                if (min_age <= ifctele_getAge(ifctdb_getData(i)) && max_age >= ifctele_getAge(ifctdb_getData(i)))
	                {
	                	ifctele_printElement(ifctdb_getData(i));
					}
	                    		
	            }

                break;
                
            case MENU_TRACK:
                
				printf("각 환자의 전파자와 감염당한 시점 및 장소 그리고 최초 전파자를 출력합니다. \n"); 
            	for (i = 0; i < ifctdb_len(); i++)
				{
					
				}
				    
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    //printf("종료\n"); 
    
    return 0;
}
