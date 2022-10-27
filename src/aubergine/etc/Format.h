#pragma once

#include <aubergine/Header.h>

template<typename T>
auto $convert(T&& value)
{
  if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value)
  {
    return std::forward<T>(value).c_str();
  }

  return std::forward<T>(value);
}

template<typename... Args>
std::string $format(const std::string& format, Args&&... args)
{
  const auto size = std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...);

  if (size <= 0)
  {
    throw std::runtime_error("Unable to format string: " + format);
  }

  const auto buffer = std::make_unique<char[]>(size + 1);

  std::snprintf(buffer.get(), size + 1, format.c_str(), std::forward<Args>(args)...);

  return std::string(buffer.get(), buffer.get() + size);
}

template<typename... Args>
std::string $(const std::string& format, Args&&... args)
{
  return $format(format, $convert(std::forward<Args>(args))...);
}
