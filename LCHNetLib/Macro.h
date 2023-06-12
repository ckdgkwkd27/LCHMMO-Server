#define RETURN_FALSE_ON_FAIL(cond) if(cond == false) return false;

#define ASSERT_CRASH(expr)				\
{										\
	if(!expr)							\
	{									\
		int* crashVal = nullptr;		\
		*crashVal = 0xDEADBEEF;			\
	}									\
}