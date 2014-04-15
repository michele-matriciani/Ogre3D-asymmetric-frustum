#include <OgreConfigFile.h>
#include <map>
 
class MyConfig
{
public:
	static MyConfig &getInstance();
 
	//various getters used by class clients to get configuration directives
	int getValueAsInt(std::string key);
	Ogre::Real getValueAsReal(std::string key);
	std::string getValueAsString(std::string key);
	Ogre::Vector3 getValueAsVector3(std::string key);
	bool getValueAsBool(std::string key);
	Ogre::ColourValue getValueAsColourValue(std::string key);
	bool getKeyExists(std::string key);
 
private:
	MyConfig();
	~MyConfig();
	//HALConfig(const MyConfig &); //standard protection for singletons; prevent copy construction
	//HALConfig& operator=(const MyConfig &); //standard protection for singletons; prevent assignment
 
	//this is our config file
	Ogre::ConfigFile m_ConfigFile;
	//this is where our configuration data is stored
	std::map<std::string, std::string> m_Configs;
};