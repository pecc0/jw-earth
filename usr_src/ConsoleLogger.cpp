/*
 * ConsoleLogger.cpp
 *
 *  Created on: Oct 28, 2010
 *      Author: Petko
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ConsoleLogger.h"

#define PRINT_TO_G_BUFFER va_list args; \
va_start( args, pattern); \
g_buffer[0] = 0; \
vsprintf( g_buffer, pattern, args ); \
va_end(args);

namespace jw
{
namespace log
{

ConsoleLogger::ConsoleLogger() :
	m_strName("")
{

}

ConsoleLogger::ConsoleLogger(const string& name) :
	m_strName(name)
{
}

ConsoleLogger::~ConsoleLogger()
{
}

char g_buffer[512] =
{ 0 };
void jw::log::ConsoleLogger::print(const string& str)
{
	printf("[%s]: %s\n", m_strName.c_str(), str.c_str());
	fflush(stdout);
}

void ConsoleLogger::trace(const char *pattern, ...)
{
	PRINT_TO_G_BUFFER;
	print(g_buffer);
}

void ConsoleLogger::debug(const char *pattern, ...)
{
	PRINT_TO_G_BUFFER;
	print(g_buffer);
}

void ConsoleLogger::info(const char *pattern, ...)
{
	PRINT_TO_G_BUFFER;
	print(g_buffer);
}

void ConsoleLogger::warn(const char *pattern, ...)
{
	PRINT_TO_G_BUFFER;
	print(g_buffer);
}

void ConsoleLogger::error(const char *pattern, ...)
{
	PRINT_TO_G_BUFFER;
	print(g_buffer);
}

}
}
