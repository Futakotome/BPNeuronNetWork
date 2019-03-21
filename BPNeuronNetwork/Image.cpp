#include"pch.h"
#include"Image.h"
#include<iostream>
#include<bitset>


Image::Image(int row, int column, int threshold) :threshold(threshold), divideRow(4), divideColumn(4) {
	initImage(row, column);
}
Image::Image(int threshold) : threshold(threshold), row(0), column(0), divideRow(4), divideColumn(4) {

}
Image::~Image() {

}
uint8_t* Image::getPixel()const {
	return pixel;
}
void Image::initImage(int row, int column) {
	this->row = row;
	this->column = column;
	pixel = new uint8_t[row*column];
}
void Image::setPixel(int x, int y, uint8_t value)const {
	//if (value > threshold)
	//	value = 1;
	pixel[x*column + y] = value;
}
void Image::setNumber(int value) {
	this->number = value;
}
void Image::print()const {
	using namespace std;
	cout << endl << number << endl;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			cout << (pixel[i*column + j] == 0 ? " " : "*");
		}
		cout << endl;
	}
}
int Image::getColumn()const {
	return column;
}
int Image::getRow()const {
	return row;
}