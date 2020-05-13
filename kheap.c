#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
struct saved{
	uint32 add;
	int pagesno;
			};


struct saved freearr[100];
int idex =0;

void* kmalloc(unsigned int size)
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
	    if (isKHeapPlacementStrategyBESTFIT())
	    {
	    	for(int i = KERNEL_HEAP_START; i<KERNEL_HEAP_MAX; i+=PAGE_SIZE)
	    {
	    	 struct Frame_Info* ptr_frame_info;
	    	 ptr_frame_info= get_frame_info(ptr_page_directory, (void *)i, &Page_Table_Ptr);
	    		 if (ptr_frame_info==0)
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
	    		 else if (ptr_frame_info!=NULL && start_address!=0)
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
	    }
	    else if (isKHeapPlacementStrategyFIRSTFIT())
	    {
	    	int w=0;
	    	for(int i = KERNEL_HEAP_START; i<KERNEL_HEAP_MAX; i+=PAGE_SIZE)
	    	    {
	    	    	 struct Frame_Info* ptr_frame_info;
	    	    	 ptr_frame_info= get_frame_info(ptr_page_directory, (void *)i, &Page_Table_Ptr);
	    	    		 if (ptr_frame_info==0)
	    	    		 {
	    	    			 if (start_address==0)
	    	    			 {

	    	    				 start_address=i;
	    	    				 w=1;
	    	    			 }
	    	    			 else
	    	    			 {
	    	    				 w++;
	    	    				 if (w>=desired_pages)
	    	    					 break;
	    	    			 }
	    	    		 }
	    	    		 else if (ptr_frame_info!=NULL && start_address!=0)
	    	    		 {
	    	    			 start_address=0;
	    	    			 w=0;
	    	    		 }
	    	    }



	    	if (w<desired_pages)
	    		return NULL;
	    }
	    uint32 sa = start_address ;
	    for (int i =0 ; i<desired_pages;i++)
	   	 {
	   		struct Frame_Info* ptr_frame_info;
	   		allocate_frame(&ptr_frame_info);
	   		map_frame(ptr_page_directory, ptr_frame_info, (void*)start_address, PERM_PRESENT|PERM_WRITEABLE);
	   		start_address+=PAGE_SIZE;
	   	 }





	    freearr[idex].add=sa;
	    freearr[idex].pagesno=desired_pages;
	    idex++;
	    return (void *)sa;


}

void kfree(void* virtual_address)
{
	int pgs=0;

		for (int i=0;i<100;i++)
		{
			if((void *)(freearr[i].add)==virtual_address)
			{
				pgs=freearr[i].pagesno;
				freearr[i].add=0;
				freearr[i].pagesno=0;
				break ;
			}

		}
		if (pgs != 0)
		{
			for (int y=0 ; y<pgs ; y++)
		{

			unmap_frame(ptr_page_directory,virtual_address);
			virtual_address+=PAGE_SIZE;

		}

		}



}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	uint32 * ptr_table;
	uint32 frameNO ;
	uint32 start ;
	for(int i = 0 ; i <idex  ; i++)
	{
		start =freearr[i].add;
		for (int y = 0 ;y<freearr[i].pagesno;y++)
		{
		get_page_table(ptr_page_directory,(void *)start,&ptr_table);
		frameNO=ptr_table[PTX(start)];
		frameNO=frameNO >> 12;
		frameNO*=PAGE_SIZE;
		if (physical_address == frameNO)
		{
			return start;
		}
		start+=PAGE_SIZE;
		}

	}

	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	uint32 *ptr_table=NULL;
	uint32 frameNO;
	get_page_table(ptr_page_directory,(void *)virtual_address,&ptr_table);
	if (ptr_table==NULL)
		return 0;
	frameNO=ptr_table[PTX(virtual_address)];
	frameNO=frameNO >> 12;
	frameNO*=PAGE_SIZE;
	return frameNO;
}


//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{

	//TODO: [PROJECT 2019 - BONUS2] Kernel Heap Realloc
	// Write your code here, remove the panic and write your code

	return NULL;
	panic("krealloc() is not implemented yet...!!");

}
