#include "sysinc.h"
#include "module.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int zbx_module_lua_version(AGENT_REQUEST *request, AGENT_RESULT *result);
static int zbx_module_lua_execute(AGENT_REQUEST *request, AGENT_RESULT *result);

static ZBX_METRIC keys[] =
/* KEY               FLAG           FUNCTION                TEST PARAMETERS */
{
    {"lua.version",   0,             zbx_module_lua_version,  NULL},
    {"lua.execute",   CF_HAVEPARAMS, zbx_module_lua_execute,  NULL},
    {NULL}
};

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_api_version                                           *
 *                                                                            *
 * Purpose: returns version number of the module interface                    *
 *                                                                            *
 * Return value: ZBX_MODULE_API_VERSION - version of module.h module is       *
 *               compiled with, in order to load module successfully Zabbix   *
 *               MUST be compiled with the same version of this header file   *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_api_version(void)
{
	return ZBX_MODULE_API_VERSION;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_init                                                  *
 *                                                                            *
 * Purpose: the function is called on agent startup                           *
 *          It should be used to call any initialization routines             *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - module initialization failed               *
 *                                                                            *
 * Comment: the module won't be loaded in case of ZBX_MODULE_FAIL             *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_init(void)
{
	srand(time(NULL));

	return ZBX_MODULE_OK;
}

/******************************************************************************
 *                                                                            *
 * Function: zbx_module_uninit                                                *
 *                                                                            *
 * Purpose: the function is called on agent shutdown                          *
 *          It should be used to cleanup used resources if there are any      *
 *                                                                            *
 * Return value: ZBX_MODULE_OK - success                                      *
 *               ZBX_MODULE_FAIL - function failed                            *
 *                                                                            *
 ******************************************************************************/
int	zbx_module_uninit(void)
{
	return ZBX_MODULE_OK;
}

/******************************************************************************
*                                                                            *
* Function: zbx_module_item_list                                             *
*                                                                            *
* Purpose: returns list of item keys supported by the module                 *
*                                                                            *
* Return value: list of item keys                                            *
*                                                                            *
******************************************************************************/
ZBX_METRIC    *zbx_module_item_list()
{
    return keys;
}

static int zbx_module_lua_version(AGENT_REQUEST *request, AGENT_RESULT *result)
{
    SET_STR_RESULT(result, strdup(LUA_VERSION));
    return SYSINFO_RET_OK;
}

static int zbx_module_lua_execute(AGENT_REQUEST *request, AGENT_RESULT *result)
{
    char *script;
	lua_State *L = luaL_newstate();
    int res;

    if (1 != request->nparam)
    {
        SET_MSG_RESULT(result, strdup("Invalid number of parameters"));
        return SYSINFO_RET_FAIL;
    }

    script = get_rparam(request, 0);

    luaL_openlibs(L);

    res = luaL_dofile(L, script);
    if (res != LUA_OK) 
    {
        SET_MSG_RESULT(result, strdup("Could not load file"));
        lua_close(L);
        return SYSINFO_RET_FAIL;
    }
    lua_close(L);
    return SYSINFO_RET_OK;
}