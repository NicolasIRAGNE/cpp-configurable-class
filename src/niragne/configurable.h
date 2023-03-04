/**
 * @file config.h
 * @author Nicolas IRAGNE (nicolas.iragne@alyce.fr)
 * @brief Provides an interface to configure DeePy.
 * @date 2022-07-14
 *
 * @copyright Copyright niragne (c) 2022
 */

#pragma once

#include <niragne/singleton.h>
#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <filesystem>
#include <map>

namespace niragne::config
{

class ConfigRegistry;

template <typename C>
void registerOptions(ConfigRegistry& registry) = delete;

template <typename C>
void applyConfig(C&, const ConfigRegistry& registry) = delete;

template <typename Class>
concept ConfigurableMember = requires(ConfigRegistry& registry)
{
    { Class::registerOptions(registry) };
};

template <typename Class>
concept ConfigurableFunc = requires(ConfigRegistry& registry)
{
    { niragne::config::registerOptions<Class>(registry) };
};

/**
 * @brief The Configurable concept represents a class that can be configured and can register its options to the program options.
 * Any of the following functions needs to be implemented:
 * - static void C::registerOptions(ConfigRegistry& registry)
 * - static void registerOptions<C>(ConfigRegistry& registry)
 */
template <typename Class>
concept Configurable = ConfigurableMember<Class> || ConfigurableFunc<Class>;

/**
 * @brief The ConfigRegistry class is a singleton that can be used to register and retrieve options.
 * It is designed to be easy to register options and to retrieve them.
 * The general usage is:
    * - Implement a registerOptions function. This can be either a static member function or a specialization of the `registerOptions` template.
    * - This function usually does the following:
        * - Declare a `boost::program_options::options_description`
        * - Add options to the `boost::program_options::options_description`
        * - Call the `ConfigRegistry::registerConfig` function.
    * - In most cases, modify the ConfigRegistry constructor to add call the newly created `registerOptions` function.
    * - Done! All options should be parsed by the time any relevant object is created.
    * - To retrieve an option, one can use the `load(var, name)` function.
 */
class ConfigRegistry : public niragne::utils::Singleton<ConfigRegistry>
{
private:
    std::map<std::string, boost::program_options::options_description> mRegisteredOptions;
    boost::program_options::variables_map mVariablesMap;
    ConfigRegistry() = default;

public:
    std::map<std::string, boost::program_options::options_description>& getRegisteredOptions()
    {
        return mRegisteredOptions;
    };

    /**
     * @brief Add an object's options to the registry.
     * The object must implement the Configurable concept.
     * The actual registration is done by the object itself.
     */
    template <Configurable C>
    void registerConfigurable()
    {
        if constexpr (ConfigurableMember<C>)
        {
            C::registerOptions(*this);
        }
        else if constexpr (ConfigurableFunc<C>)
        {
            niragne::config::registerOptions<C>(*this);
        }
    };

    /**
     * @brief Tries to load a property from the registry.
     * In order to be loaded, the property must have been registered inside registerConfigurable.
     * @param prop The variable to set.
     * @param name The friendly name of the property.
     * @return true if the property was found and loaded, false otherwise.
     */
    template <typename T>
    bool load(T& prop, const std::string& name) const
    {
        try
        {
            prop = mVariablesMap[name].as<T>();
        }
        catch (const boost::program_options::error& e)
        {
            std::cerr << "Could not load property " << name << ": " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Could not load property " << name << std::endl;
        }
        if (mVariablesMap.count(name))
        {
            return true;
        }
        return false;
    };

    template <typename C>
    bool load(C& prop) = delete;

    /**
     * @return True if the requested property is set, false otherwise.
     */
    bool isSet(const std::string& property) const
    {
        return mVariablesMap.count(property);
    };

    /**
     * @brief Store parsed options in the registry. This is called after parsing either from the command line or from a file.
     * @param opt Parsed options returned by `boost::program_options::parse_command_line` or `boost::program_options::parse_config_file.`
     */
    void store(const boost::program_options::basic_parsed_options<char>& opt)
    {
        boost::program_options::store(opt, mVariablesMap);
        boost::program_options::notify(mVariablesMap);
    };

    /**
     * @brief Add a new named collection of options to the registry.
     * Typically, the name is the name of the object that registers the options, but it can be anything.
     * @return true if the collection was successfully registered, false otherwise.
     */
    bool registerConfig(const boost::program_options::options_description& options, const std::string& name)
    {
        if (mRegisteredOptions.contains(name))
        {
            return false;
        }
        mRegisteredOptions.try_emplace(name, options);
        return true;
    };

    void parseCommandLine(int argc, char** argv, const boost::program_options::positional_options_description& positionalOptions )
    {
        boost::program_options::options_description desc("Allowed options");
        for (const auto& [name, options] : mRegisteredOptions)
        {
            desc.add(options);
        }
        auto opt = boost::program_options::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run();
        store(opt);
    };

    void parseConfigFile(const std::filesystem::path& filename)
    {
        std::cout << "Loading config file " << filename << "..." << std::endl;
        boost::program_options::options_description desc("Allowed options");
        for (const auto& [name, options] : mRegisteredOptions)
        {
            desc.add(options);
        }
        try
        {
            boost::program_options::basic_parsed_options<char> opt = boost::program_options::parse_config_file(filename.string().c_str(), desc, true);
            store(opt);
        }
        catch (const boost::program_options::error& e)
        {
            boost::program_options::basic_parsed_options<char> opt(&desc); // weird, but it works.
            store(opt);
            throw;
        }
    };

    bool isRegistered(const std::string& name) const
    {
        return mRegisteredOptions.contains(name);
    };

    // make ctor usable by the singleton
    friend ConfigRegistry& niragne::utils::Singleton<ConfigRegistry>::getInstance();
};

/**
 * @brief This is simply a global wrapper around the ConfigRegistry, because calling `ConfigRegistry::getInstance()` is a bit verbose.
 */
template <Configurable C>
void registerConfigurable()
{
    ConfigRegistry::getInstance().registerConfigurable<C>();
};

inline ConfigRegistry& registry()
{
    return ConfigRegistry::getInstance();
}

inline std::ostream& operator<<(std::ostream& os, ConfigRegistry& registry)
{
    for (const auto& [key, val] : registry.getRegisteredOptions())
    {
        os << val << std::endl;
    }
    return os;
};

void loadConfigFile();


} // namespace niragne::config
