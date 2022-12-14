#pragma once

namespace Volund
{
	template<int Size, typename T>
	class LuaVec
	{
	public:

		T x = 0;

		T y = 0;

		T z = 0;

		T w = 0;

		LuaVec<Size, T> Cross(const LuaVec<Size, T>& Other);

		LuaVec<Size, T> Normalize();

		LuaVec<Size, T> Reflect(const LuaVec<Size, T>& Normal);

		T Length();

		T Length2();

		int GetSize();
			
		glm::vec<Size, T> GLM() const;

		operator glm::vec<Size, T>() const;

		template<int OtherSize, typename OtherT>
		operator LuaVec<OtherSize, OtherT>() const;

		T& operator[](int Index);
		const T& operator[](int Index) const;

		LuaVec<Size, T> operator+(const LuaVec<Size, T>& Other);
		LuaVec<Size, T> operator+(T Other);

		LuaVec<Size, T> operator-(const LuaVec<Size, T>& Other);
		LuaVec<Size, T> operator-(T Other);

		LuaVec<Size, T> operator*(const LuaVec<Size, T>& Other);
		LuaVec<Size, T> operator*(T Other);

		LuaVec<Size, T> operator/(const LuaVec<Size, T>& Other);
		LuaVec<Size, T> operator/(T Other);

		void operator+=(const LuaVec<Size, T>& Other);
		void operator+=(T Other);

		void operator-=(const LuaVec<Size, T>& Other);
		void operator-=(T Other);

		void operator*=(const LuaVec<Size, T>& Other);
		void operator*=(T Other);

		void operator/=(const LuaVec<Size, T>& Other);
		void operator/=(T Other);

		void operator=(const LuaVec<Size, T>& Other);
		void operator=(T Other);

		LuaVec() = default;

		LuaVec(const LuaVec<Size, T>& Vector);
		LuaVec(const glm::vec<Size, T>& Vector);

		LuaVec(T Scalar1, T Scalar2, T Scalar3, T Scalar4);
		LuaVec(T Scalar1, T Scalar2, T Scalar3);
		LuaVec(T Scalar1, T Scalar2);
		LuaVec(T Scalar);

	private:
	};

	template<>
	inline LuaVec<3, float> LuaVec<3, float>::Cross(const LuaVec<3, float>& Other)
	{
		return LuaVec<3, float>(glm::cross(this->GLM(), Other.GLM()));
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::Cross(const LuaVec<Size, T>& Other)
	{
		return LuaVec<Size, T>(0.0);
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::Normalize()
	{
		T Length = this->Length();
		LuaVec<Size, T> Temp;
		for (int i = 0; i < Size; i++)
		{
			Temp[i] = (*this)[i] / Length;
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::Reflect(const LuaVec<Size, T>& Normal)
	{
		return LuaVec<Size, T>(glm::reflect(this->GLM(), Normal.GLM()));
	}

	template<int Size, typename T>
	inline T LuaVec<Size, T>::Length()
	{
		return sqrt(this->Length2());
	}

	template<int Size, typename T>
	inline T LuaVec<Size, T>::Length2()
	{
		T Length = T(0);
		for (int i = 0; i < Size; i++)
		{
			Length += ((*this)[i] * (*this)[i]);
		}

		return Length;
	}

	template<int Size, typename T>
	inline int LuaVec<Size, T>::GetSize() 
	{
		return Size;
	}

	template<int Size, typename T>
	inline glm::vec<Size, T> LuaVec<Size, T>::GLM() const
	{
		glm::vec<Size, T> Temp;
		for (int i = 0; i < Size; i++)
		{
			Temp[i] = (*this)[i];
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::operator glm::vec<Size, T>() const
	{
		return this->GLM();
	}

	template<int Size, typename T>
	template<int OtherSize, typename OtherT>
	inline LuaVec<Size, T>::operator LuaVec<OtherSize, OtherT>() const
	{
		LuaVec<OtherSize, OtherT> Temp;

		for (int i = 0; i < Size && i < OtherSize; i++)
		{
			Temp[i] += static_cast<OtherT>((*this)[i]);
		}

		return Temp;
	}

	template<int Size, typename T>
	inline T& LuaVec<Size, T>::operator[](int Index)
	{
		switch (Index)
		{
		case 0:
		{
			return x;
		}
		break;
		case 1:
		{
			return y;
		}
		break;
		case 2:
		{
			return z;
		}
		break;
		case 3:
		{
			return w;
		}
		break;
		default:
		{
			return x;
		}
		break;
		}
	}

	template<int Size, typename T>
	inline const T& LuaVec<Size, T>::operator[](int Index) const
	{
		switch (Index)
		{
		case 0:
		{
			return x;
		}
		break;
		case 1:
		{
			return y;
		}
		break;
		case 2:
		{
			return z;
		}
		break;
		case 3:
		{
			return w;
		}
		break;
		default:
		{
			return x;
		}
		break;
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator+(const LuaVec<Size, T>& Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] += Other[i];
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator+(T Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] += Other;
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator-(const LuaVec<Size, T>& Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] -= Other[i];
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator-(T Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] -= Other;
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator*(const LuaVec<Size, T>& Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] *= Other[i];
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator*(T Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] *= Other;
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator/(const LuaVec<Size, T>& Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] /= Other[i];
		}

		return Temp;
	}

	template<int Size, typename T>
	inline LuaVec<Size, T> LuaVec<Size, T>::operator/(T Other)
	{
		LuaVec<Size, T> Temp = (*this);

		for (int i = 0; i < Size; i++)
		{
			Temp[i] /= Other;
		}

		return Temp;
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator+=(const LuaVec<Size, T>& Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] += Other[i];
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator+=(T Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] += Other;
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator-=(const LuaVec<Size, T>& Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] -= Other[i];
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator-=(T Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] -= Other;
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator*=(const LuaVec<Size, T>& Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] *= Other[i];
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator*=(T Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] *= Other;
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator/=(const LuaVec<Size, T>& Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] /= Other[i];
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator/=(T Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] /= Other;
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator=(const LuaVec<Size, T>& Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] = Other[i];
		}
	}

	template<int Size, typename T>
	inline void LuaVec<Size, T>::operator=(T Other)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] = Other;
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::LuaVec(const LuaVec<Size, T>& Vector)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] = Vector[i];
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::LuaVec(const glm::vec<Size, T>& Vector)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] = Vector[i];
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::LuaVec(T Scalar1, T Scalar2, T Scalar3, T Scalar4)
	{
		if (Size >= 1)
		{
			this->x = Scalar1;
		}
		if (Size >= 2)
		{
			this->y = Scalar2;
		}
		if (Size >= 3)
		{
			this->z = Scalar3;
		}
		if (Size >= 4)
		{
			this->w = Scalar4;
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::LuaVec(T Scalar1, T Scalar2, T Scalar3)
	{
		if (Size >= 1)
		{
			this->x = Scalar1;
		}
		if (Size >= 2)
		{
			this->y = Scalar2;
		}
		if (Size >= 3)
		{
			this->z = Scalar3;
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::LuaVec(T Scalar1, T Scalar2)
	{
		if (Size >= 1)
		{
			this->x = Scalar1;
		}
		if (Size >= 2)
		{
			this->y = Scalar2;
		}
	}

	template<int Size, typename T>
	inline LuaVec<Size, T>::LuaVec(T Scalar)
	{
		for (int i = 0; i < Size; i++)
		{
			(*this)[i] = Scalar;
		}
	}

	using LuaVec2 = LuaVec<2, float>;
	using LuaVec3 = LuaVec<3, float>;
	using LuaVec4 = LuaVec<4, float>;
}
