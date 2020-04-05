/*
 * @Author: your name
 * @Date: 2020-04-04 12:32:09
 * @LastEditTime: 2020-04-04 13:47:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\Coding\SakuraAutoCoder\CodeGen\refl.rule.hxx
 */
#pragma once
#include <atomic>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <cassert>

namespace Sakura::refl
{
	// Object is a class similar in nature to std::any, only it does not require
	// C++17 and does not require stored objects to be copyable.

	void NoOp(void*) {}
	class Object final
	{
	public:
		Object();
		template<typename T>
		explicit Object(T&& t);

		Object(Object&& o) noexcept
			: deleter_(NoOp)
		{
			*this = std::move(o);
		}
		Object& operator=(Object&& o) noexcept
		{
			// Release existing resource if any.
			deleter_(data_);

			id_ = o.id_;
			data_ = o.data_;
			deleter_ = move(o.deleter_);
			o.deleter_ = NoOp;
			return *this;
		}
		Object(const Object& o) = delete;
		Object& operator=(const Object& o) = delete;

		~Object()
		{
			deleter_(data_);
		}

		template<typename T>
		bool IsT() const;

		template<typename T>
		const T& GetT() const
		{
			return *static_cast<T*>(data_);
		}

		bool IsVoid() const;
	private:
		size_t id_;
		void* data_ = nullptr;
		std::function<void(void*)> deleter_;
	};

	// Reference is a non-const, type erased wrapper around any object.
	class Reference final
	{
	public:
		template<typename T>
		constexpr Reference(T& t);

		Reference(const Reference& o) = default;
		Reference& operator=(const Reference& o) = default;

		template <typename T>
		bool IsT() const;

		template <typename T>
		T& GetT() const
		{
			return *static_cast<T*>(data_);
		}
		size_t id_;
	private:
		void* data_ = nullptr;
	};

	struct IType
	{
		virtual ~IType() = default;
		virtual const char* GetName() const = 0;
	};

	enum class ETypes : uint8_t
	{
		EFunction,
		EMethod,
		EClass,
		EEnum
	};

	struct Parameter
	{
		std::string Name;
		IType* Param;
		ETypes Type;
	};

    struct IFunction : public IType
    {
		virtual int GetParameterCount() const = 0;
		virtual Parameter GetReturnType() const = 0;
		virtual Parameter GetParameter(int i) const = 0;

		// Syntactic sugar for calling Invoke().
		template <typename... Ts>
		Object operator()(Ts&&... ts);

		virtual Object Invoke(const std::vector<Object>& args) = 0;
    };

	class IMethod : public IType
	{
	public:
		virtual int GetParameterCount() const = 0;
		virtual Parameter GetReturnType() const = 0;
		virtual Parameter GetParameter(int i) const = 0;

		// Syntactic sugar for calling Invoke().
		template <typename... Ts>
		Object operator()(const Reference& o, Ts&&... ts);

		virtual Object Invoke(
			const Reference& o, const std::vector<Object>& args) = 0;
	};

	class IClass : public IType
	{
	public:
		virtual int GetFieldCount() const = 0;
		virtual Reference GetField(
			const Reference& o, const std::string& name) const = 0;

		virtual int GetStaticFieldCount() const = 0;
		virtual Reference GetStaticField(const std::string& name) const = 0;

		virtual int GetMethodCount() const = 0;
		virtual std::vector<std::unique_ptr<IMethod>> GetMethod(
			const std::string& name) const = 0;

		virtual int GetStaticMethodCount() const = 0;
		virtual std::vector<std::unique_ptr<IFunction>> GetStaticMethod(
			const std::string& name) const = 0;
	};

	class IEnum : public IType
	{
	public:
		virtual std::vector<std::string> GetStringValues() const = 0;
		virtual std::vector<int> GetIntValues() const = 0;
		virtual bool TryTranslate(const std::string& value, int& out) = 0;
		virtual bool TryTranslate(int value, std::string& out) = 0;
	};

	template<typename T> struct SClass : IClass {};
	template<typename T> struct SEnum : IEnum {};
	template <typename T, T t> struct Function : IFunction {};
	template <typename T, T t> struct Method : IMethod {};

#if defined(_AMD64_)
    inline constexpr size_t _FNV_offset_basis = 14695981039346656037ULL;
    inline constexpr size_t _FNV_prime = 1099511628211ULL;
#else 
    inline constexpr size_t _FNV_offset_basis = 2166136261U;
    inline constexpr size_t _FNV_prime = 16777619U;
#endif 

	inline constexpr size_t _Fnv1a_append_bytes(size_t _Val, const char* _First,
		const size_t _Count) noexcept { 
        // accumulate range [_First, _First + _Count) into partial FNV-1a hash _Val
		for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
			_Val ^= static_cast<size_t>(_First[_Idx]);
			_Val *= _FNV_prime;
		}
		return _Val;
	}

	int constexpr length(const char* str)
	{
		return *str ? 1 + length(str + 1) : 0;
	}

    using namespace std;
#define GEN_REFL_BASIC_TYPES_TWO_PARAM(T, NAME) \
    template<>\
	struct SClass<const T> : public IClass\
	{\
		inline static const constexpr char name[] = #NAME;\
		inline static const constexpr std::size_t id_ = \
			_Fnv1a_append_bytes(_FNV_offset_basis, name, length(name) * sizeof(char));\
        virtual const char* GetName() const override final{return name;}\
		virtual int GetFieldCount() const override final{return 1;}\
		virtual Reference GetField(const Reference& o,\
			const std::string& name) const override final {return o;}\
		virtual int GetStaticFieldCount() const override final {return 0;}\
		virtual Reference GetStaticField(const std::string& name) const override final\
			{throw std::exception("No static field of basic types.");}\
		virtual int GetMethodCount() const override final {return 0;}\
		virtual std::vector<std::unique_ptr<IMethod>> GetMethod(\
			const std::string& name) const override final\
			{throw std::exception("No static field of basic types.");}\
		virtual int GetStaticMethodCount() const override final {return 0;}\
		virtual std::vector<std::unique_ptr<IFunction>> GetStaticMethod(\
			const std::string& name) const override final\
			{throw std::exception("No static method of basic types.");}\
		inline static const bool isConst() {return false;}\
	};\
	template<>\
	struct SClass<T> : public SClass<const T>{inline static const bool isConst() {return true;}};

#define GEN_REFL_BASIC_TYPES(T) GEN_REFL_BASIC_TYPES_TWO_PARAM(T, T)

    GEN_REFL_BASIC_TYPES(void);
    GEN_REFL_BASIC_TYPES(char);
    GEN_REFL_BASIC_TYPES(uint8_t);
    GEN_REFL_BASIC_TYPES(uint16_t);
    GEN_REFL_BASIC_TYPES(uint32_t);
    GEN_REFL_BASIC_TYPES(uint64_t);
    GEN_REFL_BASIC_TYPES(int8_t);
    GEN_REFL_BASIC_TYPES(int16_t);
    GEN_REFL_BASIC_TYPES(int32_t);
    GEN_REFL_BASIC_TYPES(int64_t);
    GEN_REFL_BASIC_TYPES(float);
    GEN_REFL_BASIC_TYPES(double);
	GEN_REFL_BASIC_TYPES_TWO_PARAM(string, string);
	GEN_REFL_BASIC_TYPES_TWO_PARAM(pmr::string, string);
	GEN_REFL_BASIC_TYPES_TWO_PARAM(std::byte, byte);

	template <typename T>
	auto constexpr GetTypeId()
	{
		return SClass<T>::id_;
	}

    template <typename T>
    Object::Object(T&& t)
    :	id_(GetTypeId<std::decay_t<T>>()),
     	data_(new std::decay_t<T>(std::forward<T>(t)))
    {
        // This is not part of the initializer list because it
        // doesn't compile on VC.
        deleter_ = [](void* data)
        {
            delete static_cast<std::decay_t<T>*>(data);
        };
    }

	Object::Object()
	: id_(SClass<void>().id_)
		, deleter_(NoOp)
	{

	}

	template<typename T>
	bool Object::IsT() const
	{
		return (SClass<T>::id_ == id_);
	}

	template<typename T>
	inline constexpr Reference::Reference(T& t)
		: id_(GetTypeId<std::remove_reference_t<T>>())
		, data_((void*)&t)
	{

	}

	template <typename T>
	bool Reference::IsT() const
	{
		return (SClass<T>::id_ == id_);
	}
}