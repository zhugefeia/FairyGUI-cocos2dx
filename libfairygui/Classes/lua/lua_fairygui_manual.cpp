#include "lua/lua_fairygui_manual.hpp"
#include "FairyGUI.h"
#include "FairyGUIMacros.h"

#include "GLabel.h"

#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"

#include "scripting/lua-bindings/manual/CCLuaEngine.h"

//using namespace fairygui;

static int handleFairyguiEvent(int handler, fairygui::EventContext* sender)
{
	LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
	//tolua_pushusertype(stack->getLuaState(), (void*)sender, "fgui.EventContext");
	object_to_luaval<fairygui::EventContext>(stack->getLuaState(), "fgui.EventContext", (fairygui::EventContext*)sender);
	stack->executeFunctionByHandler(handler, 1);
	stack->clean();
	return 0;
}
static int handleFairyguiEventNoParams(int handler)
{
	LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
	stack->executeFunctionByHandler(handler, 0);
	stack->clean();
	return 0;
}
/*
static int handleFairyguiEvent(int handler, fairygui::EventContext* sender, int tag)
{
LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
//tolua_pushusertype(stack->getLuaState(), (void*)sender, "fgui.EventContext");
object_to_luaval<fairygui::EventContext>(stack->getLuaState(), "fgui.EventContext", (fairygui::EventContext*)sender);
stack->pushInt(tag);
stack->executeFunctionByHandler(handler, 2);
stack->clean();
return 0;
}
*/
/*
void margin_to_luaval(lua_State* L, const fairygui::Margin& _margin)
{
	if (NULL == L)
		return;
	lua_newtable(L);
	lua_pushstring(L, "left");
	lua_pushnumber(L, (lua_Number)_margin.left);
	lua_rawset(L, -3);
	lua_pushstring(L, "top");
	lua_pushnumber(L, (lua_Number)_margin.top);
	lua_rawset(L, -3);
	lua_pushstring(L, "right");
	lua_pushnumber(L, (lua_Number)_margin.right);
	lua_rawset(L, -3);
	lua_pushstring(L, "bottom");
	lua_pushnumber(L, (lua_Number)_margin.bottom);
	lua_rawset(L, -3);
}

static bool luaval_to_margin(lua_State* L, int lo, fairygui::Margin* outValue, const char* funcName)
{
	if (NULL == L || NULL == outValue)
		return false;

	bool ok = true;

	tolua_Error tolua_err;
	if (!tolua_istable(L, lo, 0, &tolua_err))
	{
#if COCOS2D_DEBUG >=1
		luaval_to_native_err(L, "#ferror:", &tolua_err, funcName);
#endif
		ok = false;
	}

	if (ok)
	{
		lua_pushstring(L, "left");
		lua_gettable(L, lo);
		outValue->left = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "top");
		lua_gettable(L, lo);
		outValue->top = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "right");
		lua_gettable(L, lo);
		outValue->right = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_pushstring(L, "bottom");
		lua_gettable(L, lo);
		outValue->bottom = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	return ok;
}
*/

static int lua_cocos2dx_fairygui_addClickListener(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::GObject* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.GObject", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::GObject*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_addClickListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (1 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 2, 0));

		self->addClickListener([=](fairygui::EventContext* sender) {
			handleFairyguiEvent(handler, sender);
		});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}
	if (2 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 2, 0));

		bool ok = true;
		int tag;
		ok &= luaval_to_int32(L, 3, (int *)&tag, "lua_cocos2dx_fairygui_addClickListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_addClickListener'", nullptr);
			return 0;
		}

		self->addClickListener([=](fairygui::EventContext* sender) {
			handleFairyguiEvent(handler, sender);
		}, fairygui::EventTag(tag));
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}

	luaL_error(L, "'addClickListener' function of GObject has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'addClickListener'.", &tolua_err);
				return 0;
#endif
}


static int lua_cocos2dx_fairygui_removeClickListener(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::GObject* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.GObject", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::GObject*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_removeClickListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (0 == argc)
	{
		self->removeClickListener(fairygui::EventTag::None);

		return 0;
	}

	if (1 == argc)
	{
		bool ok = true;
		int tag;
		ok &= luaval_to_int32(L, 2, (int *)&tag, "lua_cocos2dx_fairygui_removeClickListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_removeClickListener'", nullptr);
			return 0;
		}

		self->removeClickListener(fairygui::EventTag(tag));

		return 0;
	}

	luaL_error(L, "'removeClickListener' function of GObject has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'removeClickListener'.", &tolua_err);
				return 0;
#endif
}

int lua_cocos2dx_fairygui_GObject_getData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GObject* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "fgui.GObject", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (fairygui::GObject*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_fairygui_GObject_getData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_GObject_getData'", nullptr);
			return 0;
		}
		void* ret = cobj->getData();
		object_to_luaval<cocos2d::Ref>(tolua_S, "cc.Ref", (cocos2d::Ref*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fgui.GObject:getData", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_fairygui_GObject_getData'.", &tolua_err);
#endif

				return 0;
}

static int lua_cocos2dx_fairygui_addEventListener(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::UIEventDispatcher* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.UIEventDispatcher", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::UIEventDispatcher*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_addEventListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (2 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 3, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 3, 0));

		bool ok = true;
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "lua_cocos2dx_fairygui_addEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_addEventListener'", nullptr);
			return 0;
		}

		self->addEventListener(eventType, [=](fairygui::EventContext* sender) {
			handleFairyguiEvent(handler, sender);
		});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}
	if (3 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 3, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 3, 0));

		bool ok = true;
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "lua_cocos2dx_fairygui_addEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_addEventListener'", nullptr);
			return 0;
		}

		int tag;
		ok &= luaval_to_int32(L, 4, (int *)&tag, "lua_cocos2dx_fairygui_addEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_addEventListener'", nullptr);
			return 0;
		}

		self->addEventListener(eventType, [=](fairygui::EventContext* sender) {
			handleFairyguiEvent(handler, sender);
		}, fairygui::EventTag(tag));
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}

	luaL_error(L, "'addEventListener' function of UIEventDispatcher has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'addEventListener'.", &tolua_err);
				return 0;
#endif
}

static int lua_cocos2dx_fairygui_removeEventListener(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::UIEventDispatcher* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.UIEventDispatcher", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::UIEventDispatcher*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_removeEventListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (1 == argc)
	{
		bool ok = true;
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "lua_cocos2dx_fairygui_removeEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_removeEventListener'", nullptr);
			return 0;
		}

		self->removeEventListener(eventType);

		return 0;
	}
	if (2 == argc)
	{
		bool ok = true;
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "lua_cocos2dx_fairygui_removeEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_removeEventListener'", nullptr);
			return 0;
		}
		int tag;
		ok &= luaval_to_int32(L, 3, (int *)&tag, "lua_cocos2dx_fairygui_removeEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_removeEventListener'", nullptr);
			return 0;
		}

		self->removeEventListener(eventType, fairygui::EventTag(tag));

		return 0;
	}

	luaL_error(L, "'removeEventListener' function of UIEventDispatcher has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'removeEventListener'.", &tolua_err);
				return 0;
#endif
}

static int lua_cocos2dx_fairygui_hasEventListener(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::UIEventDispatcher* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.UIEventDispatcher", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::UIEventDispatcher*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_hasEventListener'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (1 == argc)
	{
		bool ok = true;
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "lua_cocos2dx_fairygui_hasEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_hasEventListener'", nullptr);
			return 0;
		}

		self->hasEventListener(eventType);

		return 0;
	}
	if (2 == argc)
	{
		bool ok = true;
		int eventType;
		ok &= luaval_to_int32(L, 2, (int *)&eventType, "lua_cocos2dx_fairygui_hasEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_hasEventListener'", nullptr);
			return 0;
		}
		int tag;
		ok &= luaval_to_int32(L, 3, (int *)&tag, "lua_cocos2dx_fairygui_hasEventListener");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_hasEventListener'", nullptr);
			return 0;
		}

		self->hasEventListener(eventType, fairygui::EventTag(tag));

		return 0;
	}

	luaL_error(L, "'hasEventListener' function of UIEventDispatcher has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_cocos2dx_fairygui_hasEventListener'.", &tolua_err);
				return 0;
#endif
}

/*
int lua_cocos2dx_fairygui_setMargin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "fgui.GComponent", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_fairygui_setMargin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 1)
	{
		fairygui::Margin arg0;

		ok &= luaval_to_margin(tolua_S, 2, &arg0, "fgui.GComponent:setMargin");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_setMargin'", nullptr);
			return 0;
		}
		cobj->setMargin(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fgui.GComponent:setMargin", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_fairygui_setMargin'.", &tolua_err);
				return 0;
#endif
}
*/
/*
int lua_cocos2dx_fairygui_getMargin(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::GComponent* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "fgui.GComponent", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (fairygui::GComponent*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_fairygui_getMargin'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_getMargin'", nullptr);
			return 0;
		}
		fairygui::Margin _margin = cobj->getMargin();
		margin_to_luaval(tolua_S, _margin);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fgui.GComponent:getMargin", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_fairygui_getMargin'.", &tolua_err);
				return 0;
#endif
}
*/

static int lua_cocos2dx_fairygui_play(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::Transition* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.Transition", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::Transition*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_play'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (0 == argc)
	{
		self->play();
		return 0;
	}

	if (1 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 2, 0));

		self->play([=]() {
			handleFairyguiEventNoParams(handler);
		});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}
	if (3 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 4, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 4, 0));

		bool ok = true;
		int times;
		ok &= luaval_to_int32(L, 2, (int *)&times, "lua_cocos2dx_fairygui_play");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_play'", nullptr);
			return 0;
		}

		double delay;
		ok &= luaval_to_number(L, 3, (double *)&delay, "lua_cocos2dx_fairygui_play");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_cocos2dx_fairygui_play'", nullptr);
			return 0;
		}

		self->play(times, (float)delay, [=]() {
			handleFairyguiEventNoParams(handler);
		});
		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}

	luaL_error(L, "'addEventListener' function of Transition has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_cocos2dx_fairygui_play'.", &tolua_err);
				return 0;
#endif
}

static int lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "fgui.UIObjectFactory", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 3 || argc == 4)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 3, 0));

		bool ok = true;
		std::string url;
		ok &= luaval_to_std_string(tolua_S, 2, &url, "lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension'", nullptr);
			return 0;
		}

		std::string retType;
		ok &= luaval_to_std_string(tolua_S, 4, &retType, "lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension'", nullptr);
			return 0;
		}

		fairygui::UIObjectFactory::setPackageItemExtension(url, [=]()->fairygui::GComponent* {
			fairygui::GComponent* ret = nullptr;
			if (retType == "GButton")
				ret = fairygui::GButton::create();
			else if (retType == "GLabel")
				ret = fairygui::GLabel::create();
			else if (retType == "GProgressBar")
				ret = fairygui::GProgressBar::create();
			else if (retType == "GSlider")
				ret = fairygui::GSlider::create();
			else if (retType == "GScrollBar")
				ret = fairygui::GScrollBar::create();
			else if (retType == "GComboBox")
				ret = fairygui::GComboBox::create();
			else
				ret = fairygui::GComponent::create();

			LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
			stack->pushObject(ret, "fgui.GComponent");
			stack->executeFunctionByHandler(handler,1);

			return ret;
		});

		cocos2d::Ref* self = nullptr;
		if (argc == 4) {
			self = (cocos2d::Ref*)tolua_tousertype(tolua_S, 5, 0);
		}
		if (self)
		{
			ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);
		} else {
			ScriptHandlerMgr::getInstance()->addCustomHandler((void*)Director::getInstance(), handler);
		}

		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "fgui.UIObjectFactory:setPackageItemExtension", argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension'.", &tolua_err);
#endif
				return 0;
}

static int lua_cocos2dx_fairygui_GList_setItemRenderer(lua_State* L)
{
	if (nullptr == L)
		return 0;

	int argc = 0;
	fairygui::GList* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "fgui.GObject", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<fairygui::GList*>(tolua_tousertype(L, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L, "invalid 'self' in function 'lua_cocos2dx_fairygui_GList_setItemRenderer'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(L) - 1;

	if (1 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(L, 2, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif

		LUA_FUNCTION handler = (toluafix_ref_function(L, 2, 0));

		self->itemRenderer = [=](int index, fairygui::GObject* obj) {
			LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
			stack->pushInt(index);
			//tolua_pushusertype(stack->getLuaState(), (void*)obj, "fgui.GObject");
			object_to_luaval<fairygui::GObject>(stack->getLuaState(), "fgui.GObject", (fairygui::GObject*)obj);
			stack->executeFunctionByHandler(handler, 2);
			stack->clean();
		};

		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);

		return 0;
	}

	luaL_error(L, "'setItemRenderer' function of GObject has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(L, "#ferror in function 'lua_cocos2dx_fairygui_GList_setItemRenderer'.", &tolua_err);
				return 0;
#endif
}

int lua_cocos2dx_fairygui_EventContext_getDataValue(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "fgui.EventContext", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_fairygui_EventContext_getDataValue'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_EventContext_getDataValue'", nullptr);
			return 0;
		}
		const cocos2d::Value& ret = cobj->getDataValue();
		ccvalue_to_luaval(tolua_S, ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fgui.EventContext:getDataValue", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_fairygui_EventContext_getDataValue'.", &tolua_err);
#endif

				return 0;
}

int lua_cocos2dx_fairygui_EventContext_getData(lua_State* tolua_S)
{
	int argc = 0;
	fairygui::EventContext* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "fgui.EventContext", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (fairygui::EventContext*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_cocos2dx_fairygui_EventContext_getData'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_cocos2dx_fairygui_EventContext_getData'", nullptr);
			return 0;
		}
		void* ret = cobj->getData();
		object_to_luaval<cocos2d::Ref>(tolua_S, "cc.Ref", (cocos2d::Ref*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "fgui.EventContext:getData", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_cocos2dx_fairygui_EventContext_getData'.", &tolua_err);
#endif

				return 0;
}

static void extendUIEventDispatcher(lua_State* L)
{
	lua_pushstring(L, "fgui.UIEventDispatcher");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "addEventListener", lua_cocos2dx_fairygui_addEventListener);
		tolua_function(L, "removeEventListener", lua_cocos2dx_fairygui_removeEventListener);
		tolua_function(L, "hasEventListener", lua_cocos2dx_fairygui_hasEventListener);
	}
	lua_pop(L, 1);
}

static void extendGObject(lua_State* L)
{
	lua_pushstring(L, "fgui.GObject");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "addClickListener", lua_cocos2dx_fairygui_addClickListener);
		tolua_function(L, "removeClickListener", lua_cocos2dx_fairygui_removeClickListener);
		tolua_function(L, "getData", lua_cocos2dx_fairygui_GObject_getData);
	}
	lua_pop(L, 1);
}

/*
static void extendGComponent(lua_State* L)
{
	lua_pushstring(L, "fgui.GComponent");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		//tolua_function(L, "constructFromResource", lua_cocos2dx_fairygui_constructFromResource);
		//tolua_function(L, "setMargin", lua_cocos2dx_fairygui_setMargin);
		//tolua_function(L, "getMargin", lua_cocos2dx_fairygui_getMargin);
	}
	lua_pop(L, 1);
}
*/

static void extendTransition(lua_State* L)
{
	lua_pushstring(L, "fgui.Transition");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "play", lua_cocos2dx_fairygui_play);
	}
	lua_pop(L, 1);
}

static void extendUIObjectFactory(lua_State* L)
{
	lua_pushstring(L, "fgui.UIObjectFactory");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "setPackageItemExtension", lua_cocos2dx_fairygui_UIObjectFactory_setPackageItemExtension);
	}
	lua_pop(L, 1);
}

static void extendGList(lua_State* L)
{
	lua_pushstring(L, "fgui.GList");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "setItemRenderer", lua_cocos2dx_fairygui_GList_setItemRenderer);
	}
	lua_pop(L, 1);
}

static void extendEventContext(lua_State* L)
{
	lua_pushstring(L, "fgui.EventContext");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "getDataValue", lua_cocos2dx_fairygui_EventContext_getDataValue);
		tolua_function(L, "getData", lua_cocos2dx_fairygui_EventContext_getData);
	}
	lua_pop(L, 1);
}

int register_all_fairygui_manual(lua_State* L)
{
	lua_getglobal(L, "_G");
	if (lua_istable(L, -1))
	{
		extendUIEventDispatcher(L);
		extendGObject(L);
		extendEventContext(L);
		//extendGComponent(L);
		extendTransition(L);
		extendUIObjectFactory(L);
		extendGList(L);
	}
	lua_pop(L, 1);

	return 1;
}