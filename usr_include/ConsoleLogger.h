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
 * ConsoleLogger.h
 *
 *  Created on: Oct 28, 2010
 *      Author: Petko
 */

#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_

#include <string>
#include "IJWLogger.h"

using namespace std;

namespace jw
{
namespace log
{


class ConsoleLogger: public IJWLogger
{
private:
	string m_strName;
	void print(const std::string& str);
public:
	ConsoleLogger();
	ConsoleLogger(const string& name);
	virtual ~ConsoleLogger();

	virtual void trace(const char* pattern, ...);
	virtual void debug(const char* pattern, ...);
	virtual void info(const char* pattern, ...);
	virtual void warn(const char* pattern, ...);
	virtual void error(const char* pattern, ...);
};

}
}
#endif /* CONSOLELOGGER_H_ */
