#include "geom\Point3.h"
#include "io\TXTReader.h"
#include "io\TXTWriter.h"
#include "math\Intersection.h"

#include <iostream>

Point3 Min_XYZ(const std::vector<Point3> &points)
{
	Point3 result = Point3(std::numeric_limits<double>::max(),
						   std::numeric_limits<double>::max(),
						   std::numeric_limits<double>::max());

	for (const Point3 &p : points)
	{
		if (p.x < result.x)
			result.x = p.x;
		if (p.y < result.y)
			result.y = p.y;
		if (p.z < result.z)
			result.z = p.z;
	}

	return result;
}


Point3 MaxXYZ(const std::vector<Point3> &points) {

	Point3 result(-std::numeric_limits<double>::max(),
		          -std::numeric_limits<double>::max(),
		         -std::numeric_limits<double>::max());

	for (const Point3 &p : points) {
		if (p.x > result.x)
			result.x = p.x;
		if (p.y > result.y)
			result.y = p.y;
		if (p.z > result.z)
			result.z = p.z;
	}

	return result;
}


void DetermineNeighborhood(std::vector<Point3> &points, const double radius2D)
{
	// ...
	double distance = 0;

	for (Point3 &p1 : points) {

		for (Point3 &p2 : points) {

			distance  = pow((p1.x - p2.x), 2);
			distance += pow((p1.y - p2.y), 2);
			distance += pow((p1.z - p2.z), 2);
			distance = sqrt(distance);

			if (p1 != p2 && distance <= radius2D) {
				p1.points_neighbor.push_back(&p2);

			}
		}
	}

}


 Point3 MinXPoint(const std::vector<Point3> &points)
 {
 	//...find the point with the smallest x-value in the cloud	
	
	 Point3 pts = Point3(std::numeric_limits<double>::max(),
		                 std::numeric_limits<double>::max(),
		                 std::numeric_limits<double>::max());

	 for (size_t i = 0; i < points.size(); i++) {

		 if (pts.x > points[i].x) {

			 //pts.x = points[i].x;
			 //pts.y = points[i].y;
			 //pts.z = points[i].z;
			 pts = points[i];

		 }
	}

	return pts;
 }

bool Intersection(const Point3 &p1, const Point3 &p2, const std::vector<Point3> &outline_current)
{
	for (unsigned int i = 0; i < outline_current.size() - 1; i++)
	{
		if (Intersection::InnerIntersection2DLineSegments(p1, p2, outline_current[i], outline_current[i+1]) ||
			(i != 0 && p2 == outline_current[i]))
		{
			return true;
		}
	}

	return false;
}

// Task 3 + 4
 //Point3 DetermineNextPoint(const Point3 &point_current, const Point3 &point_previous, const std::vector<Point3> &outline_current)
 //{
	// Point3 point;


	// // define first vector between virtual point and current point
	// Vector3 initial_vector;
	// initial_vector.x = point_previous.x - point_current.x;
	// initial_vector.y = point_previous.y - point_current.y;
	// initial_vector.z = point_previous.z - point_current.z;

	// // find the most likely second vector 
	// double max_angle = -std::numeric_limits<double>::max();

	////if (!Intersection(point_current, point_previous, outline_current)) {

	//	 for (Point3 *p : point_current.points_neighbor) {      // iterate among neighbouring points of the current point

	//			 // define vector between current point and its neighbours: (*p - point_current); 
	//			 // define angle between initial_vector and vector between current point and its neighbours
	//		 double angle = Angle2DDegree360(initial_vector, *p - point_current);

	//		 if (angle > max_angle) { // check neighbouring point with the min angle between current point and its neighbours
	//			 max_angle = angle;
	//			 point = p;
	//		 }
	//	 }

	//	 return point;
	//// }
 //}

Point3 DetermineNextPoint(const Point3 &point_current, const Point3 &point_previous, const std::vector<Point3> &outline_current)
{
	Point3 pts;

    // define first vector between virtual point and current point
	 Vector3 initial_vector;
	initial_vector.x = point_previous.x - point_current.x;
	initial_vector.y = point_previous.y - point_current.y;
	initial_vector.z = point_previous.z - point_current.z;

	// initialization of the maximum angle 
	double max_angle = -std::numeric_limits<double>::max();

	if (!Intersection(point_current, point_previous, outline_current)) {

		for (Point3 *p : point_current.points_neighbor) {      // iterate among neighbouring points of the current point

		   // define vector between current point and its neighbours: (*p - point_current); 
			Vector3 pts_neighbour = *p - point_current;

		    // define angle between initial_vector and vector between current point and its neighbours
			double angle = Angle2DDegree360(initial_vector, pts_neighbour);

			if (angle > max_angle) { // check neighbouring point with the min angle between current point and its neighbours
				max_angle = angle;
				pts = p;
			}
		}

		return pts;
	}
}


int main()
{
	std::cout << std::fixed;
	std::vector<Point3> points_building = TXTReader::ReadPointXYZ("data\\input\\building.xyz");

	const Point3 OFFSET = Min_XYZ(points_building);
	for (Point3 &p : points_building)
	{
		p.x -= OFFSET.x;
		p.y -= OFFSET.y;
	}


	
	// Task1
	const double radius2D = 1;
	DetermineNeighborhood(points_building, radius2D);
	std::cout << points_building[41].points_neighbor.size() << std::endl;

	// ... define current point and starting points
	Point3 point_current = MinXPoint(points_building);
	Point3 starting_point = point_current;
	
	//std::cout << "A1" << std::endl << std::endl;
	
	// ... define previous point
	Point3 point_previous;
	point_previous.x = point_current.x;
	point_previous.y = point_current.y + 1;
	point_previous.z = point_current.z;                       // could be what ever for we just care about x and y

	// ... initialize next point and result
	Point3 point_next;
	std::vector<Point3> result;

	// Task3
	 do
	 {
		// ... find next point
	 	point_next = DetermineNextPoint(point_current, point_previous, result);

		// ... update point position
        point_previous = point_current;
		point_current  = point_next;

		// ... update vector of contour points with new found points
		result.push_back(point_next);
	 
		// ... check stopping condition
	 } while (point_next != starting_point);
	

	for (Point3 &p : result)
	{
		p.x += OFFSET.x;
		p.y += OFFSET.y;
	}

	
	
	TXTWriter::WritePointXYZ("data\\output\\result.xyz", result);

	std::system("PAUSE");
	return 0;
}