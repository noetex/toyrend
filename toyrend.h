#ifdef TOYREND_COMPILER_MSVC
	#define debug_break(...) __debugbreak()
#elif defined(TOYREND_COMPILER_GCC)
	#define debug_break() __builtin_trap()
#endif

#ifdef TOYREND_BUILD_DEBUG
	#define Assert(Condition) if(Condition); else debug_break()
#else
	#undef debug_break
	#define Assert(Condition) ((void)(Condition))
#endif

#define Abs(Value) ((Value) < 0 ? -(Value) : (Value))
#define CLAMP(X, Min, Max) ((X) < (Min)) ? (Min) : ((X) > (Max)) ? (Max) : (x)

typedef int bool_t;

typedef struct
{
	uint32_t* Pixels;
	toyrend_system_data;
} toyrend_struct;
