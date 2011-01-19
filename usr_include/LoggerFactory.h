/* AUTORIGHTS */
/*
 * LoggerFactory.h
 *
 *  Created on: Oct 28, 2010
 *      Author: Petko
 */

#ifndef LOGGERFACTORY_H_
#define LOGGERFACTORY_H_

#include <map>

#include "ConsoleLogger.h"

namespace jw
{
namespace log
{

typedef std::map<string, ConsoleLogger> LoggersMap;
typedef std::map<string, ConsoleLogger>::iterator LoggersIterator;

class LoggerFactory
{
	static LoggersMap s_mapLoggers;
public:

	static IJWLogger* getLogger(const string& name);
};
}
}



#endif /* LOGGERFACTORY_H_ */
