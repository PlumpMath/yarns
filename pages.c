#include "pages.h"
#include <assert.h>
#include <stdio.h>
#include "config.h"
#include <string.h>
#include "debug.h"

#define DEBUG_MODULE DEBUG_PAGE_ALLOCATOR

#if YARNS_SELECTED_TARGET == YARNS_TARGET_MACH
#define USE_MACH_VM
#endif

#ifdef USE_MACH_VM
#include <mach/mach.h>
#include <mach/mach_vm.h>

void* page_allocate ( unsigned long bytes, unsigned options, unsigned permissions )
{
	kern_return_t rt;
	void* pointer = 0;
	vm_inherit_t inheritType;
	DEBUG("Allocating %d bytes = %d pages", bytes, bytes / 4096);
	assert(!(bytes % 4096));
	rt = vm_allocate(mach_task_self(), (vm_address_t*)&pointer, bytes, 1);
	assert(pointer);
	DEBUG(" = %p\n", pointer);
	if (options & PAGE_SECURE)
		inheritType = VM_INHERIT_NONE;
	else if (options & PAGE_SHARE)
		inheritType = VM_INHERIT_SHARE;
	else
		inheritType = VM_INHERIT_COPY;
	mach_vm_inherit(mach_task_self(), (mach_vm_address_t)pointer, bytes, inheritType);
	page_permissions(pointer, bytes, permissions);
	return pointer;
}

void page_permissions ( void* ptr, unsigned long bytes, unsigned permissions )
{
	vm_prot_t newProt = VM_PROT_NONE;
	if (permissions & PAGE_READ)
		newProt |= VM_PROT_READ;
	if (permissions & PAGE_WRITE)
		newProt |= VM_PROT_WRITE;
	if (permissions & PAGE_EXECUTE)
		newProt |= VM_PROT_EXECUTE;
	mach_vm_protect(mach_task_self(), (mach_vm_address_t)ptr, bytes, 0, permissions);
}

void page_deallocate ( void* ptr, unsigned long bytes )
{
	DEBUG("freeing %d pages at %p\n", bytes/4096, ptr);
	assert(ptr);
	assert(!(bytes % 4096));
	vm_deallocate(mach_task_self(), (vm_address_t)ptr, bytes);
}
#else
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>

void* page_allocate ( unsigned long bytes, unsigned options, unsigned permissions )
{
	void* ptr = valloc(bytes);
	if (options & PAGE_ZEROFILL)
		memset(ptr, 0, bytes);
	if (options & PAGE_SECURE)
		minherit(ptr, bytes, VM_INHERIT_NONE)
	else if (options & PAGE_SHARE)
		minherit(ptr, bytes, VM_INHERIT_SHARE);
	else
		minherit(ptr, bytes, VM_INHERIT_COPY);
	page_permissions(ptr, bytes, permissions);
	return ptr;
}

void page_permissions ( void* ptr, unsigned long bytes, unsigned permissions )
{
	int perms = 0;
	if (permissions & PAGE_READ)
		perms |= PROT_READ;
	if (permissions & PAGE_WRITE)
		perms |= PROT_WRITE;
	if (permissions & PROT_EXECUTE)
		perms |= PROT_EXECUTE;
	mprotect(ptr, bytes, perms);
}

void page_deallocate ( void* ptr, unsigned long bytes )
{
	(void)bytes;
	free(ptr);
}
#endif
