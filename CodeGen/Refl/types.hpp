#pragma one
#include <string>
#include <vector>

namespace Sakura::AutoCoder
{
	struct TypeBase
	{
		enum class Type
		{
			Enum,
			Function,
			Class
		};

		inline static const std::string enumStr = "Enum";
		inline static const std::string functionStr = "Function";
		inline static const std::string classStr = "Class";

		static const std::string& Transform(Type type);

		TypeBase(std::string file, std::string full_name);
		~TypeBase();

		virtual Type GetType() const = 0;
		const std::string& GetFullName() const;
		const std::string& GetName() const;
		const std::string& GetFile() const;
	private:
		std::string file;
		std::string full_name;
	};

	struct Enum final : public TypeBase
	{
		using ValueList = std::vector<std::pair<std::string, int>>;
		Enum(std::string file, std::string full_name);
		Type GetType() const override final;

		ValueList values;
	};

	struct NamedObject
	{
		std::string name;
		std::string Type;
	};

	struct Function final : public TypeBase
	{
		Function(std::string file, std::string full_name);
		Type GetType() const;

		std::string name;
		std::vector<NamedObject> arguments;
		std::string returnType;
	};

	struct Class final : public TypeBase
	{
		using MethodList = std::vector<Function>;
		using FieldList = std::vector<NamedObject>;
		Class(std::string file, std::string full_name);
		Type GetType() const override final;

		MethodList methods;
		MethodList staticMethods;

		FieldList fields;
		FieldList staticFields;
	};
};