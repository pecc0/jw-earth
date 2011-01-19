/* AUTORIGHTS
Copyright (C) 2010,2011 Petko Petkov (petkodp@gmail.com
      
This file is part of JWorld.

JWorld is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
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
