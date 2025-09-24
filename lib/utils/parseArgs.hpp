#pragma once

#include <boost/program_options.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

namespace utils
{
    class Parser
    {
        public:
            explicit Parser(int argc, char** argv) :
                _desc("Options"), _argc(argc), _argv(argv)
            {
                this->_desc.add_options()("help,h", "Display help message")(
                    "debug,d", "Enable debug mode")(
                    "port,p", po::value<uint16_t>(),
                    "Set port to run on")("rooms,r", po::value<uint16_t>(),
                                          "Number of rooms availables");
            };
            ~Parser() = default;

            void parseArgs()
            {
                po::store(po::parse_command_line(this->_argc, this->_argv,
                                                 this->_desc),
                          this->_variables);
                po::notify(this->_variables);
            };

            po::variables_map getArgs() { return this->_variables; }

        private:
            int _argc;
            char** _argv;
            po::options_description _desc;
            po::variables_map _variables;
    };
} // namespace utils
