#pragma once


class LightSource
{
public:

	LightSource(Vector4D pos, Vector4D col, float intens)
	{
		position = pos;
		color = col;
		intensity = intens;
	}
	LightSource() 
	{
	}
	~LightSource()
	{

	}
	float getIntensity()
	{
		return intensity;
	}
	Vector4D getPosition()
	{
		return position;
	}
	Vector4D getColor()
	{
		return color;
	}
	void setIntensity(float intens)
	{
		intensity = intens;
	}
	void setColor(Vector4D color)
	{
		this->color = color;
	}
	void setPosition(Vector4D pos)
	{
		position = pos;
	}

private:

	Vector4D position;
	Vector4D color;
	float intensity;

};
