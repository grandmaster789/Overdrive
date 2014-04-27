#include "logger.h"
#include <fstream>
#include <iostream>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/utility/empty_deleter.hpp>

namespace overdrive {
	namespace core {
		LogHelper::LogHelper(bool echo, const std::string& filename) {
			namespace expr = boost::log::expressions;
			
			auto core = boost::log::core::get();
			auto recordOrdering = boost::log::make_attr_ordering("RecordID", std::less<unsigned int>());
			auto formatter = (
				expr::format("%1% %2%\t%3%")
				% expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S")
				% expr::attr<eLogSeverity>("Severity")
				% expr::smessage
			);
			
			if (echo) {
				auto consoleBackend = boost::make_shared<Backend>();
				mConsoleSink = boost::make_shared<Sink>(consoleBackend, boost::log::keywords::order = recordOrdering);

				boost::shared_ptr<std::ostream> consoleStream(&std::clog, boost::empty_deleter());

				mConsoleSink->locked_backend()->add_stream(consoleStream);			
				mConsoleSink->set_formatter(formatter);

				core->add_sink(mConsoleSink);
			}
	
			auto fileBackend = boost::make_shared<Backend>();
			mFileSink = boost::make_shared<Sink>(fileBackend, boost::log::keywords::order = recordOrdering);

			auto fileStream = boost::make_shared<std::ofstream>(filename.c_str());
			if (!fileStream->good())
				throw std::runtime_error("Failed to open log file");

			fileStream->sync_with_stdio(false); //disable synchronization with printf and the like

			mFileSink->locked_backend()->add_stream(fileStream);
			mFileSink->set_formatter(formatter);

			core->add_sink(mFileSink);
			
			core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
			core->add_global_attribute("RecordID", boost::log::attributes::counter<unsigned int>());
		}

		LogHelper::~LogHelper() {
			if (mConsoleSink) {
				mConsoleSink->stop();
				mConsoleSink->flush();
			}

			if (mFileSink) {
				mFileSink->stop();
				mFileSink->flush();
			}
		}
	}
}
