#pragma once
#include <stdio.h>

#ifdef _DEBUG
	#define CHECK_GL_ERROR(Condition)		\
		do									\
		{									\
			Condition;						\
			GLenum no = glGetError();		\
			if (no != 0)					\
			{								\
				printf("GL error: %x: '"#Condition, no);	\
				printf("'\n");				\
				/*assert(false);*/				\
			}								\
		} while (false)

#else
	#define CHECK_GL_ERROR(Condition)	\
		do								\
		{								\
			Condition;					\
		} while (false)

#endif
