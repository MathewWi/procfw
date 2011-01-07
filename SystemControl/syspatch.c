#include <pspsdk.h>
#include <pspsysmem_kernel.h>
#include <pspkernel.h>
#include <psputilsforkernel.h>
#include <pspsysevent.h>
#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "loadexec_patch.h"
#include "utils.h"
#include "systemctrl.h"
#include "printk.h"

static STMOD_HANDLER previous;

static int syspatch_module_chain(SceModule2 *mod)
{
#ifdef DEBUG
	if (mod != NULL) {
		printk("%s\n", mod->modname);
	}
#endif

	if(0 == strcmp(mod->modname, "sceMediaSync")) {
		patch_sceLoadExec();
	}
	
	if (previous)
		return (*previous)(mod);

	return 0;
}

void syspatch_init()
{
	setup_module_handler();
	previous = sctrlHENSetStartModuleHandler(&syspatch_module_chain);
	patch_sceLoaderCore();

	sync_cache();
}