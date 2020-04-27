/*
 * @Author: your name
 * @Date: 2020-04-04 12:32:09
 * @LastEditTime: 2020-04-27 12:25:46
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
#include <map>
#include <vector>
#include <cassert>


using namespace std;
namespace std::pmr
{
	struct string;
}

namespace Sakura::refl
{
	class Object;
	class Reference;
	class IType;
	namespace detail 
	{
		template<typename T> struct declval_helper { static T value; };

		template<typename T, typename Z, Z T::*MPtr>
		struct offset_helper 
		{
			using TV = declval_helper<T>;
			char for_sizeof[
				(char *)&(TV::value.*MPtr) -
				(char *)&TV::value
			];
		};

		template<typename T, std::size_t N>
		constexpr std::size_t arraySize(T(&)[N]) noexcept
		{
			return N;
		}

		constexpr std::size_t arraySize(std::nullptr_t nul) noexcept
		{
			return 0;
		}

		template <typename F, typename T, typename... Args, std::size_t N, std::size_t... Idx>
		constexpr decltype(auto) _arr_call_impl(F f, T(&t)[N], std::index_sequence<Idx...>,
			Args&&... args) 
		{
			return f(args..., t[Idx]...);
		}
		template <typename F, typename T, typename... Args, std::size_t N>
		constexpr decltype(auto) arr_unfold_call(F f, T (&t)[N], Args&&... args) 
		{
			return _arr_call_impl(f, t, std::make_index_sequence<N>{}, args...);   
		}

		template<typename F, typename T, std::size_t N, std::size_t... Idx>
		constexpr bool matching_impl(const std::string_view tag, F&& func,
			T(&t)[N], std::index_sequence<Idx...>)
		{
			return (func(tag, t[Idx].title)|...);
		}
		template <typename F, typename T, std::size_t N>
		constexpr decltype(auto) matching(const std::string_view tag, 
			F&& f, T (&t)[N]) 
		{
			return matching_impl(tag, f, t, std::make_index_sequence<N>{});   
		}
		template <typename F>
		constexpr decltype(auto) matching(const std::string_view tag, 
			F&& f, void* npt) 
		{
			return false;   
		}

		template<typename T>
		constexpr bool has_tag(const std::string_view tag, T&& src)
		{
			return src == tag;
		}

		template<typename T>
		constexpr bool has_tag_series(const std::string_view tag, T&& src)
		{
			return src.starts_with(tag);
		}
	}

	template<typename T, typename Z, Z T::*MPtr>
	constexpr int offset_of() 
	{
		return sizeof(detail::offset_helper<T, Z, MPtr>::for_sizeof);
	}

}

#define has_member(s)\
template<typename ____TTTT>\
struct has_member_##s{\
    template <typename T_T>static auto check(T_T)->typename std::decay<decltype(T_T::s)>::type;\
    static void check(...);\
    using type=decltype(check(std::declval<____TTTT>()));\
    enum{value=!std::is_void<type>::value};\
};

#define SFIELD_INFO(NAME, MOTHER_BOARD, array, ...) \
	static struct NAME##_info{\
		constexpr NAME##_info() = default;\
		constexpr static const FieldWithMeta<Sakura::refl::detail::arraySize(array)> fd =\
		FieldWithMeta<Sakura::refl::detail::arraySize(array)>(#NAME,\
			Sakura::refl::decay_type_name<decltype(std::declval<MOTHER_BOARD>().NAME)>(),\
			offsetof(MOTHER_BOARD, NAME), array);\
		template<typename F>\
		constexpr static const bool matching(const std::string_view tag, F&& func)\
		{return Sakura::refl::detail::matching(tag, func, array);}\
		decltype(&MOTHER_BOARD::NAME) ptr = &MOTHER_BOARD::NAME;\
	};

#define SMETHOD_INFO(NAME, MOTHER_BOARD, array, ...) \
	struct NAME##_info{\
		constexpr NAME##_info() = default;\
		constexpr static const FieldWithMeta<Sakura::refl::detail::arraySize(array)> fd = \
		FieldWithMeta<Sakura::refl::detail::arraySize(array)>(#NAME,\
		Sakura::refl::decay_type_name<decltype(&MOTHER_BOARD::NAME)>(),\
		0, array);\
		template<typename F>\
		constexpr static const bool matching(const std::string_view tag, F&& func)\
		{return Sakura::refl::detail::matching(tag, func, array);}\
		decltype(&MOTHER_BOARD::NAME) ptr = &MOTHER_BOARD::NAME;\
	};

#define SSTATICFIELD_INFO(NAME, MOTHER_BOARD, array, ...)  \
	struct NAME##_info{\
		constexpr NAME##_info() = default;\
		constexpr static const FieldWithMeta<Sakura::refl::detail::arraySize(array)> fd = \
		FieldWithMeta<Sakura::refl::detail::arraySize(array)>(#NAME,\
		Sakura::refl::decay_type_name<decltype(&MOTHER_BOARD::NAME)>(),\
		0, array);\
		template<typename F>\
		constexpr static const bool matching(const std::string_view tag, F&& func)\
		{return Sakura::refl::detail::matching(tag, func, array);}\
		decltype(&MOTHER_BOARD::NAME) ptr = &MOTHER_BOARD::NAME;\
	};

#define SENUM_FIELD_INFO(NAME, MOTHER_BOARD, array, ...) \
	struct NAME##_info{\
		constexpr NAME##_info() = default;\
		constexpr static const FieldWithMeta<Sakura::refl::detail::arraySize(array)> fd = \
		FieldWithMeta<Sakura::refl::detail::arraySize(array)>(#NAME,\
		Sakura::refl::decay_type_name<MOTHER_BOARD>(),\
		0, array);\
		template<typename F>\
		constexpr static const bool matching(const std::string_view tag, F&& func)\
		{return Sakura::refl::detail::matching(tag, func, array);}\
		MOTHER_BOARD val = MOTHER_BOARD::NAME;\
	};

namespace Sakura::refl
{
	template <typename T>
	constexpr std::string_view type_name() 
	{
		#if defined(__clang__)
		constexpr std::string_view prefix =
			"std::string_view Sakura::refl::type_str() [T =  ";
		constexpr std::string_view suffix = "]";
		#elif defined(_MSC_VER)
		constexpr std::string_view prefix =
			"class std::basic_string_view<char,struct std::char_traits<char> > "
			"__cdecl Sakura::refl::type_str<";
		constexpr std::string_view suffix = ">(void)";
		#else
		constexpr std::string_view prefix =
			"constexpr std::string_view Sakura::refl::type_str() [with T = ";
		constexpr std::string_view suffix =
			"; std::string_view = std::basic_string_view<char>]";
		#endif
		auto sig = std::string_view{__PRETTY_FUNCTION__};
		sig.remove_prefix(prefix.size());
		sig.remove_suffix(suffix.size());
		return sig;
	}

	template<typename T>
	constexpr std::string_view decay_type_name()
	{
		return type_name<std::decay_t<T>>();
	}
	// Object is a class similar in nature to std::any, only it does not require
	// C++17 and does not require stored objects to be copyable.

	inline void NoOp(void*) {}

	namespace detail
	{
		template <typename Tuple, typename F, std::size_t ...Indices>
		constexpr void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
			using swallow = int[];
			(void)swallow{1,
				(f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
			};
		}

		template <typename Tuple, typename F>
		constexpr void for_each(Tuple&& tuple, F&& f) {
			constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
			for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
						std::make_index_sequence<N>{});
		}
		
		template <typename Fn, typename Tuple>
		inline constexpr void ForEachTuple(Tuple&& tuple, Fn&& fn)
		{
			for_each(std::forward<Tuple>(tuple), fn);
		}

		template<class ...Fs> struct overload_set;
		template<class F1, class... Fs>
		struct overload_set<F1, Fs...> : F1, overload_set<Fs...>::type {
			using type = overload_set;

			overload_set(F1 head, Fs... tail)
				: F1(head), overload_set<Fs...>::type(tail...)
			{ }

			using F1::operator();
			using overload_set<Fs...>::type::operator();
		};

		template<class F>
		struct overload_set<F> : F {
			using type = F;
			using F::operator();
		};

		template<class... Fs>
		struct overload_visitor : overload_set<Fs...>::type {
			using type = overload_visitor;
			using overload_set<Fs...>::type::operator();

			overload_visitor(Fs... funcs)
				: overload_set<Fs...>::type(funcs...) {
			}
		};

		template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
		template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

		int constexpr length(const char* str)
		{
			return *str ? 1 + length(str + 1) : 0;
		}

#if true
		inline constexpr static const size_t _FNV_offset_basis = 14695981039346656037ULL;
		inline constexpr static const size_t _FNV_prime = 1099511628211ULL;
#else 
		inline constexpr static const size_t _FNV_offset_basis = 2166136261U;
		inline constexpr static const size_t _FNV_prime = 16777619U;
#endif 

		inline static const constexpr size_t _Fnv1a_append_bytes(size_t _Val, const char* _First,
			const size_t _Count) noexcept
		{
			// accumulate range [_First, _First + _Count) into partial FNV-1a hash _Val
			for (size_t _Idx = 0; _Idx < _Count; ++_Idx)
			{
				_Val ^= static_cast<size_t>(_First[_Idx]);
				_Val *= _FNV_prime;
			}
			return _Val;
		}
	}


	struct Meta
	{
		struct MetaPiece
		{
			constexpr MetaPiece(const char* _t, const char* _v)
				: title(_t), value(_v)
			{}
			const std::string_view title = nullptr;
			const std::string_view value = nullptr;
		};
		constexpr Meta(const MetaPiece* _ms, uint32_t _mc)
			:metas(_ms), metaCount(_mc)
		{

		}
		template<int N>
		constexpr Meta(const MetaPiece(&_ms)[N])
			: metas(_ms), metaCount(N)
		{

		}
		constexpr Meta(const std::nullptr_t npt)
		{

		}
		const MetaPiece* metas = nullptr;//8 36
		const uint32_t metaCount = 0;//4 40
	};
	
	struct Field
	{
		constexpr Field(const std::string_view _n, std::string_view _t,
			uint32_t _o)
			:name(_n), type(_t), offset(_o)
		{
			
		}

		const std::string_view name;//8
		const std::string_view type;
		uint32_t offset;// 4 24
	};


	template<int N>
	struct FieldWithMeta final : public Field
	{
		constexpr FieldWithMeta(const std::string_view _n, std::string_view _t,
			uint32_t _o, const Meta::MetaPiece(&_ms)[N])
			: Field(_n, _t, _o), metas(_ms)
		{
			
		}
		constexpr int MetaCount() const
		{
			return N;
		}
		const Meta::MetaPiece(&metas)[N];
	};

	template<>
	struct FieldWithMeta<0> final : public Field
	{
		constexpr FieldWithMeta(const std::string_view _n, std::string_view _t,
			uint32_t _o, std::nullptr_t nptr)
			: Field(_n, _t, _o), metas(nptr)
		{
			
		}
		constexpr int MetaCount() const
		{
			return 0;
		}
		const Meta::MetaPiece* metas = nullptr;
	};
	
	// Reference is a non-const, type erased wrapper around any object.
	class Reference final
	{
	public:
		template<typename T>
		constexpr Reference(T& t);

		constexpr Reference();

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

	// Dynamic Part
	template<typename T>
	inline static const Reference GetFieldT(const Reference& o, const std::string& fieldname){assert(0); return Reference();};

	template<typename T,
		std::enable_if_t<!std::is_pointer_v<T>, int> = 0>
	inline static const bool constexpr isAtomic(){ return false; }
	template<typename T,
		std::enable_if_t<std::is_pointer_v<T>, int> = 0>
	inline static const bool constexpr isAtomic() { return true; }

	template<typename T>
	struct ClassInfo
	{
		inline static const constexpr char* GetClassName() { return "NULL"; }
		inline static constexpr const auto all_methods() { return std::make_tuple(); }
		inline static constexpr const auto all_fields() { return std::make_tuple(); }
		inline static constexpr const auto all_static_fields() { return std::make_tuple(); }
		inline static constexpr const auto all_static_methods() { return std::make_tuple(); }
	};
	template<typename T>
	struct EnumInfo
	{
		inline static const constexpr char* GetEnumName() { return "NULL"; }
		//inline static const constexpr Meta::MetaPiece meta = {...};
		//inline static const constexpr Meta::MetaPiece meta_EONE = {...}; 
		//inline static constexpr Meta perMeta[n] = {...};
	};

	using namespace std;
#define GEN_REFL_BASIC_TYPES_TWO_PARAM(T, NAME) \
	template<> inline const Reference GetFieldT<T>(const Reference& o, const std::string& fieldName){return o;}\
	template<> inline const constexpr bool isAtomic<T>(){return true;}\
	template<> struct ClassInfo<T>\
	{\
		inline static const constexpr char* GetClassName(){return #NAME;}\
		inline static constexpr const auto all_fields() { return std::make_tuple(); }\
		inline static constexpr const auto all_static_fields() { return std::make_tuple(); }\
		inline static constexpr const auto all_methods() { return std::make_tuple(); }\
		inline static constexpr const auto all_static_methods() { return std::make_tuple(); }\
		inline static const constexpr Meta::MetaPiece meta[1] =\
		{\
			{"description", "Class of "#T}\
		};\
	};


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
	GEN_REFL_BASIC_TYPES_TWO_PARAM(std::string_view, string_view);
	GEN_REFL_BASIC_TYPES_TWO_PARAM(pmr::string, string);
	GEN_REFL_BASIC_TYPES_TWO_PARAM(std::byte, byte);
	GEN_REFL_BASIC_TYPES_TWO_PARAM(const char*, const char*);

/*
	template<> struct ClassInfo<Field>
	{
		inline static const constexpr char* GetClassName() { return "Field"; }
		inline static const constexpr Meta::MetaPiece meta[1] =
		{
			{"description", "Field Informations."}
		};
		inline static constexpr const auto all_fields()
		{
			SFIELD_INFO(name, Field, nullptr)
			SFIELD_INFO(type, Field, nullptr)
			SFIELD_INFO(offset, Field, nullptr)
			SFIELD_INFO(metas, Field, nullptr)
			return hana::make_tuple(name_info(), type_info(), offset_info(), metas_info());
		}
	};

	template<> inline const Reference GetFieldT<Field>(const Reference& o, const std::string& name)
	{
		if (name == "name")
			return o.GetT<Field>().name;
		else if (name == "type")
			return o.GetT<Field>().type;
		else if (name == "offset")
			return o.GetT<Field>().offset;
		else if (name == "metas")
			return o.GetT<Field>().metas;
		assert(0 && "No field of this name.");
		return o;
	}
*/
	template<typename T>
	struct SEnum
	{
		using EnumName = std::decay_t<T>;
		using info = EnumInfo<EnumName>;
		inline static const constexpr char* GetName(){return info::GetEnumName();}
		inline static const constexpr Meta GetEnumMeta(){return Meta(info::meta);}
		inline static const constexpr std::size_t id_ =
			detail::_Fnv1a_append_bytes(Sakura::refl::detail::_FNV_offset_basis,
				info::GetEnumName(), detail::length(info::GetEnumName()) * sizeof(char));
		inline static const constexpr std::size_t GetTypeId() { return id_; }

		template <typename Fn>
		inline static constexpr void ForEachStaticField(Fn&& fn)
		{
			detail::ForEachTuple(info::all_static_fields(),
				[&fn](auto&& field_schema)
				{
					fn(field_schema.val, field_schema.fd);
				});
		}
	};

	has_member(all_fields);
	has_member(all_methods);
	has_member(all_static_fields);
	has_member(all_static_methods);

	template<typename T>
	struct SClass
	{
		using ClassName = std::decay_t<T>;
		using info = ClassInfo<ClassName>;
		inline static const constexpr char* GetName() { return info::GetClassName(); }
		inline static const constexpr Meta GetClassMeta() { return Meta(info::meta); }
		inline static const constexpr std::size_t id_ =
			detail::_Fnv1a_append_bytes(Sakura::refl::detail::_FNV_offset_basis,
				info::GetClassName(), detail::length(info::GetClassName()) * sizeof(char));
		inline static const constexpr std::size_t GetTypeId() { return id_; }
		inline static const constexpr std::size_t GetStaticFieldCount() noexcept
		{
			return 0;
		}
		inline static const Reference GetStaticField(const Reference& o, const std::string& fieldName)
		{
			return Reference();
		}
		inline static const auto GetFieldMeta(const std::string& name) noexcept
		{
			for (auto i = 0; i < GetFieldCount(); i++)
			{
				if (name == info::fields[i].name)
					return info::fields[i];
			}
			const char* _n = name.c_str();
			assert(0 && "No Field named" && _n);
			return info::fields[999999999];
		}

		inline static const constexpr std::size_t GetFieldCount() noexcept
		{
			return detail::arraySize(info::fields);
		}

		template <bool atomic, typename Fn, typename Tuple>
		inline static constexpr void __for_each_field_meta_impl(Tuple&& tup, Fn&& fn)
		{
			detail::ForEachTuple(std::forward<Tuple>(tup),
				[&fn](auto&& field_schema)
				{
					fn(field_schema);
				});
		}

		template <bool atomic, typename V, typename Fn, typename Tuple>
		inline static constexpr void __for_each_field_impl(Tuple&& tup, V&& value, Fn&& fn)
		{
			detail::ForEachTuple(std::forward<Tuple>(tup),
				[&value, &fn](auto&& field_schema)
				{
					using ClassName_C = std::decay_t<decltype(value.*(field_schema.ptr))>;
					if constexpr (!isAtomic<ClassName_C>() && atomic)
					{
						static_assert(has_member_all_fields<ClassInfo<ClassName_C>>::value);
						SClass<ClassName_C>::ForEachFieldAtomic(std::forward<ClassName_C>(value.*(field_schema.ptr)), fn);
						constexpr int leng = std::tuple_size(ClassInfo<ClassName_C>::all_static_fields());
						if constexpr (leng > 0)
						{
							static_assert(has_member_all_static_fields<ClassInfo<ClassName_C>>::value);
							SClass<ClassName_C>::ForEachStaticFieldAtomic(fn);
						}
					}
					else
						fn(value.*(field_schema.ptr), field_schema);
				});
		}

		template <bool atomic, typename Fn, typename Tuple>
		inline static constexpr void __for_each_static_field_impl(Tuple&& tup, Fn&& fn)
		{
			detail::ForEachTuple(std::forward<Tuple>(tup),
				[&fn](auto&& field_schema)
				{
					using ClassName_CS = std::decay_t<decltype(*(field_schema.ptr))>;
					if constexpr (!isAtomic<ClassName_CS>() && atomic)
						SClass<ClassName_CS>::ForEachFieldAtomic(*(field_schema.ptr), fn);
					else
						fn(*field_schema.ptr, field_schema);
				});
		}

		template <typename Fn>
		inline static constexpr void ForEachFieldMeta(Fn&& fn)
		{
			if constexpr (has_member_all_methods<ClassInfo<ClassName>>::value)
				__for_each_field_meta_impl<false>(info::all_fields(), fn);
		}
		template <typename V, typename Fn>
		inline static constexpr void ForEachField(V&& value, Fn&& fn)
		{
			if constexpr (has_member_all_methods<ClassInfo<ClassName>>::value)
				__for_each_field_impl<false>(info::all_fields(), value, fn);
			return;
		}
		template <typename V, typename Fn>
		inline static constexpr void ForEachFieldAtomic(V&& value, Fn&& fn)
		{
			if constexpr (has_member_all_methods<ClassInfo<ClassName>>::value)
				__for_each_field_impl<true>(info::all_fields(), value, fn);
			return;
		}

		template <typename Fn>
		inline static constexpr void ForEachStaticFieldMeta(Fn&& fn)
		{
			if constexpr (has_member_all_static_fields<ClassInfo<ClassName>>::value)
				__for_each_field_meta_impl<false>(info::all_static_fields(), fn);
			return;
		}
		template <typename Fn>
		inline static constexpr void ForEachStaticField(Fn&& fn)
		{
			if constexpr (has_member_all_static_fields<ClassInfo<ClassName>>::value)
				__for_each_static_field_impl<false>(info::all_static_fields(), fn);
			return;
		}
		template <typename Fn>
		inline static constexpr void ForEachStaticFieldAtomic(Fn&& fn)
		{
			if constexpr (has_member_all_static_fields<ClassInfo<ClassName>>::value)
				__for_each_static_field_impl<true>(info::all_static_fields(), fn);
			return;
		}

		template <typename Fn>
		inline static constexpr void ForEachMethodMeta(Fn&& fn)
		{
			if constexpr (has_member_all_methods<ClassInfo<ClassName>>::value)
				__for_each_field_meta_impl<false>(info::all_methods(), fn);
			return;
		}
		template <typename V, typename Fn>
		inline static constexpr void ForEachMethod(V&& value, Fn&& fn)
		{
			if constexpr (has_member_all_methods<ClassInfo<ClassName>>::value)
				detail::ForEachTuple(info::all_methods(),
					[&value, &fn](auto&& field_schema)
					{
						fn(field_schema.ptr, field_schema.fd);
					});
			return;
		}

		template <typename Fn>
		inline static constexpr void ForEachStaticMethodMeta(Fn&& fn)
		{
			if constexpr (has_member_all_static_methods<ClassInfo<ClassName>>::value)
				__for_each_field_meta_impl<false>(info::all_static_methods(), fn);
			return;
		}
		template <typename Fn>
		inline static constexpr void ForEachStaticMethod(Fn&& fn)
		{
			if constexpr (has_member_all_static_methods<ClassInfo<ClassName>>::value)
				detail::ForEachTuple(info::all_static_methods(),
					[&fn](auto&& field_schema)
					{
						fn(field_schema.ptr, field_schema.fd);
					});
			return;
		}



		// Dynamic Part
		inline static const Reference GetField(const Reference& o, const std::string& fieldName)
		{
			return GetFieldT<ClassName>(o, fieldName);
		}

		template<typename TT>
		inline static const TT GetField(const Reference& o, const std::string& fieldName)
		{
			return *static_cast<TT*>(GetFieldT<ClassName>(o, fieldName).data_);
		}
	};

	template <typename T>
	auto constexpr GetTypeId()
	{
		return SClass<T>::GetTypeId();
	}





	// Dynamic Part

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



	struct IType
	{
		virtual ~IType() = default;
		virtual const char* GetName() const = 0;
	};

	enum EFlag : uint32_t
	{
		EFunction = 1,
		EMethod = EFunction << 1,
		EAtomic = EMethod << 1,
		EStruct = EAtomic << 1,
		EClass = EStruct << 1,
		EEnum = EClass << 1,
		EConst = EEnum << 1,
		EPtr = EConst << 1,
		EReference = EPtr << 1,
		ETemplate = EReference << 1
	};
	using EFlags = uint32_t;



	template <typename T>
	inline Object::Object(T&& t)
		: id_(GetTypeId<std::decay_t<T>>()),
		data_(new std::decay_t<T>(std::forward<T>(t)))
	{
		// This is not part of the initializer list because it
		// doesn't compile on VC.
		deleter_ = [](void* data)
		{
			delete static_cast<std::decay_t<T>*>(data);
		};
	}

	inline Object::Object()
		: id_(SClass<void>::GetTypeId())
		, deleter_(NoOp)
	{

	}

	template<typename T>
	inline bool Object::IsT() const
	{
		return (SClass<T>::GetTypeId() == id_);
	}

	template<typename T>
	inline constexpr Reference::Reference(T& t)
		: id_(GetTypeId<std::remove_reference_t<T>>())
		, data_((void*)&t)
	{

	}

	inline constexpr Reference::Reference()
		: id_(GetTypeId<std::nullptr_t>()), data_(nullptr)
	{
			
	}

	template <typename T>
	inline bool Reference::IsT() const
	{
		return (SClass<T>::GetTypeId() == id_);
	}
}

namespace Sakura
{
	template<class... Fs>
	const typename refl::detail::overload_visitor<Fs...>::type overload(Fs... x)
	{
		return refl::detail::overload_visitor<Fs...>(x...);
	}
}
