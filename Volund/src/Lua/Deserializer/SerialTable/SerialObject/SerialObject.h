#pragma once

#define VOLUND_SERIAL_TYPE_ASSERT(SERIAL_TYPE) static_assert(std::is_same<SERIAL_TYPE, LuaInt>::value || std::is_same<SERIAL_TYPE, LuaFloat>::value || \
std::is_same<SERIAL_TYPE, LuaBool>::value || std::is_same<SERIAL_TYPE, LuaString>::value || \
std::is_same<SERIAL_TYPE, Vec2>::value || std::is_same<SERIAL_TYPE, Vec3>::value || \
std::is_same<SERIAL_TYPE, Vec4>::value || std::is_same<SERIAL_TYPE, SerialTable>::value, "Invalid type for SerialObject!") \

namespace Volund
{
	class SerialTable;

	class PrimitiveSerialObject
	{
	public:

		virtual void operator=(LuaInt data);
		virtual void operator=(LuaFloat data);
		virtual void operator=(LuaBool data);
		virtual void operator=(LuaString data);
		virtual void operator=(Vec2 data);
		virtual void operator=(Vec3 data);
		virtual void operator=(Vec4 data);
		virtual void operator=(SerialTable data);

		template<typename T>
		operator T();

		template<typename T>
		bool Is();

		template<typename T>
		T As();

		virtual uint64_t GetTypeHash() const = 0;

	private:

	};

	template<typename T>
	class SerialObject : public PrimitiveSerialObject
	{
	public:
		
		VOLUND_SERIAL_TYPE_ASSERT(T);

		const T& Get() const;

		T& Get();

		uint64_t GetTypeHash() const override;

		void operator=(T data) override;

		SerialObject(const T& data);

	private:

		T m_Data;
	};

	template<typename T>
	inline bool PrimitiveSerialObject::Is()
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		return this->GetTypeHash() == typeid(T).hash_code();
	}	

	template<typename T>		
	PrimitiveSerialObject::operator T()
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		return this->As<T>();
	}

	template<typename T>
	inline T PrimitiveSerialObject::As()
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		if (!this->Is<T>())
		{
			VOLUND_ERROR("Wrong type requested from PrimitiveSerialObject!");
		}

		return ((SerialObject<T>*)this)->Get();
	}

	template<typename T>
	inline const T& SerialObject<T>::Get() const
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		return this->m_Data;
	}

	template<typename T>
	inline T& SerialObject<T>::Get()
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		return this->m_Data;
	}

	template<typename T>
	inline uint64_t SerialObject<T>::GetTypeHash() const
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		return typeid(T).hash_code();
	}

	template<typename T>
	inline void SerialObject<T>::operator=(T data)
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		this->m_Data = data;
	}

	template<typename T>
	inline SerialObject<T>::SerialObject(const T& data)
	{
		VOLUND_SERIAL_TYPE_ASSERT(T);

		this->m_Data = data;
	}
}

