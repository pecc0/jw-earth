/* AUTORIGHTS */
/*
 * LoggerFactory.cpp
 *
 *  Created on: Oct 28, 2010
 *      Author: Petko
 */

#include "LoggerFactory.h"

namespace jw
{
namespace log
{
LoggersMap jw::log::LoggerFactory::s_mapLoggers;

IJWLogger * jw::log::LoggerFactory::getLogger(const string& name)
{
	LoggersIterator i = s_mapLoggers.find(name);
	if (i == s_mapLoggers.end()) {
		s_mapLoggers[name] = ConsoleLogger(name);
		i = s_mapLoggers.find(name);
	}
	return &(i->second);
}

}

}
