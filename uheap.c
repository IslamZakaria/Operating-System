
#include <inc/lib.h>

// malloc()
//	This function use BEST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
//uint32 UserHeapMemory [no_of_pages];
//uint32 no_of_pages=(USER_HEAP_MAX-USER_HEAP_START )/PAGE_SIZE;
uint32 UserHeapMemory[131072];
struct saved{
	uint32 add;
	int pagesno;
			};


struct saved freearr[1000];
struct saved allo_object[1000];
int obj_idex=0;
int idex =0;

void* malloc(uint32 size)
{
int desired_pages=ROUNDUP(size,PAGE_SIZE)/PAGE_SIZE;
uint32 adda[100];
int availsp[100];
uint32 start_address = 0;
int pgs=0;
uint32 * Page_Table_Ptr=NULL;
int idx=0;
int segms=0;
int h = 99999999;
		if (sys_isUHeapPlacementStrategyBESTFIT())
		{
		for(uint32 i = USER_HEAP_START; i<USER_HEAP_MAX; i+=PAGE_SIZE)
		{
		 if (UserHeapMemory[(i-USER_HEAP_START)/PAGE_SIZE]==0)
		 {
			 if (start_address==0)
			 {

				 start_address=i;
				 adda[idx]=i;
				 availsp[idx]=1;
				 segms+=1;
			 }
			 else
			 {
				 availsp[idx]+=1;
			 }
		 }
		 else if (UserHeapMemory[(i-USER_HEAP_START)/PAGE_SIZE]!=0 && start_address!=0)
		 {
			 start_address=0;
			 idx+=1;
		 }

}
    if (segms == 0)
    {
    	return NULL ;
    }

    for(int i = 0 ; i<segms;i++)
    {
    	if (availsp[i]>=desired_pages && availsp[i]<h)
    	{
    		h=availsp[i];
    		start_address= adda[i];
    	}

    }


    if( h == 99999999)
    {
    	return NULL ;
    }
    		uint32 index_uh = (start_address-USER_HEAP_START)/PAGE_SIZE;
  		    for (int i = index_uh ; i<desired_pages+index_uh;i++)
  		    {
  		    	UserHeapMemory[i]=1;
  		    }
  			sys_allocateMem(start_address,desired_pages);
  		   freearr[idex].add=start_address;
  		   freearr[idex].pagesno=desired_pages;
  		   idex++;
  			return (void*)start_address;
		}
		return NULL;
}

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	//TODO: [PROJECT 2019 - MS2 - [6] Shared Variables: Creation] smalloc() [User Side]
	// Write your code here, remove the panic and write your code
	//panic("smalloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement BEST FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	int desired_pages=ROUNDUP(size,PAGE_SIZE)/PAGE_SIZE;
	uint32 adda[100];
	int availsp[100];
	uint32 start_address = 0;
	int pgs=0;
	uint32 * Page_Table_Ptr=NULL;
	int idx=0;
	int segms=0;
	int h = 99999999;
			if (sys_isUHeapPlacementStrategyBESTFIT())
			{
			for(uint32 i = USER_HEAP_START; i<USER_HEAP_MAX; i+=PAGE_SIZE)
			{
			 if (UserHeapMemory[(i-USER_HEAP_START)/PAGE_SIZE]==0)
			 {
				 if (start_address==0)
				 {

					 start_address=i;
					 adda[idx]=i;
					 availsp[idx]=1;
					 segms+=1;
				 }
				 else
				 {
					 availsp[idx]+=1;
				 }
			 }
			 else if (UserHeapMemory[(i-USER_HEAP_START)/PAGE_SIZE]!=0 && start_address!=0)
			 {
				 start_address=0;
				 idx+=1;
			 }

	}
	    if (segms == 0)
	    {
	    	return NULL ;
	    }

	    for(int i = 0 ; i<segms;i++)
	    {
	    	if (availsp[i]>=desired_pages && availsp[i]<h)
	    	{
	    		h=availsp[i];
	    		start_address= adda[i];
	    	}

	    }


	    if( h == 99999999)
	    {
	    	return NULL ;
	    }
	    		int vl = sys_createSharedObject(sharedVarName,desired_pages,isWritable,(void *)start_address);
	    		if (vl==E_SHARED_MEM_EXISTS||vl==E_NO_SHARE)
	    		{
	    			return NULL;
	    		}
	    		uint32 index_uh = (start_address-USER_HEAP_START)/PAGE_SIZE;
	  		    for (int i = index_uh ; i<desired_pages+index_uh;i++)
	  		    {
	  		    	UserHeapMemory[i]=1;
	  		    }

	  			//sys_allocateMem(start_address,desired_pages);
	  		    freearr[idex].add=start_address;
	    		freearr[idex].pagesno=desired_pages;
	  		    idex++;
	  			return (void*)start_address;
			}
			return NULL;

	//	3) Call sys_createSharedObject(...) to invoke the Kernel for allocation of shared variable
	//		sys_createSharedObject(): if succeed, it returns the ID of the created variable. Else, it returns -ve
	//	4) If the Kernel successfully creates the shared variable, return its virtual address
	//	   Else, return NULL

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyBESTFIT() to check the current strategy

	//change this "return" according to your answer
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	//TODO: [PROJECT 2019 - MS2 - [6] Shared Variables: Get] sget() [User Side]
	int si = sys_getSizeOfSharedObject(ownerEnvID,sharedVarName);
	if (si ==E_SHARED_MEM_NOT_EXISTS)
	{
		return NULL;
	}
	int desired_pages=ROUNDUP(si,PAGE_SIZE)/PAGE_SIZE;
		uint32 adda[100];
		int availsp[100];
		uint32 start_address = 0;
		int pgs=0;
		uint32 * Page_Table_Ptr=NULL;
		int idx=0;
		int segms=0;
		int h = 99999999;
				if (sys_isUHeapPlacementStrategyBESTFIT())
				{
				for(uint32 i = USER_HEAP_START; i<USER_HEAP_MAX; i+=PAGE_SIZE)
				{
				 if (UserHeapMemory[(i-USER_HEAP_START)/PAGE_SIZE]==0)
				 {
					 if (start_address==0)
					 {

						 start_address=i;
						 adda[idx]=i;
						 availsp[idx]=1;
						 segms+=1;
					 }
					 else
					 {
						 availsp[idx]+=1;
					 }
				 }
				 else if (UserHeapMemory[(i-USER_HEAP_START)/PAGE_SIZE]!=0 && start_address!=0)
				 {
					 start_address=0;
					 idx+=1;
				 }

		}
		    if (segms == 0)
		    {
		    	return NULL ;
		    }

		    for(int i = 0 ; i<segms;i++)
		    {
		    	if (availsp[i]>=desired_pages && availsp[i]<h)
		    	{
		    		h=availsp[i];
		    		start_address= adda[i];
		    	}

		    }


		    if( h == 99999999)
		    {
		    	return NULL ;
		    }
		    		int vl = sys_getSharedObject(ownerEnvID,sharedVarName,(void *)start_address);
		    		if (vl<0)
		    		{
		    			return NULL;
		    		}
		    		uint32 index_uh = (start_address-USER_HEAP_START)/PAGE_SIZE;
		  		    for (int i = index_uh ; i<desired_pages+index_uh;i++)
		  		    {
		  		    	UserHeapMemory[i]=1;
		  		    }

		  			//sys_allocateMem(start_address,desired_pages);
		  		    freearr[idex].add=start_address;
		    		freearr[idex].pagesno=desired_pages;
		  		    idex++;
		  			return (void*)start_address;
				}
				return NULL;

	//	1) Get the size of the shared variable (use sys_getSizeOfSharedObject())
	//	2) If not exists, return NULL
	//	3) Implement BEST FIT strategy to search the heap for suitable space
	//		to share the variable (should be on 4 KB BOUNDARY)
	//	4) if no suitable space found, return NULL
	//	 Else,
	//	5) Call sys_getSharedObject(...) to invoke the Kernel for sharing this variable
	//		sys_getSharedObject(): if succeed, it returns the ID of the shared variable. Else, it returns -ve
	//	6) If the Kernel successfully share the variable, return its virtual address
	//	   Else, return NULL
	//

}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2019 - MS2 - [5] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
	int y = 0 ;
	//struct saved claer_entries;
	for (int i = 0; i < idex ; i++)
	{
		if (freearr[i].add==(uint32)virtual_address)
		{
			uint32 in = (freearr[i].add-USER_HEAP_START)/PAGE_SIZE;
			for(int y = in ; y < freearr[i].pagesno+in ;y++)
			{
				UserHeapMemory[y]=0;
			}
			sys_freeMem(freearr[i].add,freearr[i].pagesno);
			freearr[i].add=0;
			freearr[i].pagesno=0;
			break;
		}
	}


	//you should get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details
}


//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//=============
// [1] sfree():
//=============
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	//TODO: [PROJECT 2019 - BONUS4] Free Shared Variable [User Side]
	// Write your code here, remove the panic and write your code
	panic("sfree() is not implemented yet...!!");

	//	1) you should find the ID of the shared variable at the given address
	//	2) you need to call sys_freeSharedObject()

}


//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT 2019 - BONUS3] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

}
