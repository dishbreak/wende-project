#pragma once



class C3Point
{
	public:
		union
		{
			double X;
			double Azimuth;
		};
		union
		{
			double Y;
			double Elevation;
		};		

	public:
		C3Point(void);
		C3Point(double x, double y);
		~C3Point(void);

};
