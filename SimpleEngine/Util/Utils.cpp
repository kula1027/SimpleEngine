#include "Utils.h"
#include <iostream>
#include <sstream>
#include <vector>




/// <summary>
/// 경로 pathFrom에서 시작해서 pathRel로 이동된 경로를 리턴한다.
/// eg. 
/// pathFrom: abc/def/img.png 
/// pathRel: ../qwe/pic.png
/// return: abc/qwe/pic.png
/// </summary>
/// <param name="pathFrom"></param>
/// <param name="pathRel"></param>
/// <returns></returns>
std::string Utils::TravelPath(std::string pathFrom, std::string pathRel) {		
	std::stringstream steramPathFrom(pathFrom);
	std::string strTokPathFrom;
	std::vector<std::string> strVecPathFrom;
	while (std::getline(steramPathFrom, strTokPathFrom, '/')) {
		strVecPathFrom.push_back(strTokPathFrom);
	}	
	if (strVecPathFrom[strVecPathFrom.size() - 1].find('.') != std::string::npos) {
		strVecPathFrom.pop_back();
	}	
	
	std::stringstream steramPathRel(pathRel);
	std::string strTokPathRel;
	std::vector<std::string> strVecPathRel;
	while (std::getline(steramPathRel, strTokPathRel, '/')) {
		strVecPathRel.push_back(strTokPathRel);
	}

	std::vector<std::string> strVecRet = strVecPathFrom;
	for (int loop = 0; loop < strVecPathRel.size(); loop++) {
		if (strVecPathRel[loop].compare("..") == 0) {
			strVecRet.pop_back();
		} else {
			strVecRet.push_back(strVecPathRel[loop]);
		}
	}

	std::string retPath;
	for (int loop = 0; loop < strVecRet.size(); loop++) {
		if (loop != 0) { retPath += "/"; }
		retPath += strVecRet[loop];
	}

	return retPath;
}
