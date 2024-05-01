#pragma once

#define DBG_FILE() \
	fprintf(stdout, "In file '%s'\n", __FILE__);

#define DBG_FUNC() \
	fprintf(stdout, "Invoked '%s'\n", __func__);

#define DBG_LINE() \
	fprintf(stdout, "At line %d\n", __LINE__);

#define DBG(...) \
	fprintf(stdout, "-----\n"); \
	DBG_FILE(); \
	DBG_FUNC(); \
	DBG_LINE(); \
	fprintf(stdout, __VA_ARGS__);

#define DBG_TINY() \
	fprintf(stdout, "-----\n"); \
	DBG_FILE(); \
	DBG_FUNC(); \
	DBG_LINE();
