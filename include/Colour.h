#pragma once
#include <SFML/Graphics.hpp>

namespace color
{
    //Not made by me
	sf::Color HSLtoRGB(double H, double S, double V)
	{
		//rad to degrees
		H *= 57.2958;
		double C = S * V;
		double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
		double m = V - C;
		double Rs, Gs, Bs;

		if (H >= 0 && H < 60) {
			Rs = C;
			Gs = X;
			Bs = 0;
		}
		else if (H >= 60 && H < 120) {
			Rs = X;
			Gs = C;
			Bs = 0;
		}
		else if (H >= 120 && H < 180) {
			Rs = 0;
			Gs = C;
			Bs = X;
		}
		else if (H >= 180 && H < 240) {
			Rs = 0;
			Gs = X;
			Bs = C;
		}
		else if (H >= 240 && H < 300) {
			Rs = X;
			Gs = 0;
			Bs = C;
		}
		else {
			Rs = C;
			Gs = 0;
			Bs = X;
		}
		
		return sf::Color(255*(Rs + m), 255*(Gs + m), 255*(Bs + m));
	}
}