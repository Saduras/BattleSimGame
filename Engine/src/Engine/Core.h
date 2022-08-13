#pragma once

#ifdef ENG_PLATFORM_WINDOWS
#if ENG_DYNAMIC_LINK
	#ifdef ENG_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#else
	#define ENGINE_API
#endif
#else
	#error Engine only supports Windows!	
#endif

#ifdef ENG_DEBUG
	#define ENG_ENABLE_ASSERTS
#endif

#ifdef ENG_ENABLE_ASSERTS
#include <string>

	#define ENG_ASSERT(x, msg) { if(!(x)) { ENG_ERROR("Assertion Failed: {0}", msg); throw std::logic_error("Assertion Failed!");} }
	#define ENG_ASSERT_FMT(x, format_str, ...) { if(!(x)) { ENG_ERROR(std::string("Assertion Failed: ") + format_str, __VA_ARGS__); throw std::logic_error("Assertion Failed!");} }
	#define ENG_CORE_ASSERT(x, msg) { if(!(x)) { ENG_CORE_ERROR("Assertion Failed: {0}", msg); throw std::logic_error("Assertion Failed!");} }
	#define ENG_CORE_ASSERT_FMT(x, format_str, ...) { if(!(x)) { ENG_CORE_ERROR(std::string("Assertion Failed: ") + format_str, __VA_ARGS__); throw std::logic_error("Assertion Failed!");} }
#else
	#define ENG_ASSERT(x, ...)
	#define ENG_ASSERT_FMT(x, format_str, ...) 
	#define ENG_CORE_ASSERT(x, ...)
	#define ENG_CORE_ASSERT_FMT(x, format_str, ...)
#endif

#define BIT(x) (1 << x)

#define ENG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define ALIAS_TEMPLATE_FUNCTION(highLevelF, lowLevelF) \
template<typename... Args> \
inline auto highLevelF(Args&&... args) -> decltype(lowLevelF(std::forward<Args>(args)...)) \
{ \
    return lowLevelF(std::forward<Args>(args)...); \
}