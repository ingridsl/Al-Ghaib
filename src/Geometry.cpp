#include <math.h>

#include "Geometry.hpp"

// VEC2 //
Vec2::Vec2(){
	x = y = 0;
}

Vec2::Vec2(float x1, float y1){
	x = x1;
	y = y1;
}

void Vec2::operator=(const Vec2& v){
	x = v.x;
	y = v.y;
}

void Vec2::operator+=(const Vec2& v){
	x += v.x;
	y += v.y;
}

void Vec2::operator-=(const Vec2& v){
	x -= v.x;
	y -= v.y;
}

void Vec2::operator*=(const float&  p){
	x *= p;
	y *= p;
}

void Vec2::operator/=(const float&  p){
	x /= p;
	y /= p;
}

Vec2 Vec2::operator+(const Vec2& rhs) const {
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float rhs) const {
    return Vec2(x * rhs, y * rhs);
}

Vec2 Vec2::operator/(const float rhs) const {
    return Vec2(x / rhs, y / rhs);
}

Vec2 Vec2::Soma(Vec2 vector){
	return Vec2((x + vector.x), (y + vector.y));
}

Vec2 Vec2::Sub(Vec2 vector){
	return Vec2((x - vector.x), (y - vector.y));
}

Vec2 Vec2::Mult(int escalar){
	return Vec2(x*escalar, y*escalar);
}

Vec2 Vec2::Rotate(float angle){
	return Vec2((x*cos(angle) - y*sin(angle)), (y*cos(angle) + x*sin(angle)));
}

Vec2 Vec2::Normalize(){
	return Vec2((x/Abs()), (y/Abs()));
}

float Vec2::Abs(){
	return (float) sqrt((pow(x,2)+pow(y,2)));
}

float Vec2::Angle(){
	return atan2f(y,x);
}

float Vec2::Distance(Vec2 vector){
	return Sub(vector).Abs();
}

float Vec2::Inclination(Vec2 vector){
	return Sub(vector).Angle();
}

// RECT //
Rect::Rect(){
	x = y = w = h = 0;
}

Rect::Rect(float x1, float y1, float w1, float h1){
	x = x1;
	y = y1;
	w = w1;
	h = h1;
}

void Rect::SomaVec2(Vec2 vector){
	x = x+ vector.x;
	y = y+ vector.y;
}

float Rect::CenterX(){
	return (w/2)+x;
}

float Rect::CenterY(){
	return (h/2)+y;
}

Vec2 Rect::Center(){
	return Vec2(CenterX(), CenterY());

}

float Rect::DistanceRect(Rect rectangle){
	return Center().Distance(rectangle.Center());
}

bool Rect::Inside(float x1, float y1){
	return (x1 >= x && x1 <= x+w && y1 >= y && y1 <= y+h);
}

bool Rect::Inside(Rect v2){
	return (v2.x >= x && v2.x + v2.w <= x+w
			&& v2.y >= y && v2.y + v2.h <= y+h);
}

bool Rect::InsideX(Rect v2){
	return (v2.x >= x && v2.x + v2.w <= x+w);
}

bool Rect::InsideY(Rect v2){
	return (v2.y >= y && v2.y + v2.h <= y+h);
}

bool Rect::Intersect(Rect v2){
	if((v2.x + v2.w >= x && v2.x + v2.w <= x+w)||
			(v2.x >= x && v2.x <= x+w)){
		if(v2.y + v2.h >= y && v2.y + v2.h <= y+h) return true;
		if(v2.y >= y && v2.y <= y+h) return true;
	}
	return false;
}

bool Rect::Collide(Rect v2){
	if(((x + w > v2.x && x < v2.x) ||
			(x < v2.x + v2.w && x + w > v2.x)) &&
				((y + h > v2.y && y < v2.y) ||
						(y < v2.y + v2.h && y + h > v2.y))){
		return true;
	}
	return false;
}

void Rect::operator=(const Rect& r){
	x = r.x;
	y = r.y;
	w = r.w;
	h = r.h;
}
