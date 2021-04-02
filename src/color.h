#pragma once

struct Color
{
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float data[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	};

	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
	Color(float r, float g, float b, float a = 1.0f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	bool operator==(const Color &c) const { return (r == c.r && g == c.g && b == c.b && a == c.a); }
	bool operator!=(const Color &c) const { return (r != c.r || g != c.g || b != c.b || a != c.a); }

	float& operator[](int idx) { return data[idx]; }
	const float& operator[](int idx) const { return data[idx]; }

	bool operator<(const Color& c) const;
	
	Color operator+(const Color& c) const
	{
		Color r = *this;
		r += c;
		return r;
	}
	void operator+=(const Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
	}

	Color operator-() const { return {1.0f - r, 1.0f - g, 1.0f - b, 1.0f - a}; }
	Color operator-(const Color &c) const
	{
		Color r = *this;
		r -= c;
		return r;
	}
	void operator-=(const Color &c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;
	}

	Color operator*(const Color &c) const { return { r * c.r, g * c.g, b * c.b, a * c.a }; }
	Color operator*(const float &v) const { return { r * v, g * v, b * v, a * v }; }
	void operator*=(const Color &c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		a *= c.a;
	}
	void operator*=(const float &v)
	{
		r *= v;
		g *= v;
		b *= v;
		a *= v;
	}

	Color operator/(const Color &c) const { return { r / c.r, g / c.g, b / c.b, a / c.a }; }
	Color operator/(const float &v) const { return { r / v, g / v, b / v, a / v }; }
	void operator/=(const Color &c)
	{
		r /= c.r;
		g /= c.g;
		b /= c.b;
		a /= c.a;
	}
	void operator/=(const float &v)
	{
		if (v == 0)
		{
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
			a = 1.0f;
		}
		else
		{
			r /= v;
			g /= v;
			b /= v;
			a /= v;
		}
	}

};

inline bool Color::operator<(const Color& c) const
{
	if (r == c.r)
	{
		if (g == c.g)
		{
			if (b == c.b)
			{
				return (a < c.a);
			}
			else
			{
				return (b < c.b);
			}
		}
		else
		{
			return g < c.g;
		}
	}
	else
	{
		return r < c.r;
	}
}
