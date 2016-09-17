#include "../../../include/mcbt/dict/MCBT_FIO_FileIO.h"

const std::exception BTFile::EX_OBJECT_NO_INIT("this object wasn't initilized correctly or got deleted");
const std::exception BTFile::EX_INVALID_PATH("invalid file path");
const std::exception BTFile::EX_INVALID_FORMAT("content does not match expected format");

//validate()
const std::regex BTFile::re_basic_structure("^(\\s*(([\\t\\ ]*\\[[\\t\\ ]*#[0-9]+[\\t\\ ]*([A-Za-z-_]+)?[\\t\\ ]*\\])([^\\[\\-\\]])*(\\[[\\t\\ ]*-[\\t\\ ]*\\]\\s*)))*$"); //expected format for content
const std::regex BTFile::re_structure_fragment("\\n*([\\t\\n\\r\\s]*((\\[#[0-9]+\\s[A-Za-z-_]*\\]){1}([^\\[-\\]])*(\\[-\\]){1})+)*"); //fragment of valid string in content
//---

//standardise()											//format itemheader
const std::regex BTFile::re_standardise1("\\s*\\[[\\t\\ ]*#([0-9]+)([\\t\\ ]+([A-Za-z-_]+))?[\\t\\ ]*\\]\\s*\\n(([\\t\\ ]*[A-Z-a-z0-9#_-]*?[\\t\\ ]*\\n)+?)[\\s]*([\\t\\ ]*\\[[\\t\\ ]*-[\\t\\ ]*\\][\\t\\ ]*(\\n|$))");
const std::regex BTFile::re_standardise2("^[\\t\\ ]*\\n");					//remove empty lines
const std::regex BTFile::re_standardise3("^([\\ \\s]+)");					//indent itemid/aliases
const std::regex BTFile::re_standardise4("^\\s*(\\[.*)\\n([\\t\\ ]*[A-Za-z-_]*)([^\\]]*\\])");	//add empty line after itemid
//---

bool BTFile::standardise(void)
{
	return BTFile::standardise(this->sContent);
}
bool BTFile::standardise(std::string &sContent)
{
	if (BTFile::validateContent(sContent) == false)
		return false;

	sContent = std::regex_replace(sContent, re_standardise1, "[#$1 $3]\n$4$6");
	sContent = std::regex_replace(sContent, re_standardise2, "");
	sContent = std::regex_replace(sContent, re_standardise3, "\t");
	sContent = std::regex_replace(sContent, re_standardise4, "$1\n$2\n$3");

	return true;
}

bool BTFile::validateContent(void) const
{
	return BTFile::validateContent(this->sContent);
}
bool BTFile::validateContent(const std::string &sContent)
{
	if (std::regex_match(sContent, BTFile::re_basic_structure) != true || sContent == "")
		return false;
	return true;
}

void BTFile::readFile(const bool validate)
{
	BTFile::readFile(this->sFilePath, this->sContent, validate);
}
void BTFile::readFile(const std::string sFilePath, std::string &sContent, const bool validate)
{
	if (sFilePath == "" || std::ifstream(sFilePath).bad() == true)
		throw BTFile::EX_INVALID_PATH;

	try
	{
		std::ifstream file(sFilePath);
		sContent = "";
		sContent.assign((std::istreambuf_iterator<char>(file)),
			(std::istreambuf_iterator<char>()));
	}
	catch (const std::exception&)
	{
		throw std::exception("error reading file");
	}

	if (validate == true)
		if (BTFile::validateContent(sContent) == false)
		{
			sContent = "";
			throw BTFile::EX_INVALID_FORMAT;
		}
}

void BTFile::writeFile(const bool validate) const
{
	BTFile::writeFile(this->sFilePath, this->sContent, validate);
}
void BTFile::writeFile(const std::string sFilePath, const bool validate) const
{
	BTFile::writeFile(sFilePath, this->sContent, validate);
}
void BTFile::writeFile(const std::string sFilePath, const std::string &sContent, const bool validate)
{
	if (sFilePath == "")
		throw BTFile::EX_INVALID_PATH;
	if (validate == true)
		if (BTFile::validateContent(sContent) == false)
			throw BTFile::EX_INVALID_FORMAT;

	try
	{
		std::ofstream file(sFilePath);
		file << sContent;
		file.close();
	}
	catch (const std::exception&)
	{
		throw std::exception("error writing file");
	}
}

void BTFile::deleteFile(void)
{
	BTFile::deleteFile(this->sFilePath);
	this->sFilePath = "";
	this->sContent = "";
}
void BTFile::deleteFile(const std::string sFilePath)
{
	std::remove(sFilePath.c_str());
}

void BTFile::getContent(std::string &sContent, const bool validate) const
{
	if (this->sFilePath == "")
		throw BTFile::EX_OBJECT_NO_INIT;
	if (BTFile::validateContent(this->sContent) == false)
	{
		sContent = "";
		throw BTFile::EX_INVALID_FORMAT;
	}

	sContent = this->sContent;
}
void BTFile::setContent(const std::string &sContent, const bool validate)
{
	if (this->sFilePath == "")
		throw BTFile::EX_OBJECT_NO_INIT;
	if (BTFile::validateContent(sContent) == false)
		throw BTFile::EX_INVALID_FORMAT;

	this->sContent = sContent;
}
