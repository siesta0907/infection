//
//  ifct_database.c
//  Infection Path
//  Database platform code for storing infest path elements
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <stdlib.h>

#include "ifct_database.h"

#define LIST_END -1

//node definition for linked list
typedef struct node{
    int index;      //index of the node
    void* obj;      //object data 자신 노드 
    void* next;     //pointer to the next 다음 노드 
    void* prev;     //pointer to the next 이전 노드 
} node_t;


static node_t* list_database = NULL; //노드 데이터베이스 
static node_t* listPtr = NULL; //마지막 노드 
static int list_cnt = 0; //노드 길이 


//Inner functions (cannot used at the outside of this file)
//노드를 생성하는 함수 
static node_t* genNode(void)
{
    //allocate memory for creating the node
    //동적으로 노드 할당 
    node_t* ndPtr = (node_t*)malloc(sizeof(node_t));
    
	//노드, 변수 초기화 
    if (ndPtr != NULL)
    {
        ndPtr->next = NULL;  
        ndPtr->obj = NULL;  
        ndPtr->prev = NULL; 
    }
    
    return ndPtr;
}

static node_t* ifctList(int index)
{
	//현재 데이터베이스 list_database를  ndPtr에 저장 
    node_t* ndPtr = list_database;
    
	if (listPtr != NULL && listPtr->index <= index)
    {
        ndPtr = listPtr;
    }
    
    //return for wrong input
    //인덱스 크기가 -1보다 작거나 리스트의 길이보다 길때 오류 메세지 출력 
    if (index <-1)
    {
        printf("[ERROR] ifctList() : either list is null or index is wrong! (offset : %i)\n", index);
        return NULL;
    }
    if (index >= list_cnt)
    {
        printf("[ERROR] ifctList() : index is larger than length (len:%i, index:%i)\n", list_cnt, index);
        return NULL;
    }
    
    //repeat travelling until the node is the end or offset becomes 0
    //만약  ndptr이 null이 될때까지 탐색 
    while (ndPtr != NULL)
    {
    	//내가 찾고자 하는 노드이면 break 
        if (ndPtr->index == index)
            break;
        //아니라면 다음 노드로 넘어 감 
        ndPtr = ndPtr->next;
    }
    
    return ndPtr;
}

//노드 개수 세는 함수 
static int updateIndex(void)
{
    int index=0;
    node_t* ndPtr = list_database;
    
    while ( ndPtr != NULL )//travel until it is the end node
    {
        ndPtr->index = index++;
        ndPtr = ndPtr->next; //travel once
    }
    
    return index;
}



//API function
/*
    description : adding a data object to the list end
    input parameters : obj - data object to add to the list
    return value : addition result (0 - succeeded, -1 - failed)
    
    operation : 1. make a new node
                2. find the last node in the list
                3. make the last node's next pointer to point the new node
                4. update the index
*/

//노드 추가하는 함수 
int ifctdb_addTail(void* obj)
{
    node_t* ndPtr;
    node_t* newNdPtr;
    
    //parameter checking
    //오류 체크 
    if (obj == NULL)
    {
        printf("[ERROR] ifsdb_addTail() : Failed to do addTail : input object indicates NULL!\n");
        return -1;
    }
    
    //generate new node
    newNdPtr = genNode();
    //생성할 때 오류 발생시 오류 메세지 출력 + 종료 
    if (newNdPtr == NULL)
    {
        printf("[ERROR] ifsdb_addTail() : Failed to do addTail : Failed to create a node\n");
        return -1; //indicate to the upper function that node addition is failed
    }
     
    newNdPtr->obj = obj;
    
    //add node to the list tail
    //현재 데이터베이스에 아무도 없으면 
	if (list_database == NULL)
    {
    	//새로운 노드를 데이터베이스에 연결 
        list_database = newNdPtr;
        newNdPtr->index = 0;
    }
    else
    {
    	//이전 노드와 현재 노드 연결 + 인덱스 설정 
        ndPtr = ifctList(list_cnt-1);
        ndPtr->next = newNdPtr;
        newNdPtr->prev = ndPtr;
        newNdPtr->index = ndPtr->index+1;
    }
    
    //마지막 노드를 현재 생성한 노드로 갱신 
    listPtr = newNdPtr;
    
    //길이 증가 
    list_cnt++;
    
    return 0;
}


/*
    description : delete data object from the list
    input parameters : index - index'th data to delete
    return value : deletion result (0 - succeeded, -1 - failed)
*/


//입력한 번호의 노드를 삭제하는 함수 
int ifctdb_deleteData(int index)
{
    node_t* ndPrevPtr;
    node_t* ndNextPtr;
    node_t* delNdPtr;
    
    //parameter checking
    if ( (delNdPtr = ifctList(index)) == NULL)
    {
        printf("[ERROR] ifctdb_deleteData() : Failed to do deleteData : input index is invalid (%i)!\n", index);
        return -1;
    }
    
    ndPrevPtr = delNdPtr->prev;
    if (ndPrevPtr != NULL)
    {
        ndPrevPtr->next = delNdPtr->next;
    }
    ndNextPtr = delNdPtr->next;
    if (ndNextPtr != NULL)
    {
        ndNextPtr->prev = delNdPtr->prev;
    }
    
    free(delNdPtr->obj);
    free(delNdPtr);
    
    list_cnt--;
    
    if (list_cnt == 0)
    {
        list_database = NULL;
    }
    else
    {
        updateIndex();
    }
    
    return 0;
}


//functions for list observation -----------------------------

/*
    description : return the number of data objects in the list
    return value : length
*/

//노드 길이를 리턴하는 함수 
int ifctdb_len(void)
{
    return list_cnt;
}


/*
    description : get the object data
    input parameters : index
    return value : object pointer
*/

//입력하신 번호의 노드를 리턴하는 함수 
void* ifctdb_getData(int index)
{
    void* obj = NULL;
    node_t* ndPtr;
    
    //parameter checking
    if ((ndPtr = ifctList(index)) != NULL)
    {
        obj = (void*)ndPtr->obj;
        listPtr = ndPtr;
    }
    
    if (obj == NULL)
        printf("[ERROR] ifctdb_len() : there is no data of index %i\n", index);
    
    return obj;
}
	
