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
