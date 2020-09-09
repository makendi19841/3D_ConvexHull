#pragma once

#include "Util.h"
#include "..\geom\Point3.h"

#include <fstream>
#include <sstream>
#include <vector>

class TXTReader
{

public:
	static std::vector<Point3> ReadPointXYZ(const std::string path)
	{
		std::vector<Point3> result;
		std::ifstream file(path, std::ios::in);

		std::string fileInput_point;
		std::vector<std::string> coords;

		while (file && !file.eof())
		{
			std::getline(file, fileInput_point);
			if (!fileInput_point.empty())
			{
				coords = util::Util::Split(fileInput_point, " ", false);
				if (coords.size() == 3)
				{
					result.push_back(Point3(atof(coords[0].c_str()), atof(coords[1].c_str()), atof(coords[2].c_str())));
				}
			}
		}

		return result;
	}

};