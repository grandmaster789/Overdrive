#ifndef OVERDRIVE_BOOST_H
#define OVERDRIVE_BOOST_H

//include boost-related headers, sorted roughly per subject

#include "platform.h"

//CURRENT BOOST VERSION: 1.55.0
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
//#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/record_ordering.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <boost/asio/io_service.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/thread/future.hpp>

#include <boost/utility/empty_deleter.hpp>

#endif
