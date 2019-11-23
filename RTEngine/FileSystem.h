#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_

#include "Resource.h"
#include <string>
#include <vector>

#define LIBRARY_FOLDER "Library\\"
#define MESHES_FOLDER "Library\\Meshes\\"
#define TEXTURES_FOLDER "Library\\Textures\\"



#define PREFABS_FOLDER "Library\\Prefabs\\"
#define SCENES_FOLDER "Library\\Scenes"

#define SETTINGS_FOLDER "Settings\\"
#define USER_PREFABS_FOLDER "Assets\\Prefabs\\"
#define USER_SCENES_FOLDER "Assets\\Scenes\\"
#define ASSETS_FOLDER "Assets\\"

enum lib_dir {
	LIBRARY_MESHES,
	LIBRARY_TEXTURES,
	LIBRARY_PREFABS,
	LIBRARY_SCENES,
	SETTINGS,
	ASSETS,
	ASSETS_SCENES,
	ASSETS_PREFABS,
	NO_LIB
};

class FileSystem {
public:
	FileSystem() {};
	~FileSystem() {};

public:
	static bool ExportBuffer(char* data,int size, const char* file_name, lib_dir lib = NO_LIB, const char* extension = ""); // get buffer from a file
	static char* ImportFile(const char* file_name); // recieve buffer from a file

	static std::string GetFileString(const char* file_name);
	static void SetFileString(const char* file_name, const char* file_content);

	static void CreateEmptyFile(const char* file_name, lib_dir lib = NO_LIB, const char* extension = "");
	static void DestroyFile(const char* file_name, lib_dir lib = NO_LIB, const char* extension = "");
	static bool ExistisFile(const char* file_name, lib_dir lib = NO_LIB, const char* extension = "");
	static bool copyFileTo(const char* full_path_src, lib_dir dest_lib = NO_LIB, const char* extension = "", std::string file_new_name = "");
	static int getFileLastTimeMod(const char* file_name, lib_dir lib = NO_LIB, const char* extension = "");
	static bool FindInDirectory(const char* directory, const char* file_name, std::string& final_path); // File name with extension
	static std::string getPathFromLibDir(lib_dir r_type);

	static void DiscoverFiles(std::string path, std::vector<std::string>* file_array, std::vector<std::string>* directory_array);


	static void getFileNameFromPath(std::string& str);
	static bool getPath(std::string & str);
	static void getExtension(std::string& str);
	static void FormFullPath(std::string& path, const char* file_name, lib_dir lib, const char* extension);
	static void SplitFilePath(std::string& full_path, std::string* path = nullptr, std::string*  name = nullptr, std::string* extension = nullptr);
	static bool removeExtension(std::string& str);
	static bool removePath(std::string& str);






};

#endif

