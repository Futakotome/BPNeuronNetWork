#pragma once
#include<cstdint>

class Image
{

	int threshold;
	uint8_t * pixel;
	int row, column, divideRow, divideColumn;

public:
	int number;
	int width, height;
	uint8_t * dividePixel;
	Image(int row, int column, int threshold = 0);
	Image(int threshold = 0);
	uint8_t * getPixel() const;
	void initImage(int row, int column);
	void setPixel(int x, int y, uint8_t value) const;
	void setNumber(int value);
	void print()const;
	int getRow()const;
	int getColumn()const;
	~Image();





};
