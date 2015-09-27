#include <string>
#include <map>

namespace border {
	enum BorderElements
	{
		HORIZONTAL, VERTICAL, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, 
		TOP_INTERSECTION, LEFT_INTERSECTION, BOTTOM_INTERSECTION, RIGHT_INTERSECTION, CENTER_INTERSECTION
	};

	struct BorderMapSingle : map<BorderElements, string>
	{
		BorderMapSingle()
		{
			this->operator[](HORIZONTAL) = "─";
			this->operator[](VERTICAL) = "│";
			this->operator[](TOP_LEFT) = "┌";
			this->operator[](TOP_RIGHT) = "┐";
			this->operator[](BOTTOM_LEFT) = "└";
			this->operator[](BOTTOM_RIGHT) = "┘";
			this->operator[](TOP_INTERSECTION) = "┬";
			this->operator[](LEFT_INTERSECTION) = "├";
			this->operator[](BOTTOM_INTERSECTION) = "┴";
			this->operator[](RIGHT_INTERSECTION) = "┤";
			this->operator[](CENTER_INTERSECTION) = "┼";
		};
		~BorderMapSingle(){};
	};

	struct BorderMapDouble : map<BorderElements, string>
	{
		BorderMapDouble()
		{
			this->operator[](HORIZONTAL) = "═";
			this->operator[](VERTICAL) = "║";
			this->operator[](TOP_LEFT) = "╔";
			this->operator[](TOP_RIGHT) = "╗";
			this->operator[](BOTTOM_LEFT) = "╚";
			this->operator[](BOTTOM_RIGHT) = "╝";
			this->operator[](TOP_INTERSECTION) = "╦";
			this->operator[](LEFT_INTERSECTION) = "╠";
			this->operator[](BOTTOM_INTERSECTION) = "╩";
			this->operator[](RIGHT_INTERSECTION) = "╣";
			this->operator[](CENTER_INTERSECTION) = "╬";
		};
		~BorderMapDouble(){};
	};

	/*
	╒══╤══╕ ╓──╥──╖
	│  │  │ ║  ║  ║
	╞══╪══╡ ╟──╫──╢
	│  │  │ ║  ║  ║
	╘══╧══╛ ╙──╨──╜
	*/
	
	static BorderMapSingle borderElementsMapSingle;
	static BorderMapDouble borderElementsMapDouble;
}
