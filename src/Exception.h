#ifndef _EXCEPTION_H__
#define _EXCEPTION_H__
#include <setjmp.h>

typedef struct tagException {
	jmp_buf StackInfo;
	int ExcepType;
	char message[1024];
} ExcepSign;

// get exception type
#define ExceType(ExcepSign) ((ExcepSign)->ExcepType) 

// get code block with exceptions
#define Try(ExcepSign) if (((ExcepSign)->ExcepType = setjmp((ExcepSign)->StackInfo)) == 0)

// cache exceptions
#define Catch(ExcepSign, ExcepType) else if((ExcepSign)->ExcepType == ExcepType)

// cache exceptions 
#define CatchElse(ExcepSign) else if ((ExcepSign)->ExcepType > 0)

// throw
#define Throw(ExcepSign) longjmp(ExcepSign->StackInfo, ExcepSign->ExcepType)

#endif
