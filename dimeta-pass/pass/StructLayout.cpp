
#include "Dimeta.h"
#include "DimetaData.h"

#include <llvm/ADT/STLExtras.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/Support/type_traits.h>
#include <memory>
#include <string>
#include <type_traits>

namespace ditest {

template <class... Ts> struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overload(Ts...) -> overload<Ts...>;

namespace detail {

using namespace llvm;
// using namespace clang;

static void PrintOffset(raw_ostream &OS, int Offset, unsigned IndentLevel) {
  OS << llvm::format("%10" PRId64 " | ", (int64_t)Offset);
  OS.indent(IndentLevel * 2);
}

static void PrintIndentNoOffset(raw_ostream &OS, unsigned IndentLevel) {
  OS << "           | ";
  OS.indent(IndentLevel * 2);
}

template <typename Type, typename Func>
auto apply_function(const Type &type, Func &&handle_qualified_type) {
  using namespace dimeta;

  if constexpr (std::is_same_v<Type, typename dimeta::QualifiedCompound> ||
                std::is_same_v<Type, typename dimeta::QualifiedFundamental>) {
    return std::forward<Func>(handle_qualified_type)(type);
  } else {
    return std::visit(
        [&](auto &&qualified_type) {
          return apply_function(qualified_type,
                                std::forward<Func>(handle_qualified_type));
        },
        type);
  }
}

template <typename Type> dimeta::ArraySize array_size(const Type &type) {
  return detail::apply_function(type, [](const auto &t) -> dimeta::Extent {
    if (t.array_size.size() > 1 || (t.is_vector && t.array_size.size() > 2)) {
      assert(false);
    }
    // Vector array-size does not count towards array type-size
    if (t.is_vector && t.array_size.size() == 1) {
      return 1;
    }
    const auto array_size_factor =
        t.array_size.empty() ? 1 : t.array_size.at(0);

    return array_size_factor;
  });
}

template <typename Type> std::string name_or_typedef_of(const Type &type) {
  return detail::apply_function(type, [](const auto &qual_type) {
    const bool no_typedef = qual_type.typedef_name.empty();

    if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<
                                     decltype(qual_type)>>,
                                 typename dimeta::QualifiedCompound>) {
      const bool no_identifier = qual_type.type.identifier.empty();
      const bool no_name = qual_type.type.name.empty();

      if (!no_typedef) {
        return qual_type.typedef_name;
      }

      if (no_identifier && no_name && no_typedef) {
        // return get_anon_struct_identifier(qual_type);
        assert(false);
      }

      if (no_identifier && no_name) {
        return qual_type.typedef_name;
      }

      if (!no_name) {
        return qual_type.type.name;
      }
      return qual_type.type.identifier;
    } else {
      if (qual_type.type.encoding == dimeta::FundamentalType::kFunctionPtr) {
        return std::string{"fptr"};
      }
    }

    return no_typedef ? qual_type.type.name : qual_type.typedef_name;
  });
}

template <typename Type> std::string pointer_of(const Type &type) {
  return detail::apply_function(type, [](const auto &qual_type) {
    std::string qual_string{};
    for (auto quals : qual_type.qual) {
      switch (quals) {
      case dimeta::Qualifier::kPtr:
        qual_string += "*";
        break;
      case dimeta::Qualifier::kRef:
        qual_string += "&";
        break;
      default:
        continue;
      }
    }
    if (!qual_string.empty()) {
      return " " + qual_string;
    }
    return qual_string;
  });
}

template <typename Type> std::string array_of(const Type &type) {
  std::string array;
  const auto extent = array_size(type);
  if (extent > 1) {
    array = "[" + std::to_string(extent) + "]";
  }
  return array;
}

template <typename Type> std::string const_of(const Type &type) {
  return detail::apply_function(type, [](const auto &qual_type) {
    std::string qual_string{};
    for (auto quals : qual_type.qual) {
      switch (quals) {
      case dimeta::Qualifier::kConst:
        qual_string += "const ";
        break;
      default:
        continue;
      }
    }
    return qual_string;
  });
}

template <typename Type> std::string name_of_type(const Type &type) {
  return detail::apply_function(type, [&](const auto &qual_type) {
    const auto name = name_or_typedef_of(qual_type);
    if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<
                                     decltype(qual_type)>>,
                                 typename dimeta::QualifiedCompound>) {
      if (!qual_type.type.name.empty() && name == qual_type.type.name) {
        switch (qual_type.type.type) {
        case dimeta::CompoundType::kClass:
          return "class " + name;
        case dimeta::CompoundType::kStruct:
          return "struct " + name;
        default:
          break;
        }
      }
    }
    return name;
  });
}

template <typename Type> std::string qualified_type(const Type &type) {
  return const_of(type) + name_of_type(type) + pointer_of(type) +
         array_of(type);
}

template <typename Type> bool is_ptr(const Type &type) {
  return !pointer_of(type).empty();
}

template <typename Type> bool is_vtableptr(const Type &type) {
  return detail::apply_function(type, [&](const auto &qual_type) {
    const auto name = name_or_typedef_of(qual_type);
    if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<
                                     decltype(qual_type)>>,
                                 typename dimeta::QualifiedFundamental>) {
      return qual_type.type.encoding == dimeta::FundamentalType::kVtablePtr;
    }
    return false;
  });
}

std::string vtableptr_of(const dimeta::CompoundType &compound) {
  std::string type{"("};
  type += (compound.name.empty() ? compound.identifier : compound.name);
  type += " vtable pointer)";
  return type;
}

std::string qualified_base_type(const dimeta::BaseClass &type, bool first) {
  std::string postfix{"("};
  if (!type.base.type.members.empty() &&
      is_vtableptr((*type.base.type.members.begin())->member)) {
    postfix += "primary ";
  }
  postfix += "base)";

  if (type.is_empty_base_class) {
    postfix += " (empty)";
  }
  return postfix;
}

} // namespace detail

void print_members(llvm::raw_ostream &out, const dimeta::CompoundType &compound,
                   int indent = 0, int offset_ = 0);

void print_bases(llvm::raw_ostream &out, const dimeta::CompoundType &compound,
                 int indent = 0, int offset_ = 0);

void print_struct(llvm::raw_ostream &out,
                  const dimeta::QualifiedCompound &qual_comp,
                  bool top_level = true, int indent = 0, int offset_ = 0) {
  using namespace detail;
  const auto &compound = qual_comp.type;

  int offset = offset_;

  if (top_level) {
    PrintOffset(out, offset, indent);
    out << name_of_type(qual_comp) << "\n";
  }

  ++indent;
  print_bases(out, qual_comp.type, indent, offset);
  print_members(out, qual_comp.type, indent, offset);

  if (top_level) {
    PrintIndentNoOffset(out, indent - 1);
    out << "[sizeof=" << qual_comp.type.extent << "]\n";
  }
}

void print_bases(llvm::raw_ostream &out, const dimeta::CompoundType &compound,
                 int indent, int offset_) {
  using namespace detail;
  int offset = offset_;

  for (const auto &ith_base : llvm::enumerate(compound.bases)) {
    const auto base_pointer = ith_base.value();
    offset = ith_base.value()->offset + offset_;
    PrintOffset(out, offset, indent);

    out << qualified_type(base_pointer->base) << ' '
        << detail::qualified_base_type(*base_pointer.get(),
                                       ith_base.index() == 0)
        << '\n';

    print_members(out, base_pointer->base.type, indent + 1, offset);
  }
}

void print_members(llvm::raw_ostream &out, const dimeta::CompoundType &compound,
                   int indent, int offset_) {
  using namespace detail;
  int offset = offset_;
  for (const auto &ith_member : llvm::enumerate(compound.members)) {
    const auto member_pointer = ith_member.value();
    offset = compound.offsets.at(ith_member.index()) + offset_;
    PrintOffset(out, offset, indent);

    if (is_vtableptr(ith_member.value()->member)) {
      out << vtableptr_of(compound) << "\n";
      continue;
    }

    out << qualified_type(member_pointer->member) << ' ' << member_pointer->name
        << '\n';

    if (!detail::is_ptr(member_pointer->member)) {
      if (auto *pval =
              std::get_if<dimeta::QualifiedCompound>(&member_pointer->member)) {
        print_struct(out, *pval, false, indent, offset);
      }
    }
  }
}

void print_layout(llvm::raw_ostream &out,
                  const dimeta::QualifiedCompound &type) {
  print_struct(out, type);
  out << "\n";
}

} // namespace ditest