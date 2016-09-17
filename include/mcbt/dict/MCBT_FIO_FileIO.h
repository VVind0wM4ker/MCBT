#pragma once

#include <string>
#include <fstream>
#include <regex>

class BTFile
{
public:
	BTFile(const std::string sFilePath)
		: sFilePath(sFilePath)
	{}
	~BTFile()
	{}
			bool	validateContent(void) const;
	static	bool	validateContent(const std::string &sContent);

			bool	standardise(void);
	static	bool	standardise(std::string &sContent);

			void	readFile(const bool validate = true);
	static	void	readFile(const std::string sFilePath, std::string &sContent, const bool validate = true);

			void	writeFile(const bool validate = true) const;
			void	writeFile(const std::string sFilePath, const bool validate = true) const;
	static	void	writeFile(const std::string sFilePath, const std::string &sContent, const bool validate = true);

			void	deleteFile(void);
	static	void	deleteFile(const std::string sFilePath);

			void	getContent(std::string &sContent, const bool validate = true) const;
			void	setContent(const std::string &sContent, const bool validate = true);

private:
	BTFile();

					std::string sContent;
					std::string sFilePath;

	static	const	std::regex re_basic_structure;
	static	const	std::regex re_structure_fragment;
	static	const	std::regex re_standardise1;
	static	const	std::regex re_standardise2;
	static	const	std::regex re_standardise3;
	static	const	std::regex re_standardise4;

	static	const	std::exception EX_OBJECT_NO_INIT;
	static	const	std::exception EX_INVALID_PATH;
	static	const	std::exception EX_INVALID_FORMAT;
};
