#include "base/ccConfig.h"
#ifndef __cocos2dx_fairygui_event_h__
#define __cocos2dx_fairygui_event_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_fairygui_manual(lua_State* tolua_S);

#endif