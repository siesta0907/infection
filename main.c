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


int trackInfester(int patient_no, int detected_time, int *place); //전파자 번호 리턴하는 함수 

int main(int argc, const char * argv[]) {
    
    int menu_selection; //메뉴 고르는 번호 
    void *ifct_element; //파일 불러올때 쓰는 변수들 
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    
    int i; //for문 인덱스 
    int p_no, min_age, max_age; //입력받을 환자 번호, 최소 나이, 최대 나이 
    
	char place_[MAX_PLACENAME];  //입력받은 장소 
	int isplace;  //장소 있는지 확인할 변수 
	int infester = -1; //전파자 환자 인덱스
	int places[5]; //환자추격할때 쓰는 변수, 이동경로 저장할 장소 
	 
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
                
				printf("입력하신 환자의 전파자를 추적해 최초 전파자를 출력합니다. \n");
				printf("환자 번호를 입력하세요: ");
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

//전파자 찾는 함수
//환자 번호와 발병 확인된 날짜, 5개의 이동 장소(배열)를 입력받으면 전파자의 인덱스를 리턴, 없으면 -1 리턴
 
int trackInfester(int patient_no, int detected_time, int *place){
	
	
	int i, j; //반복문 변수 
	int infestTime = detected_time; //감염된 날짜, detected_time으로 초기 
	int infestplace;
	int  p_no , p_time, p_place1, p_place2; //비교환자의 정보를 담을 변수들 
	int infesterNum = -1; //전파자 번호 

	
	for (i =0; i< ifctdb_len(); i++) { 	//환자 수 만큼 반복 
		if (patient_no != i) 	//비교하는 환자 리스트중에서 자기 자신을 제외하게끔 
		{
			p_no = i;
		   	p_time = ifctele_getinfestedTime(ifctdb_getData(i));
			p_place1 = ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY-1); //비교환자의 발병 확인 당일날 장소 
			p_place2 = ifctele_getHistPlaceIndex(ifctdb_getData(i), N_HISTORY-2); //비교환자의 발병 확인 전날 장소 
			
			for(j =0; j<3; j++) //감염가능한 날짜만큼 조사 
			{
				//발병 확인 당일날 같은 장소, 시간에 있었는지 검사 
				if ((place[0+j] == p_place1) && (detected_time -4+j == p_time))
				{
					if(infestTime > p_time) 
					{
						infestTime = p_time; 
						infesterNum = p_no;
						infestplace = p_place1;
					}
					
				}
				
				//발병 확인 전날 같은 장소, 시간에 있었는지 검사 
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
		printf("%d번 환자는 최초 전파자입니다. \n", patient_no);
	} 
	else
	{
		printf("%d번 환자는 %d 번 환자에게 %d일에 %s 에서 감염당하였습니다. \n", patient_no, infesterNum, infestTime, ifctele_getPlaceName(infestplace));
	}
	return infesterNum;
}
