#include <iostream>
#include "test.h"

namespace niragne::config
{
template <>
bool ConfigRegistry::load<niragne::test::Printer>(niragne::test::Printer& printer)
{
    load(printer.m_data, "printer.text");
    load(printer.m_padding, "printer.padding");
    return true;
}

template <>
void registerOptions<niragne::test::Printer>(ConfigRegistry& registry)
{
    boost::program_options::options_description desc("Printer options");
    // clang-format off
    desc.add_options()
        ("printer.text", boost::program_options::value<std::string>()->default_value("UNINITIALIZED"), "text to print")
        ("printer.padding", boost::program_options::value<int>()->default_value(0), "padding to add before printing")
        ;
    // clang-format on
    registry.registerConfig(desc, "Printer");
}
} // namespace niragne::config

int main(int argc, char** argv)
{
    auto& registry = niragne::config::registry();
    registry.registerConfigurable<niragne::test::Printer>();
    std::cout << "Registered options:" << std::endl;
    std::cout << registry << std::endl;
    registry.parseCommandLine(argc, argv, boost::program_options::positional_options_description());
    registry.parseConfigFile("../test.cfg");
    niragne::test::Printer printer;
    niragne::config::registry().load(printer);
    printer.print();
}