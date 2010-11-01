/*
 * IJWLogger.h
 *
 *  Created on: Oct 28, 2010
 *      Author: Petko
 */

#ifndef IJWLOGGER_H_
#define IJWLOGGER_H_

namespace jw
{
namespace log
{
class IJWLogger
{
public:
	virtual void trace(const char* pattern, ...) = 0;
	virtual void debug(const char* pattern, ...) = 0;
	virtual void info(const char* pattern, ...) = 0;
	virtual void warn(const char* pattern, ...) = 0;
	virtual void error(const char* pattern, ...) = 0;
};

}
}



#endif /* IJWLOGGER_H_ */
