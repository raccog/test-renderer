#include "utils.h"

#include <fstream>

namespace utils {

	std::string fileReadString(const std::filesystem::path &filePath) {
		// TODO: check for empty path
		// TODO: check if path is not a file
		
		std::ifstream fileStream(filePath);

		// TODO: check if file has been opened
		
		std::string fileContents((std::istreambuf_iterator<char>(fileStream)),
								 (std::istreambuf_iterator<char>()));

		// TODO: check if file has been read

		return fileContents;
	}

}
