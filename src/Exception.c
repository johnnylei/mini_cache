#include <stdlib.h>
#include <strings.h>
#include "Exception.h"

void exceptionDestroy(void *object) {
	ExcepSign * exception = (ExcepSign *)object;
	free(exception->message);
	free(exception);
}

ExcepSign * initException() {
	ExcepSign * exception = (ExcepSign *)malloc(sizeof(ExcepSign));
	bzero(exception, sizeof(ExcepSign));
	exception->message = (char *)malloc(EXCEPTION_MESSAGE_SIZE);
	bzero(exception->message, EXCEPTION_MESSAGE_SIZE);
	exception->ExcepType = 0;

	exception->destroy = exceptionDestroy;
	return exception;
}
